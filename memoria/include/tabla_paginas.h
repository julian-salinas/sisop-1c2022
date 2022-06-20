#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

    #include<stdio.h>
    #include<stdlib.h>
    #include <netdb.h>
    #include <stdbool.h>


    /**
     * @DESC: Entrada de tabla de primer nivel
     *  - numero_tabla: Hace referencia a una tabla del 2do nivel
     */ 
    typedef struct {
        int32_t numero_tabla;
    } t_entrada_primer_nivel;


    /**
     * @DESC: Entrada de una tabla de segundo nivel
     *  - bit_presencia: 1 si el marco está en memoria principal
     *  - bit_u: indica si la pagina fue usada recientemente, se pone en 1 cuando se trae a memoria y lo modifica el algoritmo clock
     *  - bit_modificado: 1 si el marco fue modificado
     */ 
    typedef struct {
        uint32_t marco;
        bool bit_presencia;
        bool bit_u;
        bool bit_modificado; 
    } t_entrada_segundo_nivel;


    /**
     * Tabla de primer nivel, "tabla previa", cada entrada hace referencia a una tabla de 2do nivel
     *  - cantidad_entradas: cantidad de filas de la tabla
     *  - entradas: filas de la tabla (valores)
     */ 
    typedef struct {
        int32_t cantidad_entradas;
        t_entrada_primer_nivel* entradas;
    } t_tabla_primer_nivel;


    /**
     * Tabla de segundo nivel: contiene los marcos de un proceso
     *  - cantidad_entradas: cantidad de filas de la tabla - Se inicializa en 0
     *  - entradas: filas de la tabla (valores)
     */ 
    typedef struct {
        int32_t cantidad_entradas;
        t_entrada_segundo_nivel* entradas;
    } t_tabla_segundo_nivel;


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
    
#endif