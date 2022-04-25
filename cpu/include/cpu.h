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

    void terminar_programa(int, t_log*, t_config*);
    
    // esta funcion es para test, despues se va a eliminar
    void enviar_config(t_config* config, int socket_cliente);

#endif /* CPU_H_ */
