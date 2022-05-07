#include "utils.h"


void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    free(args);

    int header;
    header = recibir_header(socket_cliente);

    switch (header) {

        case INSTRUCCIONES:
            log_info(logger, "Se recibieron instrucciones");
            t_paquete* paquete = recibir_paquete(socket_cliente, header);

            t_proceso* proceso = buffer_take_PROCESO(paquete -> payload);                
            
            // Podés descomentar esto en caso de ser un desconfiado y querer apreciar algo de lo que llegó
            // t_instruccion* instruccion = list_get(proceso -> lista_instrucciones, 0);
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


t_PCB* crear_PCB(/*lo que me da la consola*/) {
    t_PCB* pcb = malloc(sizeof(t_PCB));
    //pcb -> PID = algo
    //pcb -> tamanio = algo
    //pcb -> lista_instrucciones = algo
    //pcb -> program_counter = algo
    //pcb -> tabla_paginas = algo
    //pcb -> estimacion_rafaga = algo
    return pcb;
}