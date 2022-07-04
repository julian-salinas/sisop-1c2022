#ifndef FRAME_H
#define FRAME_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>
    #include <unistd.h>
    #include <semaphore.h>
    #include <commons/collections/list.h>
    #include "memoria.h"

    typedef struct{
        void* puntero_frame; // Es la dirección de memoria a la que apunta
        int32_t bit_ocupado;  // Indica si está en uso o no
        int32_t numero_frame; // Es el número de frame 
    } t_frame;

    typedef t_list t_lista_frames;

    t_lista_frames* lista_frames;

    sem_t* mutex_lista_frames;

    void generarFrames(void* memoria, uint32_t tamanio_memoria, uint32_t tamanio_frame);

    /**
     * @DESC: Devuelve un frame libre en caso de que exista
     * @return frame libre si encontró un frame - NULL si no encontró ningún frame libre
     */ 
    t_frame* get_frame_libre(void);

        /**
     * @DESC: Devuelve la posición de un frame libre en caso de que exista
     * @return posición de frame libre si encontró un frame - 0 si no encontró ningún frame libre
     */ 
    int get_posicion_frame_libre(void);


    /**
     * @DESC: Verifica bit de ocupado de un frame 
     * @return: 1 si un frame está libre - 0 si no lo está
     */
    int frame_esta_libre(uint32_t posicion_frame);


    /**
     * @DESC: Obtiene el frame en una posición (lo saca de lista_frames). Usa semáforo mutex_lista_frames
     * @return: frame de la posición especificada
     */     
    t_frame* get_frame(uint32_t posicion_frame); 

#endif /* FRAME_H */