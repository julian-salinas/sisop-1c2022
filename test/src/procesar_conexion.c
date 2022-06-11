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

        case PROCESO_BLOQUEADO:
            log_info(logger, "Se recibió PCB bloqueada");
            break;
        case PROCESO_FINALIZADO:
            log_info(logger, "Se recibió PCB estado EXIT");
            break;
        case INTERRUPCION:
            log_info(logger, "Se recibió pcb de CPU por interrupción.");
            conexion_cpu_dispatch = crear_socket_cliente("127.0.0.1","8001");
    t_instruccion* instruccion;
    t_lista_instrucciones* lista_instrucciones = crear_lista_instrucciones();
    instruccion = crear_instruccion(EXIT);
    agregar_instruccion_a_lista(lista_instrucciones, instruccion);

	 t_proceso* proceso = crear_proceso(8, lista_instrucciones);
     t_PCB* pcb = crear_PCB(proceso, conexion_cpu_dispatch);
    enviar_pcb(conexion_cpu_dispatch, PCB, pcb);
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

