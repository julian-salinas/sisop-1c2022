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

        case CONEXION_CPU_MEMORIA:
        {
            /* TODO
            / recibir paquete y guardar paginas por tabla y tamanio pagina */
            t_buffer* payload = recibir_payload(socket_cliente);
            paginas_por_tabla = buffer_take_UINT8(payload);
            tamanio_pagina = buffer_take_UINT8(payload);
            //funciona 
            log_info(logger, "Se recibió configuración de memoria.");
            printf("Páginas por tabla: %u\n",paginas_por_tabla);
            printf("Tamaño de página: %u\n",tamanio_pagina);
            break; 
        }
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

    return;
}

void procesar_conexion_kernel_cpu(int socket_cliente) {
    int8_t header;
    
    while (1) {
        header = recibir_header(socket_cliente);
        log_info(logger, "Conexion con KERNEL - Se recibió header %d", header);

        switch (header) {

        case EJECUTAR_PROCESO:
            log_info(logger, "Se recibió pcb del Kernel.");
            t_buffer* buffer = recibir_payload(socket_cliente);
            t_PCB* pcb = buffer_take_PCB(buffer);  
            ejecutar_ciclo_instruccion(pcb, socket_cliente);
            break;
        case INTERRUPCION: 
            //TO DO
            //Enviar OK a Kernel 
            sem_wait(mutex_interrupt);
            interrupcion=1;
            sem_post(mutex_interrupt);
            break;               
        }
    }
}