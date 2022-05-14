#ifndef CPU_H_
#define CPU_H_

    #include<stdio.h>
    #include<stdlib.h>

    // includes de las commons
    #include<commons/string.h>
    #include<commons/config.h>

    #include "procesar_conexion.h"

    // includes de shared
    #include "logging.h"
    #include "config.h"
    #include "serializacion.h"
    #include "estructura.h"
    #include "networking.h"
    #include "utils.h"

    int conexion_memoria;
	char* ip_memoria;
	char* puerto_memoria;
	char* pueto_escucha_dispatch;

	t_log* logger;
	t_config* config;
	t_list* lista;

    //esta función es para pedirle a memoria su configuración
    void obtener_config_memoria();

#endif /* CPU_H_ */
