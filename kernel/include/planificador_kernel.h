#ifndef PLANIFICADOR_KERNEL_H
#define PLANIFICADOR_KERNEL_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<commons/config.h>
    #include <commons/collections/queue.h>
    #include <semaphore.h>

    #include "networking.h"
    #include "serializacion.h"
    #include "enum.h"
    #include "proceso.h"
    
    #include "kernel.h"

    typedef enum {
        FIFO,
        SRT
    } algoritmo_planificacion;

    algoritmo_planificacion algoritmo_elegido;

    int calcular_estimacion(t_PCB* pcb);
    void elegir_algoritmo(char* algoritmo);
    void iniciar_planificacion();
    void inicializar_semaforos_plani();
    void finalizar_semaforos_plani();
    void new_a_ready();
    void new_a_running();
    void running_a_ready(t_proceso* procesoAMover);
    void running_a_blocked(t_proceso* procesoAMover);
    void blocked_a_ready();
    void blocked_a_exit();
    void new_a_exit();
    void running_a_exit(t_proceso* procesoAMover);
    void ready_a_exit();

    //Colas
    t_queue* cola_new;
    t_queue* cola_ready;
    t_queue* cola_exit;
    t_queue* cola_blocked;

    //Semáforos colas
    sem_t* mutex_cola_new;
    sem_t* mutex_cola_ready;
    sem_t* mutex_cola_exit;
    sem_t* mutex_cola_blocked;


#endif /* PLANIFICADOR_KERNEL_H */
