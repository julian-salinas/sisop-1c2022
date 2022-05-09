#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

    #include "enum.h"
    #include "serializacion.h"
    #include "networking.h"

    void procesar_conexion(void* void_args);

    void enviar_config_a_cpu(int socket_cliente);

#endif /* PROCESAR_CONEXION_H */
