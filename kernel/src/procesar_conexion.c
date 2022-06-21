#include "procesar_conexion.h"

void procesar_conexion(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket = args->fd;
    char* nombre_servidor = args->server_name;
    free(args);

    int header = recibir_header(socket);
    t_PCB* pcb;

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
            sem_post(sem_multiprogramacion); // Se libera multiprog. después de sacar al proceso de memoria
            
            break;

        case PROCESO_BLOQUEADO:
            sem_post(sem_cpu_disponible);

            pcb = socket_get_PCB(socket); // Obtener pcb del proceso bloqueado
            log_info(logger, "Se recibió proceso bloqueado por %d ms", pcb -> tiempo_bloqueo);
            
            ajustar_estimacion(pcb);

            running_a_blocked(pcb);  // Pasar a cola blocked

            // Iniciar hilo que se va a encargar de suspender al proceso en caso de que se zarpe de tiempo 
            pthread_create(&thread_suspension, 0, (void*) func_suspension, (void*) pcb);
            pthread_detach(thread_suspension);

            break;

        case -1:
            log_error(logger, "Cliente desconectado de %s", nombre_servidor);
            break;

        default:
            log_error(logger, "El codigo de operacion %d es incorrecto - %s", header, nombre_servidor);
            break;
    }
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
    pcb -> tiempo_bloqueo = -1;
    pcb -> estado = NEW;

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