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
            int conexion_cpu = crear_socket_cliente(IP_MEMORIA, "8013");
            log_info(logger, "Socket cliente memoria-cpu creado.");
            enviar_config_a_cpu(conexion_cpu);	
            break;

        case -1:
            log_error(logger, "Cliente desconectado de %s...", nombre_servidor);
            return;

        default:
            log_error(logger, "Algo anduvo mal en el server de %s", nombre_servidor);
            log_info(logger, "Cop: %d", header);
            return;
    }

    log_warning(logger, "El cliente se desconecto de server %s", nombre_servidor);
    return;
}


void enviar_config_a_cpu(int socket_cliente){
    /* 
        TODO: Adaptar esto a la nueva forma de memoria
    */
    return;
	// t_paquete* paquete = serializar_config_cpu_memoria(config_data.paginas_por_tabla, config_data.tam_pagina);
	// log_info(logger, "Serializo ok.");	
	// enviar_paquete(socket_cliente,paquete);
	// destruir_paquete(paquete);
	// log_info(logger, "Config enviado.");

}