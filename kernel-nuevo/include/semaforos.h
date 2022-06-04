#ifndef SEMAFOROS_H
#define SEMAFOROS_H

    #include <semaphore.h>
    
    #include "config.h"
    #include "kernel.h"

    // Semáforos mutex para variables - Arrancan en 1 todos
    sem_t* mutex_pid;

    // Semáforos mutex para planificadores - Arrancan en 1 todos
    sem_t* mutex_mediano_plazo;
    
    // Semáforos contadores para variables
    sem_t* sem_multiprogramacion;

    // Semáforos binarios? Para despertar procesos - Arrancan en 0 todos
    sem_t* sem_mediano_plazo;
    sem_t* sem_corto_plazo, *sem_mediano_plazo, *sem_largo_plazo;

    // Inicializar todos los semaforos
    void ini_semaforos(void);

#endif /* SEMAFOROS_H */