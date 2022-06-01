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

    if(strcmp(algoritmo,"SRT") == 0) {
        algoritmo_elegido = SRT;
        log_info(logger, "Se eligio el algoritmo SRT");
    }
}

void iniciar_planificacion(){

    cola_new = queue_create();
    cola_ready = queue_create();
    cola_exit = queue_create();
    cola_blocked = queue_create();

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
    t_proceso* procesoAMover = queue_pop(cola_new);
    sem_post(mutex_cola_new);

    sem_wait(mutex_cola_ready);
    queue_push(cola_ready, procesoAMover);
    sem_post(mutex_cola_ready);

    //log_info(logger, "El proceso con Id:  pasó de NEW a READY.") //acá no tengo el PCB para obtener el id del proceso.
}

void new_a_running(){

    sem_wait(mutex_cola_new);
    queue_pop(cola_new);
    sem_post(mutex_cola_new);

    //log_info(logger, "El proceso con Id:  pasó de NEW a RUNNING.") //acá no tengo el PCB para obtener el id del proceso.
}

void running_a_ready(t_proceso* procesoAMover){

    sem_wait(mutex_cola_ready);
    queue_push(cola_new, procesoAMover);
    sem_post(mutex_cola_ready);

    //log_info(logger, "El proceso con Id:  pasó de RUNNING a READY.") //acá no tengo el PCB para obtener el id del proceso.
}

void running_a_blocked(t_proceso* procesoAMover){

    sem_wait(mutex_cola_blocked);
    queue_push(cola_blocked, procesoAMover);
    sem_post(mutex_cola_blocked);

    //log_info(logger, "El proceso con Id:  pasó de RUNNING a BLOCKED.") //acá no tengo el PCB para obtener el id del proceso.
}

void blocked_a_ready(){

    sem_wait(mutex_cola_blocked);
    t_proceso* procesoAMover = queue_pop(cola_blocked);
    sem_post(mutex_cola_blocked);

    sem_wait(mutex_cola_ready);
    queue_push(cola_ready, procesoAMover);
    sem_post(mutex_cola_ready);

    //log_info(logger, "El proceso con Id:  pasó de BLOCKED a READY.") //acá no tengo el PCB para obtener el id del proceso.
}

void blocked_a_exit(){

    sem_wait(mutex_cola_blocked);
    t_proceso* procesoAMover = queue_pop(cola_blocked);
    sem_post(mutex_cola_blocked);

    sem_wait(mutex_cola_exit);
    queue_push(cola_exit, procesoAMover);
    sem_post(mutex_cola_exit);

    //log_info(logger, "El proceso con Id:  pasó de BLOCKED a EXIT.") //acá no tengo el PCB para obtener el id del proceso.
}

void new_a_exit(){

    sem_wait(mutex_cola_new);
    t_proceso* procesoAMover = queue_pop(cola_new);
    sem_post(mutex_cola_new);

    sem_wait(mutex_cola_exit);
    queue_push(cola_exit, procesoAMover);
    sem_post(mutex_cola_exit);

    //log_info(logger, "El proceso con Id:  pasó de NEW a EXIT.") //acá no tengo el PCB para obtener el id del proceso.
}

void running_a_exit(t_proceso* procesoAMover){

    sem_wait(mutex_cola_exit);
    queue_push(cola_exit, procesoAMover);
    sem_post(mutex_cola_exit);

    //log_info(logger, "El proceso con Id:  pasó de RUNNING a EXIT.") //acá no tengo el PCB para obtener el id del proceso.
}

void ready_a_exit(){

    sem_wait(mutex_cola_ready);
    t_proceso* procesoAMover = queue_pop(cola_ready);
    sem_post(mutex_cola_ready);

    sem_wait(mutex_cola_exit);
    queue_push(cola_exit, procesoAMover);
    sem_post(mutex_cola_exit);

    //log_info(logger, "El proceso con Id:  pasó de READY a EXIT.") //acá no tengo el PCB para obtener el id del proceso.
}