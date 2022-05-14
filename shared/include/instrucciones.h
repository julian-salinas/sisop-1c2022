#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <commons/collections/list.h>
    #include "serializacion.h"
    #include "networking.h"
    #include "enum.h"
    #include "estructura.h"
    

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
     *        - identificador único
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
    typedef t_list t_lista_instrucciones;


    /**
     * @DESC: Estructura que contiene un proceso, con tamanio (ingresado desde la consola) y lista de
     *        instrucciones (pseudocódigo parseado)
     */ 
    typedef struct {
        size_t tamanio;
        t_lista_instrucciones* lista_instrucciones;
    } t_proceso;


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


    /**
     * @DESC: Agrega una instrucción a un buffer
     * @param buffer: buffer donde se agregará la instrucción
     * @param instrucción: instrucción a agregar
     */ 
    void agregar_a_buffer_INSTRUCCION(t_buffer* buffer, t_instruccion* instruccion);


    /**
     * @DESC: Enviar lista de instrucciones (Consola → Kernel)
     * @param lista_instrucciones: lista que se va a enviar
     * @param socket: socket que conectado a kernel
     */
    void enviar_lista_instrucciones(int socket, t_lista_instrucciones* lista_instrucciones); 

    /**
     * @DESC: Toma una lista de instrucciones del buffer
     * @param buffer: buffer de donde se tomará la lista de instrucciones
     */ 
    t_instruccion* buffer_take_INSTRUCCION(t_buffer* buffer);

#endif /* INSTRUCCIONES_H */