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
    #include <pthread.h>

    #include "serializacion.h"
    #include "enum.h"
    
    #define IP_MEMORIA "127.0.0.1"
    #define PUERTO_MEMORIA "8002"


    typedef struct {
        t_log* log;
        int fd;
        char* server_name;
    } t_procesar_conexion_args;

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


    /* --------------------------- NUEVA FUNCION -------------------------- */
    /**
     * @DESC: Crear un socket servidor para recibir a cliente
     * @param logger: logger ya creado, que va a loggear el resultado de la operación
     * @param name: nombre del servidor que se está levantando
     * @param ip: ip a la cual va a conectarse el socket
     * @param puerto: puerto al cual va a conectarse el socket
     * @return: devuelve el socket servidor
     */ 
    int iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto);

    
    /* --------------------------- NUEVA FUNCION -------------------------- */
    /**
     * @DESC: Escuchar a nuevos clientes, crear un hilo si un cliente se conecta
     * @param logger: logger que va a loggear el resultado de la operación
     * @param server_name: nombre del servidor (módulo)
     * @param server_socket: socket del servidor
     * @param func_procesar_conexion: función propia de cada modulo, que contiene el switch con 
     *                                codigos de operacion y toda la bola
     */ 
    int server_listen(t_log* logger, char* server_name, int server_socket, void(*func_procesar_conexion)(void));


    /**
     * @DESC: No estoy lo suficientemente informado para dejar una descripción xd
     * @param logger: logger que va a loggear el resultado de la operacion
     * @param name: nombre del servidor
     * @param socket_servidor: socket del servidor
     */
    int esperar_clientes(t_log* logger, const char* name, int socket_servidor); 


    /**
     * @DESC: Prepara al servidor para recibir un cliente (listen)
     * @param socket_servidor: socket del server que se quiere preparar para recibir al cliente
     */ 
    int esperar_cliente(int socket_servidor);


    /**
     * @DESC: Enviar los datos al server
     * @param socket: socket con datos a enviar
     * @param source: puntero a los datos enviados
     * @param size: tamaño de los datos enviados
     */
    void socket_send(int socket, void* source, size_t size);


    /**
     * @DESC: Enviar código de operación
     * @param socket: socket con la info a enviar
     * @param header: codigo_operacion para realizar handshake
     */ 
    void enviar_header(int socket, uint8_t header);
    
    
    /**
     * @DESC: Obtener los valores que contiene un socket
     * @param socket: socket que contiene la info
     * @param dest: puntero donde se guardaran los datos
     * @param size: tamaño de los datos que se van a leer
     * @return: return false si hubo un error en la recepción de los datos
     */ 
    bool socket_get(int socket, void* dest, size_t size);


    /**
     * @DESC: Obtener header del socket (codigo de operacion)
     * @param socket: socket que contiene el request
     * @return: devuelve el código leído
     */ 
    uint8_t recibir_header(int socket);
    

    /**
     * @DESC: Obtener payload (buffer) del socket
     * @param socket: socket que contiene los datos
     * @return: devuelve el buffer leído
     */
    t_buffer* recibir_payload(int socket);


    /**
     * @DESC: Recibir un paquete
     * @param socket: socket que contiene el paquete
     * @param header: header QUE YA SE LEYÓ (habría q mejorar esto)
     */ 
    t_paquete* recibir_paquete(int socket);


    /**
     * @DESC: Enviar un paquete al server
     * @param socket_cliente: socket que será el emisor el paquete
     * @param paquete: puntero a t_paquete que contiene lo que vamos a enviar, en su 
     *                 header contiene el codigo de operacion (uint*_t) y en el payload el buffer
     */
    void enviar_paquete(int socket_cliente, t_paquete* paquete);    

#endif /* NETWORKING_H */