#ifndef KERNEL_H_
#define KERNEL_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <commons/log.h>
    #include <commons/collections/list.h>

    #include "networking.h"
    #include "proceso.h"

    #include "procesar_conexion.h"
    #include "config.h"
    #include "semaforos.h"
    #include "planificadores.h"

    // Logger
    t_log* logger;

    // Configuración
    t_kernel_config* kernel_config;

    // Conexiones (sockets)
    int conexion_memoria, conexion_cpu;

    // Variable que nos va a ayudar a saber si tenemos multiprogramacion disponible
    int grado_multiprogramacion_usado;


#endif /* KERNEL_H_ */
