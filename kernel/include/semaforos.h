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
    sem_t* mutex_mediano_plazo, *mutex_suspension;
    
    // Semáforos contadores para variables
    sem_t* sem_multiprogramacion;

    // Semáforo que indica que llegó un nuevo proceso (y si)
    sem_t* sem_nuevo_proceso;

    // Semáforos binarios? Para despertar procesos - Arrancan en 0 todos
    sem_t* sem_largo_plazo;

    // Semáforos mutex para sockets
    sem_t* mutex_socket_memoria, *mutex_socket_cpu_dispatch, *mutex_socket_cpu_interrupt;

    sem_t* sem_proceso_interrumpido;
    
    sem_t* mutex_proceso_corriendo;

    sem_t* mutex_log;
    
    // Inicializar todos los semaforos
    void ini_semaforos(void);

#endif /* SEMAFOROS_H */