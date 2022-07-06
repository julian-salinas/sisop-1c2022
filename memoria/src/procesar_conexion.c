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

    switch (header) {

        case CONEXION_CPU_MEMORIA:
            //enviar a CPU cantidad de entradas por tabla de páginas y tamaño de página;
            conexion_cpu = crear_socket_cliente(memoria_config->ip_memoria, memoria_config->puerto_escucha);
            log_info(logger, "Socket cliente memoria-cpu creado.");
            enviar_config_a_cpu(conexion_cpu, logger, memoria_config->paginas_por_tabla, memoria_config->tamanio_pagina);	
            break;


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
            break;


        case PROCESO_SUSPENDIDO:

            //retardo cpu
            usleep(memoria_config -> retardo_memoria * 1000);

            //se escribe en swap la info necesaria
            //se actulizan los bits de las tablas
            //Devolver pcb al kernel
            enviar_pcb(socket_cliente, MEMORIA_OK, pcb); //no haría falta que el pcb vaya y venga, pero así nos queda igual en todos lados
            break;


        case PROCESO_FINALIZADO:
        
            //retardo cpu
            usleep(memoria_config -> retardo_memoria * 1000);

            //se actualizan los bits de las tablas
            //se elimina su archivo de swap
            enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
            break;


        case PRIMER_ACCESO_MEMORIA:

            //retardo cpu
            usleep(memoria_config -> retardo_memoria * 1000);
            
            //recibo el nro de tabla de primer nivel y su entrada
            t_buffer* payload = recibir_payload(socket_cliente);
            int32_t nro_tabla_primer_nivel = buffer_take_INT32(payload);
            int32_t entrada_tabla_primer_nivel = buffer_take_INT32(payload);

            //obtengo la tabla de primer nivel y el nro de tabla de segundo nivel
            t_tabla_primer_nivel* tabla_primer_nivel = (t_tabla_primer_nivel*) dictionary_get(tablas_primer_nivel, int_a_string(nro_tabla_primer_nivel));
            int32_t nro_tabla_segundo_nivel = (int32_t) list_get(tabla_primer_nivel -> entradas, entrada_tabla_primer_nivel);
            
            //le mando a cpu el nro de tabla de segundo nivel
            enviar_boludeces_a_cpu(nro_tabla_segundo_nivel);

            break;
        

        // NOTA: DEJO ESTO COMENTADO PORQUE TIENE REDEFINICIONES - ARREGLAR Y DESCOMENTAR

        /*
        case SEGUNDO_ACCESO_MEMORIA:

            //retardo cpu
            usleep(memoria_config -> retardo_memoria * 1000);
            
            //recibo el nro de tabla de primer nivel y su entrada
            t_buffer* payload = recibir_payload(socket);
            int32_t nro_tabla_segundo_nivel = buffer_take_INT32(payload);
            int32_t entrada_tabla_segundo_nivel = buffer_take_INT32(payload);

             //obtengo la tabla de segundo nivel y el nro de marco de la entrada de segundo nivel
            t_tabla_segundo_nivel* tabla_segundo_nivel = (t_tabla_segundo_nivel*)dictionary_get(tablas_segundo_nivel, string_from_format("%d", nro_tabla_segundo_nivel));
            t_entrada_segundo_nivel* entrada = (t_entrada_segundo_nivel*) list_get(tabla_segundo_nivel -> entradas, entrada_tabla_segundo_nivel);

            // Si el marco no está en memoria, pasarlo a memoria
            validar_entrada_en_memoria(entrada);

            //le mando el nro de marco al cpu
            enviar_boludeces_a_cpu(entrada -> nro_frame);

            break;
        

        case TERCER_ACCESO_MEMORIA:

            //retardo cpu
            usleep(memoria_config -> retardo_memoria * 1000);
            
            // acceder a la porción de memoria correspondiente 
            break; 
        */


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


// no sé en qué estaba pensando cuando hice esta función, creo que no tiene ningún sentido, porlas la dejo
// void validar_entrada_en_memoria(t_entrada_segundo_nivel* entrada) {
//     // Si el bit de presencia está en 1, vaya y pase
//     if (entrada -> bit_presencia) {
//         return;
//     }

//     int posicion_frame_libre = get_posicion_frame_libre();

//     // Posibilidad de obtener un frame libre, sin tener que correr algoritmo de reemplazo
//     if (get_posicion_frame_libre) {
//         entrada -> nro_frame = posicion_frame_libre;
//         return;
//     }

//     // Si el bit de presencia está en 0, no está en memoria, correr algoritmo de reemplazo
//     if (algoritmo_reemplazo == CLOCK) {
//         log_info(logger, "Utilizando algoritmo CLOCK para reemplazar un frame");
//         posicion_frame_libre = algoritmo_clock();
//         entrada -> nro_frame = posicion_frame_libre;
//     }
//     else {
//         log_info(logger, "Utilizando algoritmo CLOCK MEJORADO para reemplazar un frame");
//         posicion_frame_libre = algoritmo_clock_mejorado();
//         entrada -> nro_frame = posicion_frame_libre;        
//     }
// }


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
    // Archivo swap    
    crear_archivo_swap(pcb -> PID, pcb -> tamanio);

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

    return tabla_primer_nivel -> id_tabla;
}