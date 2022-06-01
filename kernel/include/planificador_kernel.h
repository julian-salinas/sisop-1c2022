#ifndef PLANIFICADOR_KERNEL_H
#define PLANIFICADOR_KERNEL_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<commons/config.h>

    #include "networking.h"
    #include "serializacion.h"
    #include "enum.h"
    #include "proceso.h"
    
    #include "kernel.h"

    typedef enum {
        FIFO,
        SRT
    } algoritmo_planificacion;

    algoritmo_planificacion algoritmo_elegido;

    int calcular_estimacion(t_PCB* pcb);

    void elegir_algoritmo(char* algoritmo);


#endif /* PLANIFICADOR_KERNEL_H */
