#ifndef PLANIFICADORES_H
#define PLANIFICADORES_H

    #include <pthread.h>
    #include <commons/log.h>
    
    #include "kernel.h"
    #include "colas.h"
    #include "semaforos.h"
    #include "enum.h"

    typedef enum {
        FIFO,
        SJF
    } algoritmo_planificacion;

    /* Valor que se va a definir al inicio del programa y no cambia durante su ejecución */
    algoritmo_planificacion algoritmo_elegido;

    bool proceso_corriendo;

    /* Threads de planificadores */
    pthread_t thread_corto_plazo, thread_mediano_plazo, thread_largo_plazo;

    /* Threads de I/O */
    pthread_t thread_io;

    /* Flags para transiciones */
    bool transicion_new_a_ready, transicion_running_a_exit, transicion_running_a_blocked, transicion_ready_a_running;

    /* Calcula estimacion de un proceso */
    int calcular_estimacion(t_PCB* pcb);
    
    /* Mapea algoritmo que llegó por config (SJF/SRT o FIFO) */
    void elegir_algoritmo(char* algoritmo);

    /* Función que inicializa todos los threads de kernel */
    void ini_threads(void);

    /* Funciones que van a ocupar los hilos de los planificadores */
    void* func_corto_plazo(void* args);
    
    void* func_mediano_plazo(void* args);
    
    void* func_largo_plazo(void* args);

    void* func_io(void* args);

    void ordenar_cola_ready(void);

    bool algoritmo_SJF(void* proceso1, void* proceso2);

#endif /* PLANIFICADORES_H */