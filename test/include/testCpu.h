#ifndef TESTCPU_H_
#define TESTCPU_H_

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

    #include "procesar_conexion.h"

    int contador_id_proceso;
    t_log* logger;

    /**
    * @DESC: test para cpu
    */

   t_PCB* crear_PCB(t_proceso* proceso, int fd);
  int  conexion_cpu_dispatch;
    
#endif /* TESTCPU_H */
