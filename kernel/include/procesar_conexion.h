#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<commons/log.h>

    // includes de shared
    #include "networking.h"
    #include "serializacion.h"
    #include "enum.h"
    #include "proceso.h"

    t_log* logger;

    int conexion_cpu;
    char* ip_cpu;
    char* puerto_cpu_dispach;

    int conexion_memoria;
    char* ip_memoria;
    char* puerto_memoria;

    uint32_t contador_id_proceso;
    int estimacion_rafaga_inicial;

    void procesar_conexion(void* void_args);

    t_PCB* crear_PCB(t_proceso* proceso);

    // Inicializar configuración
    int conexion_cpu;

#endif /* PROCESAR_CONEXION_H */
