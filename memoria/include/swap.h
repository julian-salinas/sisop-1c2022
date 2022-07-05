#ifndef SWAP_H
#define SWAP_H

    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <string.h>
    #include <stdlib.h>
    #include <commons/string.h>
    
    #include "memoria.h"
    #include "tabla_paginas.h"
    #include "frame.h"


    void eliminar_archivo_proceso(uint32_t PID);
    void crear_archivo_proceso(uint32_t PID);


    /**
     * @DESC: Contiene "una entrada" de swap, o sea, lo que se guarda en swap por cada página
     */
    typedef struct {
        int32_t nro_pagina;
        uint32_t dato;
    } t_page_data;


    /**
     * @DESC: Estructura que modela un archivo de Swap de un proceso
     * - path: path absoluto al archivo swap
     * - fd: file descriptor del archivo
     * - size: tamanio total en bytes del archivo
     * - entradas: array con info sobre cada pagina guardada
     */
    typedef struct {
        char* path;
        int fd;
        size_t tamanio;
        t_page_data* entradas;
    } t_swap;


    /**
     * @DESC: Diccionario que contiene varibales t_swap
     * clave: PID
     * valor: t_swap
     */ 
    t_dictionary* diccionario_swap;


    /**
     * @DESC: Mallocquear (?) y guardar los valores de los datos de una pagina
     * @param nro_pagina: numero de la página
     * @param dato: número a guardar
     */ 
    t_page_data* crear_page_data(int32_t nro_pagina, uint32_t dato);


    /**
     * @DESC: Liberar de memoria una variable de tipo t_page_data
     */ 
    void destruir_page_data(t_page_data* page_data);


    /**
     * @DESC: Crear una variable t_swap* con los datos de swap de un proceso
     * @param PID: ID del proceso
     * @tamanio: tamanio del proceso
     */ 
    t_swap* crear_archivo_swap(uint32_t PID, size_t tamanio);


    /**
     * @DESC: Destruye variable t_swap* y todo lo que contiene
     */ 
    void destruir_archivo_swap(t_swap* archivito_swap);


    /**
     * @DESC: Leer por índice el archivo swap
     * @param archivito_swap: swap del proceso
     * @param nro_pagina: numero de la página de la cual se desea obtener los datos que contiene
     */  
    t_page_data* leer_pagina(t_swap* archivito_swap, int nro_pagina);


    /**
     * @DESC: Escribir por índice en swap
     * @param archivito_swap: swap del proceso
     * @param nro_pagina: numero de la página sobre la cual se va a escribir
     * @param page_data: contiene numero de página y dato, es un poco redundante pasar el número
     *                   de página dos veces, pero es mas cómodo/ordenado de esta forma
     */ 
    void escribir_pagina(t_swap* archivito_swap, int nro_pagina, t_page_data* page_data);


    /**
     * @DESC: Mover una página a swap y liberar el frame que estaba ocupando
     * @param PID: ID del proceso "owner" de la página
     * @param entrada: pagina a swappear
     */ 
    void swappear(uint32_t PID, t_entrada_segundo_nivel* entrada);


    /**
     * @DESC: Traer una página que estaba en memoria virtual, a memoria real
     * @param PID: ID del proceso "owner" de la página
     * @param entrada: pagina a swappear
     */ 
    void desswappear(uint32_t PID, t_entrada_segundo_nivel* entrada);

#endif /* SWAP_H */
