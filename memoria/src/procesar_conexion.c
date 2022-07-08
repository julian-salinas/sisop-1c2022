#include "procesar_conexion.h"


void procesar_conexion(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    tamanio_tabla = memoria_config -> tamanio_pagina * memoria_config -> paginas_por_tabla;
    free(args);

    int header;
    header = recibir_header(socket_cliente);
    t_PCB* pcb;

    log_info(logger, "Se recibió el header %d", header);

    int32_t nro_pagina;
    int32_t nro_tabla_segundo_nivel;
    t_buffer* payload;

    switch (header) {

        case CONEXION_CPU_MEMORIA:
            //enviar a CPU cantidad de entradas por tabla de páginas y tamaño de página;
            // conexion_cpu = crear_socket_cliente(memoria_config->ip_memoria, memoria_config->puerto_escucha);
            log_info(logger, "Socket cliente memoria-cpu creado.");
            enviar_config_a_cpu(socket_cliente, logger, memoria_config->paginas_por_tabla, memoria_config->tamanio_pagina);	
            break;


        case KERNEL: // Handshake inicial con Kernel

            while (1) {
                header = recibir_header(socket_cliente);
                log_info(logger, "Conexion con Kernel - Se recibio header %d", header);

                switch (header) {

                    case NUEVO_PROCESO:
                        //retardo cpu
                        usleep(memoria_config -> retardo_memoria * 1000);

                        pcb = socket_get_PCB(socket_cliente);
                        
                        int id_tabla_creada = crear_proceso_memoria(pcb);

                        if (!id_tabla_creada) {
                            enviar_pcb(socket_cliente, PROCESO_RECHAZADO, pcb);
                        }

                        //Modificar pcb agregando el valor de tabla de paginas
                        pcb -> tabla_paginas = id_tabla_creada;

                        //Devolver pcb al kernel
                        enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
                        log_info(logger, "Se devolvió PCB con tabla de páginas");

                        break;
                    

                    case PROCESO_SUSPENDIDO:
                        //retardo cpu
                        usleep(memoria_config -> retardo_memoria * 1000);

                        pcb = socket_get_PCB(socket_cliente);

                        //obtengo las entradas que esten en memoria y las swappeo
                        t_list* entradas_a_swappear = get_entradas_en_memoria_proceso(pcb -> PID);
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

                        //libero memoria de las tablas
                        t_tabla_primer_nivel* tp_lvl1 = get_tabla_primer_nivel(pcb -> tabla_paginas);
                        for (uint32_t i = 0; i < list_size(tp_lvl1 -> entradas); i++)
                        {
                            t_tabla_segundo_nivel* tp_lvl2 = get_tabla_segundo_nivel((t_tabla_segundo_nivel*) list_get(tp_lvl1 -> entradas, i));
                            free((void*)tp_lvl2 -> entradas);
                        }
                        free((void*)tp_lvl1 -> entradas);
                        //PREGUNTA: Hace falta sacar la tabla del diccionario?

                        //borro archivo swap
                        destruir_archivo_swap(pcb -> PID);

                        enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
                        break;
                }
            }

        case PRIMER_ACCESO_MEMORIA:

            //retardo memoria
            usleep(memoria_config -> retardo_memoria * 1000);
            
            //recibo el nro de tabla de primer nivel y pagina buscada
            payload = recibir_payload(socket_cliente);
            int32_t nro_tabla_primer_nivel = buffer_take_INT32(payload); // Nro TP primer nivel del proceso donde vamos a buscar la página
            nro_pagina = buffer_take_INT32(payload); // Número de página a la que se desea acceder

            //obtengo nro de tabla de segundo nivel
            nro_tabla_segundo_nivel = get_nro_tabla_segundo_nivel_pagina(nro_tabla_primer_nivel, nro_pagina);
            
            //le mando a cpu el nro de tabla de segundo nivel
            enviar_boludeces_a_cpu(nro_tabla_segundo_nivel);

            break;
        

        case SEGUNDO_ACCESO_MEMORIA:
            /*
            CPU tiene que enviar:
                - INT32 - Número de tabla de segundo nivel
                - INT32 - Número de página buscada
                - INT32 - PID
            */

            //retardo memoria
            usleep(memoria_config -> retardo_memoria * 1000);
            
            //recibo el nro de tabla de primer nivel y su entrada
            payload = recibir_payload(socket);
            nro_tabla_segundo_nivel = buffer_take_INT32(payload);
            nro_pagina = buffer_take_INT32(payload);
            int32_t PID = buffer_take_INT32(payload);

            //obtengo la tabla de segundo nivel y el nro de marco de la entrada de segundo nivel
            t_tabla_segundo_nivel* tabla_segundo_nivel = (t_tabla_segundo_nivel*)dictionary_get(tablas_segundo_nivel, int_a_string(nro_tabla_segundo_nivel));
            t_entrada_segundo_nivel* entrada = get_entrada_de_pagina(tabla_segundo_nivel, nro_pagina);

            if (!entrada -> bit_presencia) {
                desswappear(PID, entrada);
            }

            log_info(logger, "Enviando marco %d a CPU", entrada -> nro_frame);

            //le mando el nro de marco al cpu
            enviar_boludeces_a_cpu(entrada -> nro_frame);

            break;
        

        case TERCER_ACCESO_MEMORIA:

            //retardo cpu
            usleep(memoria_config -> retardo_memoria * 1000);
            
            int32_t nro_frame = buffer_take_INT32(payload);
            t_frame* frame = get_frame(nro_frame);

            // acceder a la porción de memoria correspondiente 
            enviar_boludeces_a_cpu(hexa_a_int(frame -> puntero_frame));
            
            break; 


        case -1:
            log_error(logger, "Cliente desconectado de %s...", nombre_servidor);
            return;


        default:
            log_error(logger, "Algo anduvo mal en el server de %s", nombre_servidor);
            log_info(logger, "Cop: %d", header);
            return;
    }

    return;
}


void enviar_config_a_cpu(int socket_cliente, t_log* logger, uint8_t paginas_por_tabla, uint8_t tamanio_pagina){

    t_paquete* paquete = serializar_config_cpu_memoria(paginas_por_tabla, tamanio_pagina);
    enviar_paquete(socket_cliente,paquete);
    destruir_paquete(paquete);
    log_info(logger, "Config enviado.");

}


void enviar_boludeces_a_cpu(int32_t nro_tabla_segundo_nivel) {
    t_paquete* paquete = crear_paquete(MEMORIA_OK, sizeof(int32_t));
    agregar_a_buffer_INT32(paquete -> payload, nro_tabla_segundo_nivel);
    enviar_paquete(socket, paquete);
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