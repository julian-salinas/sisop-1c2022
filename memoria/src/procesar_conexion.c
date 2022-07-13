#include "procesar_conexion.h"

void procesar_conexion(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    tamanio_tabla = memoria_config -> tamanio_pagina * memoria_config -> paginas_por_tabla;
    free(args);

    int8_t header;
    header = recibir_header(socket_cliente);

    switch (header) {

        case KERNEL: // Handshake inicial con Kernel
            log_info(logger, "Se conectó Kernel - header %d", header);
            procesar_conexion_kernel_memoria(socket_cliente);
            break;

        case CPU: // Handshake inicial con CPU
            log_info(logger, "Se conectó CPU - header %d", header);
            procesar_conexion_cpu_memoria(socket_cliente);
            break;

        case -1:
            log_error(logger, "Cliente desconectado de %s...", nombre_servidor);
            break;

        default:
            log_error(logger, "Algo anduvo mal en el server de %s", nombre_servidor);
            log_info(logger, "Cop: %d", header);
            break;
    }

    return;
}


void procesar_conexion_kernel_memoria(int socket_cliente) {
    int8_t header;
    int id_tabla_creada;
    t_PCB* pcb;
    t_list* entradas_a_swappear;
    t_tabla_primer_nivel* tp_lvl1;
    t_tabla_segundo_nivel* tp_lvl2;
    t_paquete* respuesta;

    while (1) {
        header = recibir_header(socket_cliente);
        log_info(logger, "Conexion con Kernel - Se recibio header %d", header);

        switch (header) {

            case NUEVO_PROCESO:
                //retardo cpu
                usleep(memoria_config -> retardo_memoria * 1000);
                
                pcb = socket_get_PCB(socket_cliente);
                log_info(logger, "Nuevo proceso - ID:%d", pcb -> PID);
                
                id_tabla_creada = crear_proceso_memoria(pcb);

                if (!id_tabla_creada) {
                    enviar_pcb(socket_cliente, PROCESO_RECHAZADO, pcb);
                }

                //Modificar pcb agregando el valor de tabla de paginas
                pcb -> tabla_paginas = id_tabla_creada;

                //Devolver pcb al kernel
                respuesta = crear_paquete(MEMORIA_OK, sizeof(uint32_t));
                agregar_a_buffer_INT32(respuesta -> payload, id_tabla_creada);
                enviar_paquete(socket_cliente, respuesta);
                log_info(logger, "Se devolvió PCB del proceso %d con tabla de páginas %d", pcb -> PID, id_tabla_creada);

                break;
            

            case PROCESO_SUSPENDIDO:
                //retardo cpu
                usleep(memoria_config -> retardo_memoria * 1000);

                pcb = socket_get_PCB(socket_cliente);
                log_info(logger, "Proceso SUSPENDIDO - ID:%d", pcb -> PID);

                //obtengo las entradas que esten en memoria y las swappeo
                entradas_a_swappear = get_entradas_en_memoria_proceso(pcb -> PID);
                for (uint32_t i = 0; i < list_size(entradas_a_swappear); i++)
                {
                    swappear(pcb -> PID, list_get(entradas_a_swappear, i));
                }

                //Devolver pcb al kernel
                enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
                break;


            case PROCESO_FINALIZADO:
                //retardo cpu
                usleep(memoria_config -> retardo_memoria * 1000);

                pcb = socket_get_PCB(socket_cliente);
                log_info(logger, "Proceso FINALIZADO - ID:%d", pcb -> PID);
                
                //libero memoria de las tablas
                tp_lvl1 = get_tabla_primer_nivel(pcb -> tabla_paginas);
                for (uint32_t i = 0; i < list_size(tp_lvl1 -> entradas); i++)
                {
                    tp_lvl2 = get_tabla_segundo_nivel((int) list_get(tp_lvl1 -> entradas, i));
                    free((void*)tp_lvl2 -> entradas);
                }
                free((void*)tp_lvl1 -> entradas);

                //PREGUNTA: Hace falta sacar la tabla del diccionario? - noup

                //borro archivo swap
                destruir_archivo_swap(pcb -> PID);

                enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
                break;

            case -1:
                log_error(logger, "Kernel se desconectó");
                return;

            default:
                log_error(logger, "Kernel acaba de mandarnos cualquier cosa - %d", header);
                return;
        }
    }
}


void procesar_conexion_cpu_memoria(int socket_cliente) {
    int8_t header;
    int32_t nro_pagina, nro_tabla_primer_nivel, nro_tabla_segundo_nivel, direccion_fisica, nro_frame, nro_entrada_primer_nivel;
    uint32_t dato, PID;
    t_buffer* payload;
    t_tabla_primer_nivel* tabla_primer_nivel;
    t_tabla_segundo_nivel* tabla_segundo_nivel;
    t_entrada_segundo_nivel* entrada;
    
    while (1) {
        header = recibir_header(socket_cliente);
        log_info(logger, "Conexion con CPU - Se recibió header %d", header);

        switch (header) {

            case CONEXION_CPU_MEMORIA:
                //enviar a CPU cantidad de entradas por tabla de páginas y tamaño de página;
                log_info(logger, "Se recibió header %d - Enviando config a CPU", header);
                log_info(logger, "Socket cliente memoria-cpu creado.");
                enviar_config_a_cpu(socket_cliente, logger, memoria_config->paginas_por_tabla, memoria_config->tamanio_pagina);	
                break;

            case PRIMER_ACCESO_MEMORIA:
                log_info(logger, "CASE: Primer acceso a memoria");

                //retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);

                log_info(logger, "Terminó usleep");
                
                //recibo el nro de tabla de primer nivel y pagina buscada
                payload = recibir_payload(socket_cliente);
                PID = buffer_take_UINT32(payload); // Número de página a la que se desea acceder
                nro_entrada_primer_nivel = buffer_take_INT32(payload); // Nro TP primer nivel del proceso donde vamos a buscar la página

                log_info(logger, "Se leyó paquete enviado por cpu");

                // obtengo nro de tabla de segundo nivel
                // nro_tabla_segundo_nivel = get_nro_tabla_segundo_nivel_pagina(nro_tabla_primer_nivel, nro_pagina);
                
                tabla_primer_nivel = get_tabla_primer_nivel(PID);
                nro_tabla_segundo_nivel = list_get(tabla_primer_nivel -> entradas, nro_entrada_primer_nivel);

                log_info(logger, "Se obtuvo tabla de 2do nivel");
                
                //le mando a cpu el nro de tabla de segundo nivel
                enviar_boludeces_a_cpu(socket_cliente, nro_tabla_segundo_nivel);
                
                break;
            
            case SEGUNDO_ACCESO_MEMORIA:
                log_info(logger, "CASE: Segundo acceso a memoria");

                //retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);
                
                //recibo el nro de tabla de primer nivel y su entrada
                payload = recibir_payload(socket_cliente);
                nro_tabla_segundo_nivel = buffer_take_UINT32(payload);
                nro_pagina = buffer_take_UINT32(payload);
                PID = buffer_take_UINT32(payload);

                log_info(logger, "Se leyeron los datos enviados por CPU");

                //obtengo la tabla de segundo nivel y el nro de marco de la entrada de segundo nivel
                tabla_segundo_nivel = (t_tabla_segundo_nivel*)dictionary_get(tablas_segundo_nivel, int_a_string(nro_tabla_segundo_nivel));
                entrada = get_entrada_de_pagina(tabla_segundo_nivel, nro_pagina);

                log_info(logger, "Se pudo obtener tabla de segundo nivel y entrada");
                
                if (!entrada -> bit_presencia) {
                    desswappear(PID, entrada);
                }

                log_info(logger, "Enviando marco %d a CPU", entrada -> nro_frame);

                //le mando el nro de marco al cpu
                enviar_boludeces_a_cpu(socket_cliente, entrada -> nro_frame);

                break;
            
            // case TERCER_ACCESO_MEMORIA:
            //     //retardo memoria
            //     usleep(memoria_config -> retardo_memoria * 1000);
                
            //     payload = recibir_payload(socket_cliente);
            //     nro_frame = buffer_take_INT32(payload);
            //     direccion_fisica = nro_frame * memoria_config -> tamanio_pagina;
            //     enviar_boludeces_a_cpu(socket_cliente, direccion_fisica);

            //     break; 
            
            case LEER_MEMORIA:
                log_info(logger, "CASE: Leer memoria");

                // Retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);

                // Obtener dirección que envió CPU
                payload = recibir_payload(socket_cliente);
                direccion_fisica = buffer_take_INT32(payload);
                
                // Leer dato y enviárselo a CPU
                dato = leer_direccion_memoria(direccion_fisica);
                enviar_boludeces_a_cpu(socket_cliente, dato);
                
                break;
            
            case ESCRIBIR_EN_MEMORIA:
                log_info(logger, "CASE: Escribir memoria");

                // Retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);

                // Obtener dirección que envió CPU
                payload = recibir_payload(socket_cliente);
                direccion_fisica = buffer_take_INT32(payload);
                dato = buffer_take_UINT32(payload);

                escribir_direccion_memoria(direccion_fisica, dato);

                log_info(logger, "Pedido de escritura de dato %d en la posicion: %d", dato, direccion_fisica);

                // Escribir dato y enviar mensaje OK
                enviar_header(socket_cliente, MEMORIA_OK);
                
                break;
            
            case -1:
                log_error(logger, "CPU se desconectó");
                return;

            default:
                log_error(logger, "CPU acaba de mandarnos cualquier cosa - %d", header);
                return;
        }
    }
}


void enviar_config_a_cpu(int socket_cliente, t_log* logger, uint8_t paginas_por_tabla, uint8_t tamanio_pagina){

    t_paquete* paquete = serializar_config_cpu_memoria(paginas_por_tabla, tamanio_pagina);
    enviar_paquete(socket_cliente,paquete);
    destruir_paquete(paquete);
    log_info(logger, "Config enviado.");

}


void enviar_boludeces_a_cpu(int socket_cliente, int32_t nro_tabla_segundo_nivel) {
    t_paquete* paquete = crear_paquete(MEMORIA_OK, sizeof(int32_t));
    agregar_a_buffer_UINT32(paquete -> payload, nro_tabla_segundo_nivel);
    enviar_paquete(socket_cliente, paquete);
    destruir_paquete(paquete);
}


int crear_proceso_memoria(t_PCB* pcb) {
    // Validar que el tamaño del proceso no está re zarpado
    if (pcb -> tamanio > (memoria_config -> tamanio_pagina * memoria_config -> paginas_por_tabla * memoria_config -> paginas_por_tabla)) {
        log_error(logger, "Proceso con ID:%d es demasiado grande y no puede alojarse en memoria", pcb -> PID);
        return 0; 
    }

    // Calcular cuantos frames va a necesitar el proceso (cuantas páginas) - Redondear para arriba
    int cantidad_frames_necesarios = round_div_up(pcb -> tamanio, memoria_config -> tamanio_pagina);

    // Calcular cantidad de tablas de segundo nivel necesarias - Redondear para arriba
    int cantidad_tablas_necesarias = round_div_up(cantidad_frames_necesarios, memoria_config -> paginas_por_tabla);

    /* ESTRUCTURAS NECESARIAS PARA PROCESO */

    // Tabla de primer nivel
    t_tabla_primer_nivel* tabla_primer_nivel = crear_tabla_primer_nivel(pcb -> PID);

    // Tablas de segundo nivel
    t_tabla_segundo_nivel* tabla_segundo_nivel;
    for(int i = 0; i < cantidad_tablas_necesarias; i++) {
        tabla_segundo_nivel = crear_tabla_segundo_nivel();

        for (int j = 0; j < memoria_config -> paginas_por_tabla; j++) {
            if (cantidad_frames_necesarios) {
                agregar_entrada_segundo_nivel(tabla_segundo_nivel, j);
                    
                log_info(logger, "Se agregó una entrada a la tabla de segundo nivel %d del proceso ID:%d", 
                        tabla_segundo_nivel -> id_tabla, 
                        pcb -> PID);

                cantidad_frames_necesarios--;
                continue;
            }
            break;
        } 
        agregar_entrada_primer_nivel(tabla_primer_nivel, tabla_segundo_nivel -> id_tabla);
        log_info(logger, "Se agregó una entrada a la tabla de primer nivel del proceso %d", pcb -> PID);
    }

    // Archivo swap    
    crear_archivo_swap(pcb -> PID, pcb -> tamanio);

    return tabla_primer_nivel -> id_tabla;
}
