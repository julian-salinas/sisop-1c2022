#include "procesar_conexion.h"

void procesar_conexion(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket = args->fd;
    char* nombre_servidor = args->server_name;
    free(args);

    /*
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
    */

    int header = recibir_header(socket);
    t_PCB* pcb;
    t_buffer* payload;

    switch (header) {

        case NUEVO_PROCESO:
            pcb = socket_create_PCB(socket);
            agregar_a_new(pcb);  // Se agrega proceso a cola NEW y se actualiza su estado
            break;


        case PROCESO_FINALIZADO:
            sem_post(sem_cpu_disponible);
            pcb = socket_get_PCB(socket);
            enviar_pcb(conexion_memoria, PROCESO_FINALIZADO, pcb);  // Avisarle a memoria para que desaloje al proceso
            enviar_header(pcb -> socket_cliente, PROCESO_FINALIZADO);  // Avisarle a consola que terminó la ejecución
            break;


        case PROCESO_BLOQUEADO:
            sem_post(sem_cpu_disponible);
            pcb = socket_get_PCB(socket);

            if ((pcb -> tiempo_bloqueo) > (double) (kernel_config -> tiempo_maximo_bloqueado)) {
                pcb -> estado = SUSPENDED_BLOCKED;
                // ¿Mandarle una solicitud a memoria para que saque al proceso de memoria?
            }
            
            running_a_blocked(pcb);
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


t_PCB* crear_PCB(t_proceso* proceso, int socket) {
    
    t_PCB* pcb = malloc(sizeof(t_PCB));
    pcb -> PID = contador_id_proceso;
    pcb -> tamanio = proceso -> tamanio;
    pcb -> lista_instrucciones = proceso -> lista_instrucciones;
    pcb -> program_counter = 0;
    pcb -> tabla_paginas = -1;
    pcb -> estimacion_rafaga = kernel_config -> estimacion_inicial;
    pcb -> tiempo_ejecucion = 0;
    pcb -> socket_cliente = socket;
    pcb -> estado = NEW;
    pcb -> tiempo_bloqueo = -1; // vo deci?

    sem_wait(mutex_pid);
        contador_id_proceso++;
    sem_post(mutex_pid);
    
    return pcb;
}


t_PCB* socket_create_PCB(int socket) {
    t_buffer* payload = recibir_payload(socket);
    t_proceso* proceso = buffer_take_PROCESO(payload);
    t_PCB* pcb = crear_PCB(proceso, socket);
    destruir_buffer(payload);

    return pcb;
}