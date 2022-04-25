#ifndef NETWORKING_H
#define NETWORKING_H

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

#include "serializacion.h"

#define IP_MEMORIA "127.0.0.1"
#define PUERTO_MEMORIA "8002"


    t_log* logger;

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


        /**
     * @DESC: Prepara al servidor para recibir un cliente (listen)
     * @param socket_servidor: socket del server que se quiere preparar para recibir al cliente
     */ 
    int esperar_cliente(int socket_servidor);


    /**
     * @DESC: Recibe el código de operacion, esta funcion probablemente la implementemos para hacer 
     *        el handshake
     * @param socket_cliente: socket del que vamos a extraer codigo de operacion para aceptar o rechazar luego
     */
    int recibir_operacion(int socket_cliente); 


    /**
     * @DESC: Recibe el buffer del socket cliente
     * @param socket_cliente: socket que nos va a enviar el buffer
     * @param tamanio_buffer: tamanio del buffer
     */
    void* recibir_buffer(int socket_cliente, size_t* tamanio_buffer); 


    /**
     * @DESC: Devuelve una lista con todos los valores que llegaron en el paquete enviado, proceso de deserialización
     * @param socket_cliente: socket que nos envía el paquete
     */
    t_list* recibir_paquete(int socket_cliente); 

    //de tp0
    int iniciar_servidor(void);
    void recibir_mensaje(int);
    void enviar_mensaje(char* mensaje, int socket_cliente);
    void liberar_conexion(int socket_cliente);
    void eliminar_paquete(t_paquete* paquete);
    

#endif /* NETWORKING_H */