#ifndef PROCESAR_CONEXION_H
#define PROCESAR_CONEXION_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<commons/log.h>
    #include<semaphore.h>
    #include<pthread.h>

    // includes de shared
    #include "networking.h"
    #include "serializacion.h"
    #include "enum.h"
    #include "proceso.h"
    
    #include "kernel.h"
    #include "semaforos.h"
    #include "colas.h"
    #include "planificadores.h"

    uint32_t contador_id_proceso;

    int estimacion_rafaga_inicial;

    void procesar_conexion(void* args);

    void procesar_conexion_dispatch(void *args);

    /**
     * @DESC: Crear un PCB a partir de un proceso
     * @param proceso: Proceso que envió la consola, contiene tamanio y lista de instrucciones
     * @param socket: Socket del cliente que envió el proceso, para meterlo dentro del PCB
     */ 
    t_PCB* crear_PCB(t_proceso* proceso, int socket);
    
    /**
     * @DESC: Obtener Y CREAR el pcb de un socket
     *  IMPORTANTE: El socket que se envía debe contener SOLO EL BUFFER, o sea, la función da por hecho
     *  que el HEADER del socket ya fue extraído.
     *  Esta función también se encarga de destruir el payload que extrae.
     * @param socket: socket cliente que contiene el proceso a extraer
     */ 
    t_PCB* socket_create_PCB(int socket);


#endif /* PROCESAR_CONEXION_H */
