#ifndef CPU_H_
#define CPU_H_

    #include<stdio.h>
    #include<stdlib.h>

    // includes de las commons
    #include<commons/string.h>
    #include<commons/config.h>

    // includes de shared
    #include "logging.h"
    #include "config.h"
    #include "serializacion.h"
    #include "networking.h"

    int conexion_memoria;
	char* ip_memoria;
	char* puerto_memoria;
	char* pueto_escucha_dispatch;

	t_log* logger;
	t_config* config;
	t_list* lista;

    void terminar_programa(int, t_log*, t_config*);
    
    // esta funcion es para test, despues se va a eliminar
    void enviar_config(t_config* config, int socket_cliente);

    //estas funciones son para conectar cpu con memoria
    t_paquete* ini_conexion_cpu_memoria();
    void request_config_memoria(t_paquete* paquete, int socket_cliente);
    void obtener_config_memoria();

#endif /* CPU_H_ */
