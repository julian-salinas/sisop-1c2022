#ifndef SERIALIZACION_H
#define SERIALIZACION_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<netdb.h>
    #include<fcntl.h> 
    #include<unistd.h>
    #include<string.h>
    #include<commons/collections/list.h>


    #define TAMANIO_DEFAULT_BUFFER 64


    /**
     * @DESC: Contenido de un buffer para serialización
     *        - offset (Usado para "recorrer" el buffer)
     *        - Tamaño de la información que contiene
     *        - Stream con el contenido del buffer
     */
    typedef struct {
        uint32_t offset;
        size_t tamanio;
        void* stream;
    } t_buffer;


    /**
     * @DESC: Contenido de un paquete
     *        - Header del mensaje (Sacado de un Enum probablemente)
     *        - Payload (Contenido del mensaje)
     */
    typedef struct {
        uint8_t header;
        t_buffer* payload;
    } t_paquete;


    /**
     * @DESC: Crear un buffer vacío para deserializar
     *        - Stream se aloca con el tamaño size
     *        - El offset se inicializa en 0
     * @param tamanio: tamaño del buffer
     * @return buffer*: Puntero al buffer vacío
     */
    t_buffer* crear_buffer(size_t tamanio);


    /**
     * @DESC: Libera memoria del stream del buffer, y luego del buffer
     * @param buffer: puntero a t_buffer a liberar de la memoria
     */
    void destruir_buffer(t_buffer* buffer);


    /**
     * @DESC: Agregar un valor al buffer
     * @param buffer: puntero a t_buffer donde se agregará el valor
     * @param valor_a_agregar: serie de caracteres a agregar al buffer
     * @param tamanio: tamanio del valor a agregar (bytes)
     */ 
    void agregar_a_buffer(t_buffer* buffer, void* valor_a_agregar, size_t tamanio);


    /**
     * @DESC: Realocar memoria del buffer, si el buffer se queda corto de tamanio, se le agregan TAMANIO_DEFAULT_BUFFER
     *        bytes más
     * @param buffer: puntero a t_buffer, donde se modificará el tamaño alocado
     * @param size: nuevo tamanio a agregar al buffer 
     */
    void ajustar_tamanio_memoria_buffer(t_buffer* buffer, size_t size);


    /**
     * @DESC: Crear un paquete con payload (buffer) vacío, listo para usar
     * @param header: header del mensaje a enviar
     * @param tamanio: tamanio que será asignado al buffer
     */ 
    t_paquete* crear_paquete(uint8_t header, size_t tamanio);


    /**
     * @DESC: Liberar memoria del buffer que contiene el paquete, y luego del paquete
     * @param paquete: puntero a t_paquete a librerar de la memoria
     */
    void destruir_paquete(t_paquete* paquete);


    /**
     * @DESC: Agregar una cadena al buffer de un paquete
     * @param paquete: puntero a t_paquete donde se agregará el valor
     * @param valor_a_agregar: valor que se agregará al buffer del paquete
     * @param tamanio: tamanio del valor a agregar
     */
    void agregar_a_paquete(t_paquete* paquete, void* valor_a_agregar, size_t tamanio);


    /**
     * @DESC: Enviar un paquete al server
     * @param paquete: puntero a t_paquete que contiene lo que vamos a enviar, en su 
     *                 header contiene el codigo de operacion (uint*_t) y en el payload el buffer
     * @param socket_cliente: socket que será el emisor el paquete
     */
    void enviar_paquete(t_paquete* paquete, int socket_cliente);


    /**
     * @DESC: Serializar un paquete, esa funcion es usada por enviar_paquete
     * @param paquete: puntero a t_paquete, se va a serializar su contenido
     * @param bytes: tamanio del paquete a serializar
     */ 
    void* serializar_paquete(t_paquete* paquete, size_t bytes);


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


#endif /* SERIALIZACION_H */