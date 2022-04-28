#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>

#include "serializacion.h"
#include "networking.h"
#include "config.h"

void enviar_mensaje(char* mensaje, int socket_cliente);

#endif /* UTILS_H_ */
