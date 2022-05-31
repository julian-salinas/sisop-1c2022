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

    int calcular_estimacion(t_PCB* pcb);


#endif /* PLANIFICADOR_KERNEL_H */
