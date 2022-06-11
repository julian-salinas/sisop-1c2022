#ifndef SEMAFOROS_H
#define SEMAFOROS_H

    #include <semaphore.h>
    
    #include "config.h"
    #include "kernel.h"

    // Semáforos mutex para variables - Arrancan en 1 todos
    sem_t* mutex_pid;

    // Indica que hay procesos esperando a ser planificados o procesos en estado ready
    sem_t* sem_procesos_esperando, *sem_procesos_en_ready, *sem_cpu_disponible, *sem_procesos_bloqueados;

    // Semáforos mutex para planificadores - Arrancan en 1 todos
    sem_t* mutex_mediano_plazo;
    
    // Semáforos contadores para variables
    sem_t* sem_multiprogramacion;

    // Semáforo que indica que llegó un nuevo proceso (y si)
    sem_t* sem_nuevo_proceso;

    // Semáforos binarios? Para despertar procesos - Arrancan en 0 todos
    sem_t* sem_mediano_plazo;
    sem_t* sem_corto_plazo, *sem_mediano_plazo, *sem_largo_plazo;
    sem_t* sem_io;

    // Semáforos de transiciones
    sem_t* mutex_transicion_new_a_ready;
    sem_t* mutex_transicion_ready_a_running;
    sem_t* mutex_transicion_running_a_ready;
    sem_t* mutex_transicion_running_a_blocked;
    sem_t* mutex_transicion_running_a_exit;
    sem_t* mutex_transicion_blocked_a_suspended_blocked;

    // Semáforo que protege variable booleana que indica si hay un proceso en CPU o no
    sem_t* mutex_proceso_buscado;
    sem_t* mutex_proceso_corriendo;

    // Inicializar todos los semaforos
    void ini_semaforos(void);

#endif /* SEMAFOROS_H */