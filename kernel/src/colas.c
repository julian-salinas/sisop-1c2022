#include "colas.h"

void inicializar_semaforos_plani(void) {
    mutex_cola_new = malloc(sizeof(sem_t));
    sem_init(mutex_cola_new, 0, 1);

    mutex_cola_ready = malloc(sizeof(sem_t));
    sem_init(mutex_cola_ready, 0, 1);

    mutex_cola_suspended_ready = malloc(sizeof(sem_t));
    sem_init(mutex_cola_suspended_ready, 0, 1);

    mutex_proceso_corriendo = malloc(sizeof(sem_t));
    sem_init(mutex_proceso_corriendo, 0, 1);
}

void inicializar_colas(void) {
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_blocked = queue_create();
    cola_exit = queue_create();
    cola_suspended_ready = queue_create();
}

bool cola_esta_vacia(t_queue* cola) {
    return queue_is_empty(cola);
}

void agregar_a_new(t_PCB* procesoAMover) {
    sem_wait(mutex_cola_new);
        queue_push(cola_new, procesoAMover);
    sem_post(mutex_cola_new);
}

void new_a_ready(void) {

    sem_wait(mutex_cola_new);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_new);
    sem_post(mutex_cola_new);

    procesoAMover -> estado = READY;

    // Solicitar tabla de páginas a memoria
    //enviar_pcb(conexion_memoria, procesoAMover);
    
    // ¿Habría que destruir procesoAMover en este momento?
    //no se sabe
    //uint8_t resp_memoria = recibir_header(conexion_memoria);

    // if (resp_memoria != MEMORIA_OK) {
    //     close(procesoAMover -> socket_cliente);

    //     // Modificar: No se puede hacer un continue si no hay un loop
    //     return;
    // }

    // t_buffer* payload = recibir_payload(conexion_memoria);
    // procesoAMover = buffer_take_PCB(payload);

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, procesoAMover);
    sem_post(mutex_cola_ready);

    transicion_new_a_ready = false;

    log_info(logger, "El proceso con Id:%d pasó de NEW a READY.", procesoAMover -> PID);
}


void suspended_ready_a_ready(void) {
    
    sem_wait(mutex_cola_suspended_ready);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_suspended_ready);
    sem_post(mutex_cola_suspended_ready);

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, procesoAMover);
    sem_post(mutex_cola_ready);

    transicion_suspended_ready_a_ready = false;

    log_info(logger, "El proceso con Id:%d pasó de SUSPENDED-READY a READY.", procesoAMover -> PID);
}

void running_a_ready(t_PCB* procesoAMover) {

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_new, procesoAMover);
    sem_post(mutex_cola_ready);

    sem_wait(mutex_proceso_corriendo);
        proceso_corriendo = false;
    sem_post(mutex_proceso_corriendo);

    transicion_running_a_ready = false;

    log_info(logger, "El proceso con Id:%d pasó de RUNNING a READY.", procesoAMover -> PID);
}

t_PCB* ready_a_running(void) {
    sem_wait(mutex_cola_ready);
        t_PCB* procesoAMover = (t_PCB*) queue_pop(cola_ready);
    sem_post(mutex_cola_ready);

    procesoAMover -> estado = RUNNING;

    sem_wait(mutex_proceso_corriendo);
        proceso_corriendo = true;
    sem_post(mutex_proceso_corriendo);

    log_info(logger, "El proceso con ID:%d pasó de READY a RUNNING", procesoAMover -> PID);

    sem_post(sem_multiprogramacion);

    transicion_ready_a_running = false;

    return procesoAMover;
}

void running_a_blocked(t_PCB* procesoAMover){

    sem_wait(mutex_cola_blocked);
        queue_push(cola_blocked, procesoAMover);
    sem_post(mutex_cola_blocked);

    log_info(logger, "El proceso con Id:%d  pasó de RUNNING a BLOCKED.", procesoAMover -> PID);

    transicion_running_a_blocked = false;
}

void blocked_a_ready(){

    sem_wait(mutex_cola_blocked);
        t_PCB* procesoAMover = queue_pop(cola_blocked);
    sem_post(mutex_cola_blocked);

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, procesoAMover);
    sem_post(mutex_cola_ready);

    log_info(logger, "El proceso con Id:%d  pasó de BLOCKED a READY.", procesoAMover -> PID);

    transicion_blocked_a_ready = false;
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
        queue_push(cola_exit, procesoAMover);
    sem_post(mutex_cola_exit);

    log_info(logger, "El proceso con Id:%d  pasó de RUNNING a EXIT.", procesoAMover -> PID);

    transicion_running_a_exit = false;

    //avisar a memoria que desaloje el proceso
}

void ready_a_exit(t_PCB* procesoAMover){

    pasar_a_exit(cola_ready, mutex_cola_ready, procesoAMover);

    log_info(logger, "El proceso con Id %d:  pasó de READY a EXIT.", procesoAMover -> PID);

    transicion_ready_a_exit = false;

    //avisar a memoria que desaloje el proceso
}

void blocked_a_suspended_blocked(t_PCB* procesoAMover){
    procesoAMover -> estado = SUSPENDED_BLOCKED;

    transacion_blocked_a_suspended_blocked = false;
}

pasar_a_exit(t_queue* cola, sem_t* semaforo, t_PCB* proceso) {

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
