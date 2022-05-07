#ifndef KERNEL_H_
#define KERNEL_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "utils.h"
    #include "kernel_config.h"
    #include "networking.h"
    #include "config.h"
    #include "serializacion.h"

	
    void terminar_programa(int conexion, int otraConexion, t_log* logger, t_kernel_config* config);

#endif /* KERNEL_H_ */
