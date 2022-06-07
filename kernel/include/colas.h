#ifndef COLAS_H
#define COLAS_H
    
    /* ======================================================================
    Este módulo se dedica a modelar las colas y transiciones entre las mismas
    ========================================================================*/

    #include <stdlib.h>
    #include <commons/log.h>
    #include <commons/collections/queue.h>
    #include <semaphore.h>

    #include "networking.h"

    #include "kernel.h"
    #include "proceso.h"


    //Colas
    t_queue* cola_new;
    t_queue* cola_ready;
    t_queue* cola_exit;
    t_queue* cola_blocked;
    t_queue* cola_suspended_ready;

    //Semáforos colas
    sem_t* mutex_cola_new;
    sem_t* mutex_cola_ready;
    sem_t* mutex_cola_exit;
    sem_t* mutex_cola_blocked;
    sem_t* mutex_cola_suspended_ready;

    sem_t* mutex_proceso_corriendo;

    int proceso_buscado;
    sem_t* mutex_proceso_buscado;

    // Inicializa todos los semáforos de colas
    void inicializar_semaforos_plani();

    // Inicializa todas las colas
    void inicializar_colas();

    // Devuelve true si la cola está vacía (y si)
    bool cola_esta_vacia(t_queue* cola);

    // Transiciones entre colas
    void agregar_a_new(t_PCB* proceso);

    void suspended_ready_a_ready(void);

    void new_a_ready(void);

    void running_a_ready(t_PCB* procesoAMover);

    t_PCB* ready_a_running(void);

    void running_a_blocked(t_PCB* procesoAMover);

    void blocked_a_ready();

    void blocked_a_exit(t_PCB* procesoAMover);

    void new_a_exit(t_PCB* procesoAMover);
            
    void running_a_exit(t_PCB* procesoAMover);

    void ready_a_exit(t_PCB* procesoAMover);

    void pasar_a_exit(t_queue* cola, sem_t* semaforo, t_PCB* proceso);    

    bool procesos_son_iguales(void* proceso);    
        
#endif