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

        case INSTRUCCIONES:
            log_info(logger, "Se recibieron instrucciones");
            t_buffer* payload = recibir_payload(socket_cliente);

            t_proceso* proceso = buffer_take_PROCESO(payload);                
            
            t_PCB* pcb = crear_PCB(proceso, socket_cliente);
            enviar_pcb(conexion_cpu,pcb); 
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
}


t_PCB* crear_PCB(t_proceso* proceso, int fd) {
    t_PCB* pcb = malloc(sizeof(t_PCB));
    pcb -> PID = contador_id_proceso;
    pcb -> tamanio = proceso -> tamanio;
    pcb -> lista_instrucciones = proceso -> lista_instrucciones;
    pcb -> program_counter = 0;
    pcb -> tabla_paginas = -1; //recien cuando esté en READY
    pcb -> estimacion_rafaga = estimacion_rafaga_inicial;
    pcb -> tiempo_ejecucion = 0;
    pcb -> socket_cliente = fd;
    pcb -> estado = NEW;

    pthread_mutex_lock(&mutex_contador_id_proceso);
    contador_id_proceso++;
    pthread_mutex_unlock(&mutex_contador_id_proceso);
    
    return pcb;

}
