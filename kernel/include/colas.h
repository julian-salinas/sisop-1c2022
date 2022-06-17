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
    #include "semaforos.h"

    // Colas
    t_queue* cola_new;
    t_queue* cola_ready;
    t_queue* cola_blocked;
    t_queue* cola_suspended_ready;
    t_queue* cola_exit;

    // Semáforos colas
    sem_t* mutex_cola_new;
    sem_t* mutex_cola_ready;
    sem_t* mutex_cola_exit;
    sem_t* mutex_cola_blocked;
    sem_t* mutex_cola_suspended_ready;

    int proceso_buscado;

    /**
     * @DESC: Inicializar todos los semáforos mutex de las colas
     */ 
    void inicializar_mutex_colas(void);

    /**
     * @DESC: Inicializar todas las colas
     */ 
    void inicializar_colas(void);

    /* ========================================= Transiciones ========================================= */
    
    /**
     * @DESC: Recibe un proceso y lo pone en estado NEW y lo agrega a la cola new.
     *        * Hace un post del semáforo sem_procesos_esperando
     * @param proceso: pcb de un proceso a pasar a new
     */ 
    void agregar_a_new(t_PCB* proceso);


    /**
     * @DESC: Realiza la transición SUSPENDED-READY -> READY. Mueve al proceso de cola
     * @wait sem_procesos_esperando
     * @post sem_procesos_en_ready
     */ 
    void suspended_ready_a_ready(void);


    /**
     * @DESC: Realiza la transición NEW -> READY. Mueve al proceso de cola 
     * @wait sem_procesos_esperando
     * @post sem_procesos_en_ready
     */ 
    void new_a_ready(void);


    /**
     * @DESC: Realiza la transición RUNNING -> READY. Agrega el proceso a la cola READY
     * @post sem_procesos_en_ready
     */ 
    void running_a_ready(t_PCB* procesoAMover);


    /**
     * @DESC: Realiza la transición READY -> RUNNING. Extrae proceso de cola.
     *        * Envía PCB a cpu con el proceso a ejecutar
     */ 
    void ready_a_running(void);


    /**
     * @DESC: Realiza la transición RUNNING -> BLOCKED. Agrega el proceso a la cola BLOCKED
     * @post sem_procesos_bloqueados
     */ 
    void running_a_blocked(t_PCB* procesoAMover);

    
    /**
     * @DESC: Realiza la transición BLOCKED -> READY. Agrega un proceso a cola READY.
     * @param proceso: proceso a agregar en cola READY
     * @post procesos_en_ready
     */ 
    void blocked_a_ready(t_PCB* proceso);


    /**
     * @DESC: Realiza la transición SUSPENDED-BLOCKED -> SUSPENDED-READY. Agrega un proceso a cola SUSPENDED-READY.
     * @param proceso: proceso a agregar en cola SUSPENDED-READY
     * @post sem_procesos_esperando
     */ 
    void suspended_blocked_a_suspended_ready(t_PCB* proceso);

    /**
     * IMPLEMENTAR: Que la función cambie el estado de un proceso y avise a memoria para que lo desaloje
     */ 
    void blocked_a_suspended_blocked(t_PCB* procesoAMover);

    void blocked_a_exit(t_PCB* procesoAMover);

    void new_a_exit(t_PCB* procesoAMover);
            
    void running_a_exit(t_PCB* procesoAMover);

    void ready_a_exit(t_PCB* procesoAMover);

    void pasar_a_exit(t_queue* cola, sem_t* semaforo, t_PCB* proceso);    

    bool procesos_son_iguales(void* proceso);    
        
#endif