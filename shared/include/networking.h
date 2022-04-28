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
    #include "enum.h"
    
    #define IP_MEMORIA "127.0.0.1"
    #define PUERTO_MEMORIA "8002"


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
     * @DESC: Enviar un paquete al server
     * @param paquete: puntero a t_paquete que contiene lo que vamos a enviar, en su 
     *                 header contiene el codigo de operacion (uint*_t) y en el payload el buffer
     * @param socket_cliente: socket que será el emisor el paquete
     */
    void enviar_paquete(t_paquete* paquete, int socket_cliente);


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

    /*
    -------------------- Comunicación entre consola y kernel ------------------------------------
    */
    
    /**
     * @DESC: Enviar lista de instrucciones (Consola → Kernel)
     * @param lista_instrucciones: lista que se va a enviar
     * @param socket_cliente: socket que conectado a kernel
     */
    void enviar_lista_instrucciones(t_lista_instrucciones* lista_instrucciones, int socket_cliente); 

    

#endif /* NETWORKING_H */