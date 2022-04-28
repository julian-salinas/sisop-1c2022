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

#define IP_KERNEL "127.0.0.1"
#define PUERTO_KERNEL "8002"

typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct {
    t_log* log;
    int fd;
    char* server_name;
} t_procesar_conexion_args;


t_log* logger;

void iterator(char* value);

int server_escuchar(t_log*, char*, int);
int esperar_clientes(t_log*, const char*, int);


void* recibir_buffer(int*, int);

int iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto);


void recibir_mensaje(int);


#endif /* UTILS_H_ */
