#ifndef KERNEL_H_
#define KERNEL_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include<commons/log.h>

    // includes del módulo
    #include "procesar_conexion.h"
    #include "kernel_config.h"

    // includes de shared
    #include "networking.h"
    #include "proceso.h"

    void finalizar_kernel(int conexion, int otraConexion, t_log* logger, t_kernel_config* config);

#endif /* KERNEL_H_ */
