#ifndef MEMORIA_H_
#define MEMORIA_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>
    #include <unistd.h>
    #include <semaphore.h>

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
    int tamanio_tabla;

    // Logger
    t_log* logger;

    t_memoria_config* memoria_config;

    typedef enum {
        CLOCK,
        CLOCK_MEJORADO
    } t_algoritmo_reemplazo;

    t_algoritmo_reemplazo algoritmo_reemplazo;

    // Variable que va a servir para saber cual es la próxima página a analizar
    int contador_clock;
    
    void* inicializar_memoria(void);

    typedef struct{
        void* puntero_frame; // Es la dirección de memoria a la que apunta
        int32_t bit_ocupado;  // Indica si está en uso o no
        int32_t numero_frame; // Es el número de frame 
        // int32_t bit_modificado;  // Indica si fue modificado
        // int32_t bit_uso; //Indica si fue usado recientemente
    } t_frame;

    typedef t_list t_lista_frames;

    t_lista_frames* lista_frames;

    sem_t* mutex_lista_frames;

    void generarFrames(void* memoria, uint32_t tamanio_memoria, uint32_t tamanio_frame);


    /**
     * @DESC: Mapea algoritmo de reemplazo, posibles valores: clock y clock-modificado
     */ 
    void elegir_algoritmo_reemplazo(char* algoritmo);


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


    /**
     * @DESC: Algoritmo de reemplazo clock
     * @return: devuelve posicion del frame liberado
     */ 
    int algoritmo_clock(void);
    
    
    /**
     * @DESC: Algoritmo de reemplazo clock mejorado
     * @return: devuelve posicion del frame liberado
     */ 
    int algoritmo_clock_mejorado(void);


    /**
     * @DESC: Aumenta el contador que usa el algoritmo clock, cuidando que no exceda la cantidad máxima de frames
     * Si al aumentar el contador se superaría la cantidad de frames que tenemos, el contador se setea en 0
     */ 
    void aumentar_contador_clock(void);

#endif /* MEMORIA_H_ */
