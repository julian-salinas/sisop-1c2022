#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<sys/socket.h>
#include<commons/log.h>
#include<commons/collections/list.h>

#include "networking.h"
#include "serializacion.h"
#include "enum.h"
#include "estructura.h"

t_log* logger;

int conexion_cpu;
char* ip_cpu;
char* puerto_cpu_dispach;

int conexion_memoria;
char* ip_memoria;
char* puerto_memoria;

uint32_t contador_id_proceso;
int estimacion_rafaga_inicial;

void procesar_conexion(void* void_args);

t_PCB* crear_PCB(/*lo que me da la consola*/);

#endif /* PROCESAR_CONEXION_H */
