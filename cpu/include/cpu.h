#ifndef CPU_H_
#define CPU_H_

    #include<stdio.h>
    #include<stdlib.h>
    #include<pthread.h>
    #include <semaphore.h>

    // includes de las commons
    #include<commons/string.h>
    #include<commons/config.h>

    // includes del módulo
    #include "procesar_conexion.h"
    #include "cpu_config.h"
    #include "ciclo_instruccion.h"
    // includes de shared
    #include "networking.h"
    #include "serializacion.h"
    #include "config.h"
    #include "proceso.h"
    #include "utils.h"
    #include "tlb.h"

    // Semáforo mutex para variable interrupt
    sem_t* mutex_interrupt;
    // Variables para los 2 hilos de CPU
    pthread_t thread_dispatch, thread_interrupt;
    t_cpu_config* cpu_config;
    int conexion_memoria, server_cpu_dispatch, server_cpu_interrupt;
    //conexion_kernel
    t_paquete* paquete;
	t_config* config;
    t_log* logger;
 
    uint8_t paginas_por_tabla;
    uint8_t tamanio_pagina;

    int interrupcion, finCicloInstruccion; //0 el kernel no envio interrupcion, 1 interrumpir ejecución

    t_tlb* tlb;

    //ini
    void ini_semaforos(void);
    void ini_conexiones(void);
    void* atender_dispatch(void *arg);
    void* atender_interrupt(void *arg);
    void ini_threads(void);
    void obtener_config_memoria(void);
    //lib
    void liberar_conexiones(void);
    void finalizar_semaforos_cpu(void);


#endif /* CPU_H_ */
