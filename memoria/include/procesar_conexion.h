#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

    #include "enum.h"
    #include "serializacion.h"
    #include "networking.h"
    #include "memoria_config.h"
    #include "estructura.h"
    #include "proceso.h"
    #include "memoria.h"
    #include "tabla_paginas.h"
    #include "utils.h"

    void procesar_conexion(void* void_args);

    void procesar_conexion_kernel_memoria(int socket_cliente);

    void procesar_conexion_cpu_memoria(int socket_cliente);

    void enviar_config_a_cpu(int socket_cliente, t_log* logger, uint32_t paginas_por_tabla, uint32_t tamanio_pagina);
    
    void enviar_boludeces_a_cpu(int socket_cliente, int32_t nro_tabla_segundo_nivel);

    int crear_proceso_memoria(t_PCB* pcb);

#endif /* PROCESAR_CONEXION_H */
