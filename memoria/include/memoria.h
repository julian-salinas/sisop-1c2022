#ifndef MEMORIA_H_
#define MEMORIA_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

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

#endif /* MEMORIA_H_ */
