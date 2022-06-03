#ifndef KERNEL_H_
#define KERNEL_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include<commons/log.h>
    #include<commons/collections/list.h>

    // includes del módulo
    #include "procesar_conexion.h"
    #include "kernel_config.h"
    #include "planificador_kernel.h"

    // includes de shared
    #include "networking.h"
    #include "proceso.h"

    t_log* logger;
    t_kernel_config* kernel_config;

    pthread_t planif_largo_plazo, planif_mediano_plazo, planif_corto_plazo; 

    sem_t* cont_multiprogramacion;

    sem_t* sem_largo_plazo, *sem_corto_plazo;

    sem_t* mutex_contador_id_proceso, *sem_nuevo_proceso;

	int conexion_memoria, conexion_cpu;    

    void finalizar_kernel(int conexion, int otraConexion, t_log* logger, t_kernel_config* config);

    void* func_largo_plazo(void* args);
    
    void* func_mediano_plazo(void* args);
    
    void* func_corto_plazo(void* args);

    bool algoritmo_SJF(void* proceso1, void* proceso2);    

    void magia_blanca(t_list *list);
    

#endif /* KERNEL_H_ */
