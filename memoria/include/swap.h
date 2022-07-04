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
    #include "frame.h"
    #include "tabla_paginas.h"
    #include "memoria_config.h"

    void eliminar_archivo_proceso(uint32_t PID);
    void crear_archivo_proceso(uint32_t PID);

    
    typedef struct {
        int32_t nro_pagina;
        uint32_t dato;
    } t_page_data;


    typedef struct {
        char* path;
        int fd;
        size_t tamanio;
        t_page_data* entradas;
    } t_swap;


    t_dictionary* diccionario_swap;

    t_page_data* crear_page_data(int32_t nro_pagina, uint32_t dato);


    void destruir_page_data(t_page_data* page_data);


    t_swap* crear_archivo_swap(uint32_t PID, size_t tamanio);


    void destruir_archivo_swap(t_swap* archivito_swap);


    t_page_data* leer_indice(t_swap* archivito_swap, int indice);


    void escribir_indice(t_swap* archivito_swap, int indice, t_page_data* page_data);


    void swappear_a_disco(uint32_t PID, t_entrada_segundo_nivel* entrada);


#endif /* SWAP_H */
