#ifndef SERIALIZACION_H
#define SERIALIZACION_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<netdb.h>
    #include<fcntl.h> 
    #include<unistd.h>
    #include<string.h>
    #include<commons/collections/list.h>
    #include<commons/string.h>

    #define TAMANIO_DEFAULT_BUFFER 64

    #include "enum.h"
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
        codigo_operacion header;
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
     * @DESC: Serializar un paquete, esa funcion es usada por enviar_paquete
     * @param paquete: puntero a t_paquete, se va a serializar su contenido
     * @param bytes: tamanio del paquete a serializar
     */ 
    void* serializar_paquete(t_paquete* paquete, size_t bytes);


    void agregar_a_buffer_INT32_P(t_buffer* stream, void* source);

    void agregar_a_buffer_INT32(t_buffer* stream, int32_t value);

    void agregar_a_buffer_UINT32_P(t_buffer* stream, void* source);

    void agregar_a_buffer_UINT32(t_buffer* stream, uint32_t value);

    void agregar_a_buffer_UINT8_P(t_buffer* stream, void* source);

    void agregar_a_buffer_UINT8(t_buffer* stream, uint8_t value);

    void agregar_a_buffer_STRING_P(t_buffer* paquete, void* source);

    void agregar_a_buffer_STRING(t_buffer* stream, char* source);

    // ------------------------- Ir leyendo paquete ------------------------- //

    void buffer_take(t_buffer* buffer, void** dest, size_t size);

    void buffer_take_INT32_P(t_buffer* stream, void** dest);

    int32_t buffer_take_INT32(t_buffer* stream);

    void buffer_take_UINT32_P(t_buffer* stream, void** dest);

    uint32_t buffer_take_UINT32(t_buffer* stream);

    void buffer_take_UINT8_P(t_buffer* stream, void** dest);

    uint8_t buffer_take_UINT8(t_buffer* stream);

    void buffer_take_STRING_P(t_buffer* stream, void** dest);

    char* buffer_take_STRING(t_buffer* stream);

    /*
    -------------------- Comunicación entre consola y kernel ------------------------------------
    */

   /**
    * @DESC: Enumerado con posibles instrucciones que va a parsear la consola
    */ 
    typedef enum {
        NO_OP,
        I_O,
        READ,
        WRITE,
        COPY,
        EXIT
    } t_identificador;


    /**
     * @DESC: Contenido de una instruccion
     *        = identificador único
     *        - Puede tener 0, 1 o 2 parámetros
     */ 
    typedef struct {
        t_identificador identificador;
        t_list* parametros;
    } t_instruccion;


    /**
     * @DESC: Estructura para referirse a una lista de instrucciones, no es más que un alias
     *        para ganar expresividad
     */  
    typedef struct {
        t_list* instrucciones; 
    } t_lista_instrucciones;


    /**
     * @DESC: Crea una instrucción vacía
     * @param tamanio: tamanio en memoria que va a ocupar la instruccion
     * @return: devuelve un puntero a t_instruccion, con el identificador y una lista
     *          de parámetros inicializada (vacía)
     */  
    t_instruccion* crear_instruccion(t_identificador identificador);


    /**
     * @DESC: Liberar de la memoria una instruccion, primero se libera
     *        la lista de parámetros, y luego la instrucción en sí
     * @param instrucción: instrucción que va a ser destruída
     */
    void destruir_instruccion(t_instruccion* instruccion); 


    /**
     * @DESC: Agregar un parámetro a una instruccion
     * @param parametro: parámetro que va a agregarse a la lista de parámetros de la instrucción
     */
    void agregar_parametro_a_instruccion(t_instruccion* instruccion, int parametro); 


    /**
     * @DESC: Crear una lista de instrucciones vacía
     * @return: puntero a lista de t_lista_instrucciones
     */
    t_lista_instrucciones* crear_lista_instrucciones(void); 


    /**
     * @DESC: Liberar de la memoria una lista de instrucciones
     * @param lista_instrucciones: lista de instrucciones a liberar
     */ 
    void destruir_lista_instrucciones(t_lista_instrucciones *lista_instrucciones);


    /**
     * @DESC: Agregar una instruccion a una lista de instrucciones
     * @param lista_instrucciones: lista a la cual se va a agregar la instruccion
     * @param instruccion: instruccion que será agregada
     */ 
    void agregar_instruccion_a_lista(t_lista_instrucciones* lista_instrucciones, t_instruccion* instruccion);


#endif /* SERIALIZACION_H */