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

        case CONEXION_CPU_MEMORIA:;
            //enviar a CPU cantidad de entradas por tabla de páginas y tamaño de página;
            conexion_cpu = crear_socket_cliente(IP_MEMORIA, "8001");
            log_info(logger, "Socket cliente memoria-cpu creado.");
            enviar_config_a_cpu(conexion_cpu, logger, memoria_config->paginas_por_tabla, memoria_config->tamanio_pagina);	
            break;

        case SOL_TABLA_PAGINAS:

            pcb = socket_get_PCB(socket_cliente);
            //calculo cuántas tablas necesito
            int cantidad_tablas_segundo_nivel = pcb -> tamanio / tamanio_tabla;
            int cantidad_tablas_segundo_nivel_maximas = memoria_config -> paginas_por_tabla;

            if (cantidad_tablas_segundo_nivel > cantidad_tablas_segundo_nivel_maximas){ //si el tamaño del proceso es mayor al máximo permitido
                enviar_pcb(socket_cliente, PROCESO_RECHAZADO, pcb);
                return;
            }

            //creo las tablas de primer nivel
            t_tabla_primer_nivel* tabla_primer_nivel = crear_tabla_primer_nivel();

            //creo las tablas de segundo nivel
            for (size_t i = 0; i < cantidad_tablas_segundo_nivel; i++)
            {
                t_tabla_segundo_nivel* tablas_segundo_nivel = crear_tabla_segundo_nivel();
                agregar_entrada_primer_nivel(tabla_primer_nivel, tablas_segundo_nivel -> id_tabla);
            }

            //modificar pcb agregando el valor de tabla de paginas
            pcb -> tabla_paginas = tabla_primer_nivel -> id_tabla;

            //devolver pcb al kernel
            enviar_pcb(socket_cliente, MEMORIA_OK, pcb);
            break;

        case PROCESO_SUSPENDIDO:
            //se libera su espacio en memoria
            //se escribe en swap la info necesaria
            break;

        case PROCESO_FINALIZADO:
            //se libera su espacio en memoria
            //se elimina su archivo de swap
            //se le avisa a consola o le avisa el kernel?
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
