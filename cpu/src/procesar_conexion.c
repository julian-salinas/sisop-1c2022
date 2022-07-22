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

        case KERNEL: // Handshake inicial con Kernel
            log_info(logger, "Se conectó Kernel - header %d", header);
            procesar_conexion_kernel_cpu(socket_cliente);
            break;

        case -1:
            log_error(logger, "Cliente desconectado de %s...", nombre_servidor);
            return;

        default:
            log_error(logger, "Algo anduvo mal en el server de %s", nombre_servidor);
            log_info(logger, "Cop: %d", header);
            return;
    }
}


void procesar_conexion_kernel_cpu(int socket_cliente) {
    int8_t header;
    t_buffer* buffer;
    t_PCB* pcb;
    
    while (1) {
        header = recibir_header(socket_cliente);
        log_info(logger, "Conexion con KERNEL - Se recibió header %d", header);

        switch (header) {

            case EJECUTAR_PROCESO:
                log_info(logger, "CASE EJECUTAR PROCESO");
                log_info(logger, "Se recibió pcb del Kernel.");
                buffer = recibir_payload(socket_cliente);
                pcb = buffer_take_PCB(buffer);  
                destruir_buffer(buffer);
                log_info(logger, "Recibimos PCB con ID:%d y Tabla de páginas %d", pcb -> PID, pcb -> tabla_paginas);
                ejecutar_ciclo_instruccion(pcb, socket_cliente);
                break;

            case INTERRUPCION: 
                log_info(logger, "INTERRUPCION");
                sem_wait(mutex_interrupt);
                interrupcion=1;
                log_info(logger, "INTERRUPCION %d", interrupcion);
                sem_post(mutex_interrupt);
                break;               

            case -1:
                log_warning(logger, "Se desconectó Kernel");
                return;

            default:
                log_warning(logger, "Kernel nos mandó cualquier cosa: %d", header);
                return;
        
        }
    }
}