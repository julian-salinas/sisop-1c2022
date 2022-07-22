#include "colas.h"


void inicializar_mutex_colas(void) {
    mutex_cola_new = malloc(sizeof(sem_t));
    sem_init(mutex_cola_new, 0, 1);

    mutex_cola_ready = malloc(sizeof(sem_t));
    sem_init(mutex_cola_ready, 0, 1);

    mutex_cola_suspended_ready = malloc(sizeof(sem_t));
    sem_init(mutex_cola_suspended_ready, 0, 1);

    mutex_cola_blocked = malloc(sizeof(sem_t));
    sem_init(mutex_cola_blocked, 0, 1);

}


void inicializar_colas(void) {
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_blocked = queue_create();
    cola_suspended_ready = queue_create();
}


void agregar_a_new(t_PCB* procesoAMover) {
    sem_wait(mutex_cola_new);
        queue_push(cola_new, (void*) procesoAMover);
    sem_post(mutex_cola_new);

    log_info(logger, "El proceso con ID:%d pasó a estado NEW", procesoAMover -> PID);

    sem_post(sem_procesos_esperando);
}


void suspended_ready_a_ready(void) {
    
    sem_wait(mutex_cola_suspended_ready);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_suspended_ready);
    sem_post(mutex_cola_suspended_ready);

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    if (algoritmo_elegido == SJF) {
        // No es necesario verificar que la cola ready tenga procesos, acabamos de agregarle uno
        ordenar_cola_ready();
    }

    log_info(logger, "El proceso con ID:%d pasó de SUSPENDED-READY a READY.", procesoAMover -> PID);
    
    sem_post(sem_procesos_en_ready);
}


void new_a_ready(void) {

    sem_wait(mutex_cola_new);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_new);
    sem_post(mutex_cola_new);

    procesoAMover -> estado = READY;

    // Solicitar tabla de páginas a memoria
    log_info(logger, "Pidiendo memoria para el proceso con ID %d: ", procesoAMover -> PID);
    sem_wait(mutex_socket_memoria);

        enviar_pcb(conexion_memoria, NUEVO_PROCESO, procesoAMover);
        uint8_t resp_memoria = recibir_header(conexion_memoria);
    
        if (resp_memoria == PROCESO_RECHAZADO) {
            log_warning(logger, "EL proceso %d fue rechazado por memoria por ser demasiado grande.", procesoAMover -> PID);
            close(procesoAMover -> socket_cliente);
            return;
        }

        t_buffer* payload = recibir_payload(conexion_memoria);
        uint32_t tabla_paginas = buffer_take_INT32(payload);
        destruir_buffer(payload);
        tabla_paginas = procesoAMover -> PID;
        procesoAMover -> tabla_paginas = tabla_paginas;

    sem_post(mutex_socket_memoria);
    
    log_info(logger, "Se recibió el proceso %d con la tabla de páginas %d", procesoAMover -> PID, procesoAMover -> tabla_paginas);

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    log_info(logger, "El proceso con ID:%d pasó de NEW a READY.", procesoAMover -> PID);

    sem_post(sem_procesos_en_ready);
}


void ready_a_running(void) {

    sem_wait(mutex_cola_ready);
    
    if (proceso_corriendo || queue_is_empty(cola_ready)) {
        sem_post(mutex_cola_ready);
        return;
    }
    sem_post(mutex_cola_ready);

    sem_wait(mutex_cola_ready);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_ready);
    sem_post(mutex_cola_ready);

    procesoAMover -> estado = RUNNING;
    procesoAMover -> tiempo_restante = 0;
    
    sem_wait(mutex_socket_cpu_dispatch);
        enviar_pcb(conexion_cpu_dispatch, EJECUTAR_PROCESO, procesoAMover); // Pasarle el proceso a CPU para que lo ejecute
    sem_post(mutex_socket_cpu_dispatch);

    log_info(logger, "El proceso con ID:%d pasó de READY a RUNNING", procesoAMover -> PID);
    destruir_PCB(procesoAMover);

    sem_wait(mutex_proceso_corriendo);
    proceso_corriendo = true;
    sem_post(mutex_proceso_corriendo);
}


void running_a_ready(t_PCB* procesoAMover) {

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    log_info(logger, "El proceso con ID:%d pasó de RUNNING a READY.", procesoAMover -> PID);

    sem_post(sem_procesos_en_ready);
}


void running_a_blocked(t_PCB* procesoAMover){

    procesoAMover -> estado = BLOCKED;

    sem_wait(mutex_cola_blocked);
        queue_push(cola_blocked, (void*) procesoAMover);
    sem_post(mutex_cola_blocked);

    sem_post(sem_procesos_bloqueados);

    log_info(logger, "El proceso con ID:%d  pasó de RUNNING a BLOCKED.", procesoAMover -> PID);
}


void blocked_a_ready(t_PCB* procesoAMover){

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    log_info(logger, "El proceso con ID:%d  pasó de BLOCKED a READY.", procesoAMover -> PID);

    sem_post(sem_procesos_en_ready);
}


void blocked_a_suspended_blocked(t_PCB* procesoAMover){
    procesoAMover -> estado = SUSPENDED_BLOCKED;

    log_info(logger, "El proceso con ID:%d pasó de BLOCKED a SUSPENDED-BLOCKED", procesoAMover -> PID);

    sem_wait(mutex_socket_memoria);
        enviar_pcb(conexion_memoria, PROCESO_SUSPENDIDO, procesoAMover);
    sem_post(mutex_socket_memoria);

    sem_post(sem_multiprogramacion);
}


void suspended_blocked_a_suspended_ready(t_PCB* procesoAMover) {

    procesoAMover -> estado = SUSPENDED_READY;

    sem_wait(mutex_cola_suspended_ready);
        queue_push(cola_suspended_ready, (void*) procesoAMover);
    sem_post(mutex_cola_suspended_ready);

    log_info(logger, "El proceso con ID:%d  pasó de SUSPENDED-BLOCKED a SUSPENDED-READY.", procesoAMover -> PID);

    sem_post(sem_procesos_esperando);
}