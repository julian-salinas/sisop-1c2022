#include "procesar_conexion.h"


void procesar_conexion(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    free(args);

    int header;
    header = recibir_header(socket_cliente);

    switch (header) {

        case CONEXION_CPU_MEMORIA:;
            //enviar a CPU cantidad de entradas por tabla de páginas y tamaño de página;
            conexion_cpu = crear_socket_cliente(IP_MEMORIA, "8001");
            log_info(logger, "Socket cliente memoria-cpu creado.");
            enviar_config_a_cpu(conexion_cpu, logger, memoria_config->paginas_por_tabla, memoria_config->tamanio_pagina);	
            break;

        case SOL_TABLA_PAGINAS:
            //inicializar estructuras
            //modificar pcb agregando el valor de tabla de paginas
            //responder al kernel que salió todo OK - No sé si hace falta esto, pero es lo que espera el kernel
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
