#include "utils.h"

void iterator(char* value) {
	log_info(logger,"%s", value);
}


void* procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    
    // free(args);

    codigo_operacion op_code;

    while (socket_cliente != -1) {
        t_paquete* paquete;
        op_code = recibir_header(socket_cliente);

        if (!op_code) {
            return;
        }

        switch (op_code) {
            case PAQUETE:
                log_info(logger, "RECIBIMOS CODIGO PAQUETE");
                paquete = recibir_paquete(socket_cliente, op_code);

                // Acá se lee el valor q mandó consola, esto es un ejemplo
                // char* valor_prueba = buffer_take_STRING(paquete -> payload);
                break;
            
            case INSTRUCCIONES:
                log_info(logger, "Se recibieron instrucciones");
                paquete = recibir_paquete(socket_cliente, op_code);
                break;

            case -1:
                log_error(logger, "Cliente desconectado de %s...", nombre_servidor);
                return;

            default:
                log_error(logger, "Algo anduvo mal en el server de %s", op_code);
                log_info(logger, "Cop: %d", op_code);
                return;
        }
    }
    log_warning(logger, "El cliente se desconecto de %s server", nombre_servidor);
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