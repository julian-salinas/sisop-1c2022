#ifndef MEMORIA_H_
#define MEMORIA_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>

    // includes del módulo
    #include "procesar_conexion.h"
    #include "memoria_config.h"
    #include "swap.h"

    //includes de shared
    #include "networking.h"
    #include "serializacion.h"
    #include "logging.h"
    #include "config.h"
    #include "utils.h"
    #include "proceso.h"

    int conexion_cpu;

    // Logger
    t_log* logger;

    t_memoria_config* memoria_config;

    typedef struct {
        void* memoria;
    } t_memoria;

    t_memoria* inicializar_memoria(void);

    typedef struct{
        void* puntero_frame; //es la dirección de memoria a la que apunta
        int32_t bit_ocupado; //indica si está en uso o no
        int32_t pagina; //la pagina que lo ocupa
        int32_t numero_frame; //es el número de frame 
    }t_frame;

    typedef t_list t_lista_frames;

    void generarFrames(t_memoria* memoria, uint32_t tamanio_memoria, uint32_t tamanio_frame);

#endif /* MEMORIA_H_ */
