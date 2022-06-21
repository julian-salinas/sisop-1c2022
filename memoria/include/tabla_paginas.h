#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <netdb.h>
    #include <semaphore.h>
    #include <commons/string.h>
    #include <commons/collections/dictionary.h>

    #include "memoria.h"

    // Contadores para IDs de tablas
    int contador_id_primer_nivel;
    int contador_id_segundo_nivel;

    // Mutex para contadores de IDs de tablas
    sem_t* mutex_id_primer_nivel;
    sem_t* mutex_id_segundo_nivel;

    /**
     * @DESC: Entrada de tabla de primer nivel
     *  - numero_tabla: Hace referencia a una tabla del 2do nivel
     */ 
    typedef struct {
        int32_t id_tabla_segundo_nivel;
    } t_entrada_primer_nivel;


    /**
     * @DESC: Entrada de una tabla de segundo nivel
     *  - bit_presencia: 1 si el marco está en memoria principal
     *  - bit_u: indica si la pagina fue usada recientemente, se pone en 1 cuando se trae a memoria y lo modifica el algoritmo clock
     *  - bit_modificado: 1 si el marco fue modificado
     */ 
    typedef struct {
        uint32_t id_marco;
        int8_t bit_presencia;
        int8_t bit_uso;
        int8_t bit_modificado; 
    } t_entrada_segundo_nivel;


    /**
     * Tabla de primer nivel, "tabla previa", cada entrada hace referencia a una tabla de 2do nivel
     *  - cantidad_entradas: cantidad de filas de la tabla
     *  - entradas: filas de la tabla (valores)
     */ 
    typedef struct {
        int32_t id_tabla;
        int32_t cantidad_entradas;
        t_dictionary* entradas;
    } t_tabla_primer_nivel;
    

    /**
     * Tabla de segundo nivel: contiene los marcos de un proceso
     *  - cantidad_entradas: cantidad de filas de la tabla - Se inicializa en 0
     *  - entradas: filas de la tabla (valores)
     */ 
    typedef struct {
        int32_t id_tabla;
        int32_t cantidad_entradas;
        t_dictionary* entradas;
    } t_tabla_segundo_nivel;


    /**
     * @DESC: Inicializa contadores de IDs y semáforos mutex para creación de tablas
     */
    void ini_tablas_de_paginas(void);


    /**
     * @DESC: Crear una tabla de primer nivel (hacer el malloc e inicializar valores)
     * @return t_tabla_primer_nivel
     */ 
    t_tabla_primer_nivel* crear_tabla_primer_nivel(void);


    /**
     * @DESC: Liberar de memoria una estructura de tipo t_tabla_primer_nivel
     */ 
    void destruir_tabla_primer_nivel(t_tabla_primer_nivel* tabla);


    /**
     * @DESC: Crear una tabla de segundo nivel (hacer malloc e inicializar valores)
     * @return t_tabla_segundo_nivel
     */ 
    t_tabla_segundo_nivel* crear_tabla_segundo_nivel(void);
    
    
    /**
     * @DESC: Liberar de memoria una estructura de tipo t_tabla_segundo_nivel
     */ 
    void destruir_tabla_segundo_nivel(t_tabla_segundo_nivel* tabla);
    

    /**
     * @DESC: Crea un puntero a una estructura de tipo entrada para tabla de primer nivel
     * @param id_entrada_segundo_nivel: ID que vinculará la entrada a una tabla de segundo nivel
     */ 
    t_entrada_primer_nivel* crear_entrada_primer_nivel(int id_tabla_segundo_nivel);
    

    /**
     * @DESC: Crea un puntero a una estructura de tipo entrada para tabla de segundo nivel
     * @param id_marco: id del marco/frame que vincula una entrada a un marco
     * @param bit_presencia: 1 o 0. Indica si la tabla está presente en memoria RAM
     * @param bit_uso: 1 o 0. Indica si la pagina fue usada recientemente, se pone en 1 cuando se trae a memoria y lo modifica el algoritmo clock
     * @param bit_modificado: 1 o 0. Indica si el frame fue modificado
     */
    t_entrada_segundo_nivel* crear_entrada_segundo_nivel(int id_marco, int bit_presencia, int bit_uso, int bit_modificado);


    /**
     * @DESC: Agrega una entrada al diccionario de entradas de una tabla de primer nivel
     * @param tabla: estructura tabla de primer nivel a la que se agregará la entrada
     * @param entrada: estructura de entrada de tabla de primer nivel, que contiene ID de una tabla de segundo nivel
     */  
    void agregar_entrada_primer_nivel(t_tabla_primer_nivel* tabla, t_entrada_primer_nivel* entrada);


    /**
     * @DESC: Agrega una entrada al diccionario de entradas de una tabla de segundo nivel
     * @param tabla: estructura tabla de segundo nivel a la que se agregará la entrada
     * @param entrada: estructura de entrada de tabla de segundo nivel, contiene id_marco, bit_p, bit_u y bit_m
     */ 
    void agregar_entrada_segundo_nivel(t_tabla_segundo_nivel* tabla, t_entrada_segundo_nivel* entrada);

#endif