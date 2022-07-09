#ifndef MEMORIA_H_
#define MEMORIA_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>
    #include <unistd.h>
    #include <semaphore.h>
    #include <commons/collections/dictionary.h>
    #include <commons/collections/list.h>

    #include "networking.h"
    #include "serializacion.h"
    #include "logging.h"
    #include "config.h"
    #include "utils.h"
    #include "proceso.h"

    #include "memoria_config.h"
    #include "tabla_paginas.h"
    #include "frame.h"
    #include "swap.h"
    #include "procesar_conexion.h"

    int conexion_cpu;
    int tamanio_tabla;

    t_log* logger;
    t_memoria_config* memoria_config;

    // Espacio contiguo que representa la memoria
    void* memoria;

    typedef enum {
        CLOCK,
        CLOCK_MEJORADO
    } t_algoritmo_reemplazo;

    t_algoritmo_reemplazo algoritmo_reemplazo;

    t_dictionary* diccionario_clocks;

    /**
     * @DESC: Hace malloc del tamaño de la memoria
     */ 
    void* inicializar_memoria(void);


    /**
     * @DESC: Mapea algoritmo de reemplazo, posibles valores: clock y clock-modificado
     */ 
    void elegir_algoritmo_reemplazo(char* algoritmo);


    /**
     * @DESC: Algoritmo de reemplazo clock
     * @return: devuelve posicion del frame liberado
     */ 
    t_entrada_segundo_nivel* algoritmo_clock(uint32_t id_proceso);
    
    
    /**
     * @DESC: Algoritmo de reemplazo clock mejorado
     * @return: devuelve posicion del frame liberado
     */ 
    t_entrada_segundo_nivel* algoritmo_clock_mejorado(uint32_t id_proceso);


    /**
     * @DESC: Aumenta el contador que usa el algoritmo clock, cuidando que no exceda la cantidad máxima de frames
     * Si al aumentar el contador se superaría la cantidad de frames que tenemos, el contador se setea en 0
     */ 
    int aumentar_contador_clock(int contador_actual, int maximo);


#endif /* MEMORIA_H_ */
