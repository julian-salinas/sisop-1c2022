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


bool cola_esta_vacia(t_queue* cola) {
    return queue_is_empty(cola);
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

    // Re-insertar proceso en memoria
    // Solicitar tabla de páginas a memoria
    enviar_pcb(conexion_memoria, SOL_TABLA_PAGINAS, procesoAMover);

    uint8_t resp_memoria = recibir_header(conexion_memoria);

    if (resp_memoria != MEMORIA_OK) {
        close(procesoAMover -> socket_cliente);
        return;
    }

    procesoAMover = socket_get_PCB(conexion_memoria);

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    // transicion_suspended_ready_a_ready = false;

    sem_wait(sem_procesos_esperando);
    sem_post(sem_procesos_en_ready);

    log_info(logger, "El proceso con ID:%d pasó de SUSPENDED-READY a READY.", procesoAMover -> PID);
}


void new_a_ready(void) {

    sem_wait(mutex_cola_new);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_new);
    sem_post(mutex_cola_new);

    procesoAMover -> estado = READY;

    // Solicitar tabla de páginas a memoria
    enviar_pcb(conexion_memoria, SOL_TABLA_PAGINAS, procesoAMover);

    uint8_t resp_memoria = recibir_header(conexion_memoria);

    if (resp_memoria != MEMORIA_OK) {
        close(procesoAMover -> socket_cliente);
        return;
    }

    procesoAMover = socket_get_PCB(conexion_memoria);

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    // sem_wait(mutex_transicion_new_a_ready);
    //     transicion_new_a_ready = false;
    // sem_post(mutex_transicion_new_a_ready);

    log_info(logger, "El proceso con ID:%d pasó de NEW a READY.", procesoAMover -> PID);

    sem_wait(sem_procesos_esperando);
    sem_post(sem_procesos_en_ready);
}


void ready_a_running(void) {
    sem_wait(mutex_cola_ready);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_ready);
    sem_post(mutex_cola_ready);

    procesoAMover -> estado = RUNNING;

    // sem_wait(mutex_proceso_corriendo);
    //     proceso_corriendo = true;
    // sem_post(mutex_proceso_corriendo);

    // sem_wait(mutex_transicion_ready_a_running);
    //     transicion_ready_a_running = false;
    // sem_post(mutex_transicion_ready_a_running);

    enviar_pcb(conexion_cpu_dispatch, PCB, procesoAMover); // Pasarle el proceso a CPU para que lo ejecute
    
    log_info(logger, "El proceso con ID:%d pasó de READY a RUNNING", procesoAMover -> PID);
}


void running_a_ready(t_PCB* procesoAMover) {

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_new, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    // sem_wait(mutex_proceso_corriendo);
    //     proceso_corriendo = false;
    // sem_post(mutex_proceso_corriendo);

    // sem_wait(mutex_transicion_running_a_ready);
    //     transicion_running_a_ready = false;
    // sem_post(mutex_transicion_running_a_ready);

    log_info(logger, "El proceso con Id:%d pasó de RUNNING a READY.", procesoAMover -> PID);

    sem_post(sem_procesos_en_ready);

}


void running_a_blocked(t_PCB* procesoAMover){

    sem_wait(mutex_cola_blocked);
        queue_push(cola_blocked, (void*) procesoAMover);
    sem_post(mutex_cola_blocked);

    sem_post(sem_procesos_bloqueados);

    log_info(logger, "El proceso con ID:%d  pasó de RUNNING a BLOCKED.", procesoAMover -> PID);

}


void blocked_a_ready(){

    sem_wait(mutex_cola_blocked);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_blocked);
    sem_post(mutex_cola_blocked);

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, (void*) procesoAMover);
    sem_post(mutex_cola_ready);

    log_info(logger, "El proceso con Id:%d  pasó de BLOCKED a READY.", procesoAMover -> PID);

    // transicion_blocked_a_ready = false;

    sem_post(sem_procesos_en_ready);
}


void blocked_a_exit(t_PCB* procesoAMover){

    pasar_a_exit(cola_blocked, mutex_cola_blocked, procesoAMover);

    log_info(logger, "El proceso con Id:%d  pasó de BLOCKED a EXIT.", procesoAMover -> PID);

    transicion_blocked_a_exit = false;

    //avisar a memoria que desaloje el proceso
}


void new_a_exit(t_PCB* procesoAMover){

    pasar_a_exit(cola_new, mutex_cola_new, procesoAMover);

    log_info(logger, "El proceso con Id:%d  pasó de NEW a EXIT.", procesoAMover -> PID);

    transicion_new_a_exit = false;

    //avisar a memoria que desaloje el proceso
}


void running_a_exit(t_PCB* procesoAMover) {

    procesoAMover -> estado = EXIT;

    sem_wait(mutex_cola_exit);
        queue_push(cola_exit, (void*) procesoAMover);
    sem_post(mutex_cola_exit);

    log_info(logger, "El proceso con ID:%d  pasó de RUNNING a EXIT.", procesoAMover -> PID);

    sem_wait(mutex_transicion_running_a_exit);
        transicion_running_a_exit = false;
    sem_post(mutex_transicion_running_a_exit);

    // Avisar a memoria que desaloje el proceso
    sem_post(sem_multiprogramacion);
}


void ready_a_exit(t_PCB* procesoAMover){

    pasar_a_exit(cola_ready, mutex_cola_ready, procesoAMover);

    log_info(logger, "El proceso con Id %d:  pasó de READY a EXIT.", procesoAMover -> PID);

    // transicion_ready_a_exit = false;

    //avisar a memoria que desaloje el proceso
}


void blocked_a_suspended_blocked(t_PCB* procesoAMover){
    procesoAMover -> estado = SUSPENDED_BLOCKED;

    sem_wait(mutex_transicion_blocked_a_suspended_blocked);
        transicion_blocked_a_suspended_blocked = false;
    sem_post(mutex_transicion_blocked_a_suspended_blocked);

    log_info(logger, "El proceso con ID:%d pasó de BLOCKED a SUSPENDED-BLOCKED", procesoAMover -> PID);

}

void blocked_a_suspended_ready(void) {

    sem_wait(mutex_cola_blocked);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_blocked);
    sem_post(mutex_cola_blocked);

    procesoAMover -> estado = SUSPENDED_READY;

    sem_wait(mutex_cola_suspended_ready);
        queue_push(cola_suspended_ready, (void*) procesoAMover);
    sem_post(mutex_cola_suspended_ready);

    log_info(logger, "El proceso con Id:%d  pasó de BLOCKED a SUSPENDED-READY.", procesoAMover -> PID);

    // transicion_blocked_a_ready = false;

    sem_post(sem_procesos_esperando);
}


void pasar_a_exit(t_queue* cola, sem_t* semaforo, t_PCB* proceso) {

    sem_wait(mutex_proceso_buscado);
        
        proceso_buscado = proceso -> PID;

        sem_wait(semaforo);
            cola -> elements = list_remove_by_condition(cola -> elements, *(procesos_son_iguales));
        sem_post(semaforo);

    sem_post(mutex_proceso_buscado);

    proceso -> estado = EXIT;

    sem_wait(mutex_cola_exit);
        queue_push(cola_exit, proceso);
    sem_post(mutex_cola_exit);
    
}

bool procesos_son_iguales(void* proceso) {
    return (((t_PCB*) proceso) -> PID == proceso_buscado);
}
