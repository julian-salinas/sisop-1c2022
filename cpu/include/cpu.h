#ifndef CPU_H_
#define CPU_H_

    #include<stdio.h>
    #include<stdlib.h>

    // includes de las commons
    #include<commons/string.h>
    #include<commons/config.h>

    // includes del módulo
    #include "procesar_conexion.h"

    // includes de shared
    #include "networking.h"
    #include "config.h"
    #include "proceso.h"
    #include "utils.h"

    int conexion_memoria;
	char* ip_memoria;
	char* puerto_memoria;
	char* pueto_escucha_dispatch;

	t_config* config;
 
    // esta función es para pedirle a memoria su configuración
    void obtener_config_memoria(t_log* logger);

#endif /* CPU_H_ */
