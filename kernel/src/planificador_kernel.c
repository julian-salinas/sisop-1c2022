#include "planificador_kernel.h"
#include "kernel.h"

int calcular_estimacion(t_PCB* pcb){

    //inicializo config para el alfa
    int estimacion_actual = 0, tiempo_ejecucion = 0;
    float alfa = kernel_config -> alfa;

    estimacion_actual = pcb -> estimacion_rafaga;

    //falta que llegue la info del tiempo_ejecucion desde CPU

    estimacion_actual = alfa * tiempo_ejecucion + (1 - alfa) * estimacion_actual;  

    pcb ->  estimacion_rafaga = estimacion_actual;

    destruir_kernel_config(kernel_config);


    return estimacion_actual;
}

void elegir_algoritmo(char* algoritmo) {
    
    if(strcmp(algoritmo,"FIFO") == 0) {
        algoritmo_elegido = FIFO;
        log_info(logger, "Se eligio el algoritmo FIFO");
    }

    if((strcmp(algoritmo,"SRT") == 0) || (strcmp(algoritmo,"SJF") == 0)) {
        algoritmo_elegido = SJF;
        log_info(logger, "Se eligio el algoritmo SRT/SJF");
    }
}

void inicializar_colas(){

    cola_new = queue_create();
    cola_ready = queue_create();
    cola_exit = queue_create();
    cola_blocked = queue_create();
    cola_suspended_blocked = queue_create();
    cola_suspended_ready = queue_create();

    inicializar_semaforos_plani();
}

void inicializar_semaforos_plani(){

    mutex_cola_new = malloc(sizeof(sem_t));
    sem_init(mutex_cola_new, 0, 1);

    mutex_cola_ready = malloc(sizeof(sem_t));
    sem_init(mutex_cola_ready, 0, 1);

    mutex_cola_exit = malloc(sizeof(sem_t));
    sem_init(mutex_cola_exit, 0, 1);

    mutex_cola_blocked = malloc(sizeof(sem_t));
    sem_init(mutex_cola_blocked, 0, 1);
    
    mutex_cola_suspended_blocked = malloc(sizeof(sem_t));
    sem_init(mutex_cola_suspended_blocked, 0, 1);

    mutex_cola_suspended_ready = malloc(sizeof(sem_t));
    sem_init(mutex_cola_suspended_ready, 0, 1);
    
    mutex_proceso_buscado = malloc(sizeof(sem_t));
    sem_init(mutex_proceso_buscado, 0, 1);
}

void finalizar_semaforos_plani() {
    free(mutex_cola_new);
    free(mutex_cola_ready);
    free(mutex_cola_exit);
    free(mutex_cola_blocked);
}

/* LAS SIGUIENTES FUNCIONES MUEVEN LOS PROCESOS DE UNA COLA A OTRA. HABRÍA QUE MANEJAR ESTADOS Y OBTENER EL PROCESO Y EL PCB */

void new_a_ready(){

    sem_wait(mutex_cola_new);
        t_PCB* procesoAMover = queue_pop(cola_new);
    sem_post(mutex_cola_new);

    sem_wait(mutex_cola_ready);
        queue_push(cola_ready, procesoAMover);
    sem_post(mutex_cola_ready);

    log_info(logger, "El proceso con Id:%d pasó de NEW a READY.", procesoAMover -> PID);

}

void new_a_running(t_PCB* procesoAMover){

    sem_wait(mutex_cola_new);
        queue_pop(cola_new);
    sem_post(mutex_cola_new);

    log_info(logger, "El proceso con Id:%d  pasó de NEW a RUNNING.", procesoAMover -> PID);

}

void running_a_ready(t_PCB* procesoAMover){

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_ready);
        queue_push(cola_new, procesoAMover);
    sem_post(mutex_cola_ready);

    log_info(logger, "El proceso con Id:%d  pasó de RUNNING a READY.", procesoAMover -> PID);

}

void running_a_blocked(t_PCB* procesoAMover){

    sem_wait(mutex_cola_blocked);
        queue_push(cola_blocked, procesoAMover);
    sem_post(mutex_cola_blocked);

    log_info(logger, "El proceso con Id:%d  pasó de RUNNING a BLOCKED.", procesoAMover -> PID);

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

}


void blocked_a_exit(t_PCB* procesoAMover){

    pasar_a_exit(cola_blocked, mutex_cola_blocked, procesoAMover);

    log_info(logger, "El proceso con Id:%d  pasó de BLOCKED a EXIT.", procesoAMover -> PID);

}

void new_a_exit(t_PCB* procesoAMover){

    pasar_a_exit(cola_new, mutex_cola_new, procesoAMover);

    log_info(logger, "El proceso con Id:%d  pasó de NEW a EXIT.", procesoAMover -> PID);

}

void running_a_exit(t_PCB* procesoAMover) {

    procesoAMover -> estado = EXIT;

    sem_wait(mutex_cola_exit);
        queue_push(cola_exit, procesoAMover);
    sem_post(mutex_cola_exit);

    log_info(logger, "El proceso con Id:%d  pasó de RUNNING a EXIT.", procesoAMover -> PID);
    
}

void ready_a_exit(t_PCB* procesoAMover){

    pasar_a_exit(cola_ready, mutex_cola_ready, procesoAMover);

    log_info(logger, "El proceso con Id %d:  pasó de READY a EXIT.", procesoAMover -> PID);

}

void agregar_a_new(t_PCB* procesoAMover) {
    
    procesoAMover -> estado = NEW;

    sem_wait(mutex_cola_new);
        queue_push(cola_new, procesoAMover);
    sem_post(mutex_cola_new);

}

void suspended_ready_a_ready() {
    
    sem_wait(mutex_cola_ready);
        t_PCB* procesoAMover = queue_pop(cola_suspended_ready);
    sem_post(mutex_cola_ready);

    procesoAMover -> estado = READY;

    sem_wait(mutex_cola_suspended_ready);
        queue_push(cola_ready, procesoAMover);
    sem_post(mutex_cola_suspended_ready);
}

bool procesos_son_iguales(void* proceso) {
    return (((t_PCB*) proceso) -> PID == proceso_buscado);
}


pasar_a_exit(t_queue* cola, sem_t* semaforo, t_PCB* proceso) {

    wait(mutex_proceso_buscado);
        
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