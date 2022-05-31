#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

    #include "enum.h"
    #include "serializacion.h"
    #include "networking.h"
    #include "memoria_config.h"
    #include "estructura.h"
    #include "proceso.h"
    #include "memoria.h"

    void procesar_conexion(void* void_args);

    void enviar_config_a_cpu(int socket_cliente, t_log* logger, uint8_t paginas_por_tabla, uint8_t tamanio_pagina);

#endif /* PROCESAR_CONEXION_H */
