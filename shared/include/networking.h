#ifndef NETWORKING_H
#define NETWORKING_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<netdb.h>
    #include<sys/socket.h>
    #include<string.h>
    #include<fcntl.h>
    #include<unistd.h>
    #include<commons/collections/list.h>    


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

#endif /* NETWORKING_H */