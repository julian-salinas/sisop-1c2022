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
        SJF
    } algoritmo_planificacion;

    algoritmo_planificacion algoritmo_elegido;

    int calcular_estimacion(t_PCB* pcb);

    void elegir_algoritmo(char* algoritmo);
    
    void inicializar_cola();
    
    void inicializar_semaforos_plani();
    
    void finalizar_semaforos_plani();
    
    void agregar_a_new(t_PCB* procesoAMover);
    
    void new_a_ready();
    
    void new_a_running();
    
    void running_a_ready(t_PCB* procesoAMover);
    
    void running_a_blocked(t_PCB* procesoAMover);
    
    void blocked_a_ready();
    
    void blocked_a_exit(t_PCB* procesoAMover);
    
    void new_a_exit(t_PCB* procesoAMover);
    
    void running_a_exit(t_PCB* procesoAMover);
    
    void ready_a_exit(t_PCB* procesoAMover);


    //Colas
    t_queue* cola_new;
    t_queue* cola_ready;
    t_queue* cola_exit;
    t_queue* cola_blocked;
    t_queue* cola_suspended_blocked;
    t_queue* cola_suspended_ready;

    //Semáforos colas
    sem_t* mutex_cola_new;
    sem_t* mutex_cola_ready;
    sem_t* mutex_cola_exit;
    sem_t* mutex_cola_blocked;
    sem_t* mutex_cola_suspended_blocked;
    sem_t* mutex_cola_suspended_ready;

    bool procesos_son_iguales(void* proceso);

    void pasar_a_exit(t_queue* cola, sem_t* semaforo, t_PCB* proceso);
    
    int proceso_buscado;
    sem_t* mutex_proceso_buscado;

    typedef struct {
        t_proceso* proceso1;
        t_proceso* proceso2;
    } t_son_mismo_proceso_args;

#endif /* PLANIFICADOR_KERNEL_H */
