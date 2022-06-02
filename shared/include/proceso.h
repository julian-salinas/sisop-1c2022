#ifndef PROCESO_H
#define PROCESO_H

    #include <stdio.h>
    #include <string.h>
    #include "networking.h"
    #include "serializacion.h"
    #include "instrucciones.h"


    typedef enum {
        NEW,
        READY,
        RUNNING,
        BLOCKED,
        SUSPENDED_BLOCKED,
        SUSPENDED_READY,
    } estado_proceso;

    typedef struct {
        uint32_t PID;
        size_t tamanio; 
        t_lista_instrucciones* lista_instrucciones;
        int program_counter;
        int32_t tabla_paginas;
        int estimacion_rafaga;
        int tiempo_ejecucion;
        int socket_cliente;
        estado_proceso estado;

    } t_PCB;

    t_proceso* crear_proceso(size_t tamanio, t_lista_instrucciones* lista_instrucciones);

    void destruir_proceso(t_proceso* proceso);

    void enviar_proceso(int socket, t_proceso* proceso);

    void agregar_a_buffer_PROCESO(t_buffer* buffer, t_proceso* proceso); 

    t_proceso* buffer_take_PROCESO(t_buffer* buffer);

    void enviar_pcb(int socket, t_PCB* pcb);

    t_PCB* buffer_take_PCB(t_buffer* buffer);
                
#endif