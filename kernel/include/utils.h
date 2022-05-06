#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<assert.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/collections/list.h>
#include <pthread.h>

#include "networking.h"
#include "serializacion.h"
#include "enum.h"
#include "estructura.h"

#define IP_KERNEL "127.0.0.1"
#define PUERTO_KERNEL "8000"

t_log* logger;

int conexion_cpu;
char* ip_cpu;
char* puerto_cpu_dispach;

int conexion_memoria;
char* ip_memoria;
char* puerto_memoria;

void iterator(char* value);

void procesar_conexion(void* void_args);

void recibir_mensaje(int);

t_PCB* crear_PCB(/*lo que me da la consola*/);


#endif /* UTILS_H_ */
