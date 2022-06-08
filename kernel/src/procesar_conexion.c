#include "procesar_conexion.h"

void procesar_conexion(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    t_buffer* payload;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    free(args);

    transicion_new_a_ready = false;
    transicion_running_a_exit = false;
    transicion_running_a_blocked = false;
    transicion_block_a_suspended_block = false;
    transicion_suspended_block_a_suspended_ready = false;
    transicion_suspended_ready_a_ready = false;
    transicion_running_a_ready = false;
    transicion_ready_a_running = false;
    transicion_blocked_a_ready = false;
    transicion_blocked_a_exit = false;
    transicion_new_a_exit = false;
    transicion_ready_a_exit = false;
    transicion_blocked_a_suspended_blocked = false;

    int header;
    header = recibir_header(socket_cliente);

    switch (header) {

        case NUEVO_PROCESO:
            /** Consola envía un proceso que acaba de crearse
             *  - Pasar proceso de NEW a READY en caso de que el grado de multiprogramación lo permita
             */ 

            log_info(logger, "Se recibio un proceso");

            // Recibir el paquete del cliente y crear PCB del mismo
            payload = recibir_payload(socket_cliente);
            t_proceso* proceso = buffer_take_PROCESO(payload);                
            t_PCB* pcb = crear_PCB(proceso, socket_cliente);
            
            sem_wait(mutex_mediano_plazo);
                agregar_a_new(pcb);  // Se agrega proceso a cola NEW y se actualiza su estado

                /* 
                // ¿y esto que hacía acá?
                enviar_pcb(conexion_cpu, pcb);
                log_info(logger, "la pcb se mando es tu culpa cpu");
                */

                sem_post(sem_nuevo_proceso);
                log_info(logger, "Proceso PID:%d se ahora en estado NEW", pcb -> PID);

            sem_post(mutex_mediano_plazo);
            destruir_buffer(payload);
            break;

        case PROCESO_FINALIZADO:  // mensaje de CPU
            
            transicion_running_a_exit = true;

            // Recibir pcb de CPU
            // Guardar pcb en variable global
            payload = recibir_payload(conexion_cpu);
            proceso_desalojado = buffer_take_PCB(payload);

            // sem post a planificador largo plazo
            sem_post(sem_largo_plazo);
            
            break;

        case PROCESO_BLOQUEADO: // mensaje de cpu
            
            transicion_running_a_blocked = true;

            // Recibir pcb de CPU
            // Guardar pcb en variable global
            payload = recibir_payload(conexion_cpu);
            proceso_desalojado = buffer_take_PCB(payload);

            // sem post a planificador corto plazo
            sem_post(sem_corto_plazo);
            
            break;

        case -1:
            log_error(logger, "Cliente desconectado de %s", nombre_servidor);
            break;

        default:
            log_error(logger, "El codigo de operacion %d es incorrecto - %s", header, nombre_servidor);
            break;
    }

    // Finalizar atender un cliente
    log_warning(logger, "El cliente se desconecto de server %s", nombre_servidor);

}


t_PCB* crear_PCB(t_proceso* proceso, int fd) {
    
    t_PCB* pcb = malloc(sizeof(t_PCB));
    pcb -> PID = contador_id_proceso;
    pcb -> tamanio = proceso -> tamanio;
    pcb -> lista_instrucciones = proceso -> lista_instrucciones;
    pcb -> program_counter = 0;
    pcb -> tabla_paginas = -1;
    pcb -> estimacion_rafaga = kernel_config -> estimacion_inicial;
    pcb -> tiempo_ejecucion = 0;
    pcb -> socket_cliente = fd;
    pcb -> estado = NEW;

    sem_wait(mutex_pid);
        contador_id_proceso++;
    sem_post(mutex_pid);
    
    return pcb;

}
