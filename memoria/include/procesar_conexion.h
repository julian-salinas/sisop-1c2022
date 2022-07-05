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

    void procesar_conexion(void* void_args);

    void enviar_config_a_cpu(int socket_cliente, t_log* logger, uint8_t paginas_por_tabla, uint8_t tamanio_pagina);
    
    int crear_proceso_memoria(t_PCB* pcb);

    void enviar_boludeces_a_cpu(int32_t nro_tabla_segundo_nivel);

#endif /* PROCESAR_CONEXION_H */
