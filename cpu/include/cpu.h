#ifndef CPU_H_
#define CPU_H_

    #include<stdio.h>
    #include<stdlib.h>

    // includes de las commons
    #include<commons/string.h>
    #include<commons/config.h>

    // includes del módulo
    #include "procesar_conexion.h"
    #include "cpu_config.h"
    #include "ciclo_instruccion.h"
    // includes de shared
    #include "networking.h"
    #include "serializacion.h"
    #include "config.h"
    #include "proceso.h"
    #include "utils.h"

    t_cpu_config* cpu_config;
    int conexion_memoria;
    t_paquete* paquete;
	t_config* config;
    t_log* logger;
    // esta función es para pedirle a memoria su configuración
    void obtener_config_memoria();

    uint8_t paginas_por_tabla;
    uint8_t tamanio_pagina;

    int interrupcion; //0 el kernel no envio interrupcion, 1 interrumpir ejecución

#endif /* CPU_H_ */
