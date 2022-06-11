#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

  
    #include <stdio.h>
    #include <stdlib.h>
    #include <commons/string.h>
    #include <commons/txt.h>
    
    // Includes de shared
    #include "networking.h"
    #include "logging.h"
    #include "instrucciones.h"
    #include "proceso.h"
    #include "utils.h"
    #include "enum.h"

    #include "testCpu.h"

    void procesar_conexion(void* void_args);

#endif /* PROCESAR_CONEXION_H */
