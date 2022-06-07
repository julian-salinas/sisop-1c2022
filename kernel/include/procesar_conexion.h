#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<commons/log.h>
    #include<semaphore.h>
    #include<pthread.h>

    // includes de shared
    #include "networking.h"
    #include "serializacion.h"
    #include "enum.h"
    #include "proceso.h"
    
    #include "kernel.h"
    #include "semaforos.h"
    #include "colas.h"
    #include "planificadores.h"

    uint32_t contador_id_proceso;

    int estimacion_rafaga_inicial;

    void procesar_conexion(void* void_args);

    t_PCB* crear_PCB(t_proceso* proceso, int fd);

#endif /* PROCESAR_CONEXION_H */
