#ifndef PLANIFICADOR_KERNEL_H
#define PLANIFICADOR_KERNEL_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<commons/config.h>
    #include <commons/collections/list.h>
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
    t_list* lista_new;
    t_list* lista_ready;
    t_list* lista_exit;
    t_list* lista_blocked;
    t_list* lista_suspended_blocked;
    t_list* lista_suspended_ready;

    //Semáforos listas
    sem_t* mutex_lista_new;
    sem_t* mutex_lista_ready;
    sem_t* mutex_lista_exit;
    sem_t* mutex_lista_blocked;
    sem_t* mutex_lista_suspended_blocked;
    sem_t* mutex_lista_suspended_ready;


    typedef struct {
        t_proceso* proceso1;
        t_proceso* proceso2;
    } t_son_mismo_proceso_args;


#endif /* PLANIFICADOR_KERNEL_H */
