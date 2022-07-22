#include "procesar_conexion.h"

void procesar_conexion(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    tamanio_tabla = memoria_config -> tamanio_pagina * memoria_config -> paginas_por_tabla;
    free(args);

    int8_t header = recibir_header(socket_cliente);

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
            log_info(logger, "Header recibido: %d", header);
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
    t_entrada_segundo_nivel* entrada;

    while (1) {
        header = recibir_header(socket_cliente);
        log_info(logger, "Conexion con Kernel - Se recibio header %d", header);

        switch (header) {

            case NUEVO_PROCESO:
                log_info(logger, "Se recibió un nuevo proceso");
                //retardo cpu
                usleep(memoria_config -> retardo_memoria * 1000);
                
                pcb = socket_get_PCB(socket_cliente);
                log_info(logger, "Nuevo proceso - ID:%d", pcb -> PID);
                
                id_tabla_creada = crear_proceso_memoria(pcb);

                if (!id_tabla_creada) {
                    enviar_pcb(socket_cliente, PROCESO_RECHAZADO, pcb);
                    destruir_PCB(pcb);
                }

                //Modificar pcb agregando el valor de tabla de paginas
                pcb -> tabla_paginas = id_tabla_creada;

                //Devolver pcb al kernel
                respuesta = crear_paquete(MEMORIA_OK, sizeof(uint32_t));
                agregar_a_buffer_UINT32(respuesta -> payload, id_tabla_creada);
                enviar_paquete(socket_cliente, respuesta);
                destruir_paquete(respuesta);
                log_info(logger, "Se devolvió PCB del proceso %d con tabla de páginas %d", pcb -> PID, id_tabla_creada);
                destruir_PCB(pcb);

                break;
            

            case PROCESO_SUSPENDIDO:
                log_info(logger, "Se recibió un proceso suspendido");
                //retardo cpu
                usleep(memoria_config -> retardo_memoria * 1000);

                pcb = socket_get_PCB(socket_cliente);
                log_info(logger, "Proceso suspendido - ID:%d", pcb -> PID);

                //obtengo las entradas que esten en memoria y las swappeo
                entradas_a_swappear = get_entradas_en_memoria_proceso(pcb -> PID);
                for (uint32_t i = 0; i < list_size(entradas_a_swappear); i++)
                {
                    swappear(pcb -> PID, list_get(entradas_a_swappear, i));
                }

                list_destroy(entradas_a_swappear);

                //Devolver pcb al kernel
                enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
                destruir_PCB(pcb);
                break;


            case PROCESO_FINALIZADO:
                log_info(logger, "Se recibió un proceso finalizado");

                //retardo cpu
                usleep(memoria_config -> retardo_memoria * 1000);

                pcb = socket_get_PCB(socket_cliente);
                log_info(logger, "Proceso finalizado - ID:%d", pcb -> PID);
                
                //libero memoria de las tablas
                tp_lvl1 = get_tabla_primer_nivel(pcb -> tabla_paginas);
                for (uint32_t i = 0; i < list_size(tp_lvl1 -> entradas); i++)
                {
                    tp_lvl2 = get_tabla_segundo_nivel((int) list_get(tp_lvl1 -> entradas, i));
                    // free((void*)tp_lvl2 -> entradas);
                    
                    for (uint32_t j = 0; j < list_size(tp_lvl2 -> entradas); j++) {
                        entrada = (t_entrada_segundo_nivel*) list_get(tp_lvl2 -> entradas, 0); 
                        
                        if (entrada -> nro_frame != -1) {
                            liberar_frame_n(entrada -> nro_frame);
                        }

                        entrada -> nro_frame = -1;
                        entrada -> bit_presencia = 0;
                        entrada -> bit_uso = 0;
                        entrada -> bit_modificado = 0;
                    }
                }
                // free((void*)tp_lvl1 -> entradas);

                //borro archivo swap
                destruir_archivo_swap(pcb -> PID);

                enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
                destruir_PCB(pcb);
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
    int32_t nro_pagina, nro_tabla_segundo_nivel, direccion_fisica, nro_entrada_primer_nivel;
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
                log_info(logger, "Enviando config a CPU", header);
                log_warning(logger, "Entradas por tabla:%d - Tamaño de página:%d", memoria_config->paginas_por_tabla, memoria_config->tamanio_pagina);
                enviar_config_a_cpu(socket_cliente, logger, memoria_config->paginas_por_tabla, memoria_config->tamanio_pagina);	
                break;

            case PRIMER_ACCESO_MEMORIA:
                log_info(logger, "Primer acceso a memoria");

                //retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);

                //recibo el nro de tabla de primer nivel y pagina buscada
                payload = recibir_payload(socket_cliente);
                PID = buffer_take_UINT32(payload); // Número de página a la que se desea acceder
                nro_entrada_primer_nivel = buffer_take_INT32(payload); // Nro TP primer nivel del proceso donde vamos a buscar la página

                destruir_buffer(payload);

                tabla_primer_nivel = get_tabla_primer_nivel(PID);
                nro_tabla_segundo_nivel = (int) list_get(tabla_primer_nivel -> entradas, nro_entrada_primer_nivel);

                log_info(logger, "Enviando tabla de 2do nivel - %d", nro_tabla_segundo_nivel);
                
                //le mando a cpu el nro de tabla de segundo nivel
                enviar_boludeces_a_cpu(socket_cliente, nro_tabla_segundo_nivel);
                
                break;
            
            case SEGUNDO_ACCESO_MEMORIA:
                log_info(logger, "Segundo acceso a memoria");

                //retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);
                
                //recibo el nro de tabla de primer nivel y su entrada
                payload = recibir_payload(socket_cliente);
                nro_tabla_segundo_nivel = buffer_take_UINT32(payload);
                nro_pagina = buffer_take_UINT32(payload);
                PID = buffer_take_UINT32(payload);

                destruir_buffer(payload);

                //obtengo la tabla de segundo nivel y el nro de marco de la entrada de segundo nivel
                sem_wait(mutex_tablas_segundo_nivel);
                char* numero_tabla = int_a_string(nro_tabla_segundo_nivel);
                    tabla_segundo_nivel = (t_tabla_segundo_nivel*) dictionary_get(tablas_segundo_nivel, numero_tabla);
                sem_post(mutex_tablas_segundo_nivel);
                free(numero_tabla);

                entrada = get_entrada_de_pagina(tabla_segundo_nivel, nro_pagina);

                if (!entrada -> bit_presencia) {
                    desswappear(PID, entrada);
                }

                log_info(logger, "Enviando marco %d a CPU", entrada -> nro_frame);

                //le mando el nro de marco al cpu
                enviar_boludeces_a_cpu(socket_cliente, entrada -> nro_frame);

                break;
            
            case LEER_MEMORIA:
                log_info(logger, "Se recibió pedido de lectura");

                // Retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);

                // Obtener dirección que envió CPU
                payload = recibir_payload(socket_cliente);
                direccion_fisica = buffer_take_INT32(payload);

                destruir_buffer(payload);

                entrada = obtener_entrada_por_DF(direccion_fisica);

                entrada -> bit_uso = 1;
                
                // Leer dato y enviárselo a CPU
                dato = leer_direccion_memoria(direccion_fisica);
                enviar_boludeces_a_cpu(socket_cliente, dato);
                
                break;
            
            case ESCRIBIR_EN_MEMORIA:
                log_info(logger, "Pedido de escritura");

                // Retardo memoria
                usleep(memoria_config -> retardo_memoria * 1000);

                // Obtener dirección que envió CPU
                payload = recibir_payload(socket_cliente);
                direccion_fisica = buffer_take_INT32(payload);
                dato = buffer_take_UINT32(payload);

                destruir_buffer(payload);

                log_info(logger, "Pedido de escritura de dato %d en la posicion: %d", dato, direccion_fisica);

                escribir_direccion_memoria(direccion_fisica, dato);

                log_info(logger, "Se escribió el dato en memoria");

                // TODO: Sacar esto antes de entregar
                if (dato != leer_direccion_memoria(direccion_fisica)) {
                    log_error(logger, "El dato se escribió erróneamente");
                }
                else {
                    log_info(logger, "El dato se escribió perfecto!");
                }

                entrada = obtener_entrada_por_DF(direccion_fisica);

                entrada -> bit_modificado = 1;
                entrada -> bit_uso = 1;

                log_info(logger, "Enviando respuesta a CPU...");

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


void enviar_config_a_cpu(int socket_cliente, t_log* logger, uint32_t paginas_por_tabla, uint32_t tamanio_pagina){

    t_paquete* paquete = serializar_config_cpu_memoria(paginas_por_tabla, tamanio_pagina);
    enviar_paquete(socket_cliente,paquete);
    destruir_paquete(paquete);
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
    int contador_nro_pag = 0;
    for(int i = 0; i < cantidad_tablas_necesarias; i++) {
        tabla_segundo_nivel = crear_tabla_segundo_nivel();

        for (int j = 0; j < memoria_config -> paginas_por_tabla; j++) {
            if (cantidad_frames_necesarios) {
                agregar_entrada_segundo_nivel(tabla_segundo_nivel, contador_nro_pag);
                contador_nro_pag++;
                    
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
