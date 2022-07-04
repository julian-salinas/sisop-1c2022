#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <netdb.h>
    #include <semaphore.h>
    #include <commons/string.h>
    #include <commons/collections/dictionary.h>
    #include <commons/collections/list.h>

    #include "utils.h"
    #include "memoria.h"

    // Diccionarios que contienen todas las tablas
    t_dictionary* tablas_primer_nivel;
    t_dictionary* tablas_segundo_nivel;

    // Mutex para diccionarios de las tablas
    sem_t* mutex_tablas_primer_nivel;
    sem_t* mutex_tablas_segundo_nivel;

    int id_tablas_segundo_nivel;
    sem_t* mutex_id_tablas;

    /**
     * @DESC: Entrada de una tabla de segundo nivel
     *  - bit_presencia: 1 si el marco está en memoria principal
     *  - bit_u: indica si la pagina fue usada recientemente, se pone en 1 cuando se trae a memoria y lo modifica el algoritmo clock
     *  - bit_modificado: 1 si el marco fue modificado
     */ 
    typedef struct {
        int32_t nro_frame;
        int8_t bit_presencia;
        int8_t bit_uso; 
        int8_t bit_modificado; 
    } t_entrada_segundo_nivel;


    /**
     * Tabla de primer nivel, "tabla previa", cada entrada hace referencia a una tabla de 2do nivel
     *  - cantidad_entradas: cantidad de filas de la tabla
     *  - entradas: lista de UINT32_T
     */ 
    typedef struct {
        int32_t id_tabla;
        t_list* entradas;
    } t_tabla_primer_nivel;
    

    /**
     * Tabla de segundo nivel: contiene los marcos de un proceso
     *  - cantidad_entradas: cantidad de filas de la tabla - Se inicializa en 0
     *  - entradas: filas de la tabla (valores)
     */ 
    typedef struct {
        int32_t id_tabla;
        t_list* entradas;
    } t_tabla_segundo_nivel;


    /**
     * @DESC: Inicializa contadores de IDs y semáforos mutex para creación de tablas
     */
    void inicializar_tablas_de_paginas(void);


    /**
     * @DESC: Crear una tabla de primer nivel (hacer el malloc e inicializar valores)
     * @return t_tabla_primer_nivel
     */ 
    t_tabla_primer_nivel* crear_tabla_primer_nivel(int id_tabla);


    /**
     * @DESC: Crear una tabla de segundo nivel (hacer malloc e inicializar valores)
     * @return t_tabla_segundo_nivel
     */ 
    t_tabla_segundo_nivel* crear_tabla_segundo_nivel(void);
    
    
    /**
     * @DESC: Liberar de memoria una estructura de tipo t_tabla_primer_nivel
     */ 
    void destruir_tabla_primer_nivel(t_tabla_primer_nivel* tabla);
    
    
    /**
     * @DESC: Liberar de memoria una estructura de tipo t_tabla_segundo_nivel
     */ 
    void destruir_tabla_segundo_nivel(t_tabla_segundo_nivel* tabla);
    

    /**
     * @DESC: Crea un puntero a una estructura de tipo entrada para tabla de segundo nivel
     * @param nro_frame: número de frame que está ocupando la página - frame que vincula una entrada a un marco
     * Todos los bits de estado están inicializados en 0
     */
    t_entrada_segundo_nivel* crear_entrada_segundo_nivel(int nro_frame);


    /**
     * @DESC: Agrega una entrada a la lista de entradas de una tabla de primer nivel
     * @param tabla: estructura tabla de primer nivel a la que se agregará la entrada
     * @param id_tabla_segundo_nivel: id tabla de segundo nivel
     */  
    void agregar_entrada_primer_nivel(t_tabla_primer_nivel* tabla, int32_t id_tabla_segundo_nivel);


    /**
     * @DESC: Agrega una entrada a la lista de entradas de una tabla de segundo nivel
     * @param tabla: estructura tabla de segundo nivel a la que se agregará la entrada
     */ 
    void agregar_entrada_segundo_nivel(t_tabla_segundo_nivel* tabla);


    /**
     * @DESC: Concatenar en una única lista todos los frames que un proceso tiene asignados
     * @param id: id del proceso
     */ 
    t_list* get_entradas_en_memoria_proceso(uint32_t id);

    /**
     * @DESC: Obtener la cantidad de entradas de segundo nivel que tiene un proceso
     */ 
    int get_cantidad_entradas_proceso(uint32_t PID);

#endif