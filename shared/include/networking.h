#ifndef NETWORKING_H
#define NETWORKING_H

#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/socket.h>
#include<string.h>


/**
 * @DESC: Función que devuelve un socket cliente para conectarse
 * @param ip: ip a la cual va a conectarse el socket
 * @param puerto: puerto al cual va a conectarse el socket
 */ 
int crear_socket_cliente(char* ip, char* puerto);


/**
 * @DESC: Liberar memoria del socket del cliente
 * @param socket_cliente: socket el cual se quiere cerrar la conexión (no se va a usar más)
*/
void liberar_socket_cliente(int socket_cliente);


/**
 * @DESC: Crear un socket servidor para recibir datos del cliente
 * @param ip: ip a la cual va a conectarse el socket
 * @param puerto: puerto al cual va a conectarse el socket
 */
int crear_socket_servidor(char *ip, char *puerto);

#endif /* NETWORKING_H */