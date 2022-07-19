#ifndef CONFIG_H
#define CONFIG_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<commons/config.h>

    typedef struct {
        t_config* config;
        char* ip_kernel;
        char* puerto_escucha; 
        char* ip_memoria; 
        char* puerto_memoria; 
        char* ip_cpu; 
        char* puerto_cpu_dispatch; 
        char* puerto_cpu_interrupt;
        char* algoritmo_planificacion; 
        double estimacion_inicial;
        double alfa;
        int grado_multiprogramacion;
        int tiempo_maximo_bloqueado;    
    } t_kernel_config;


    t_kernel_config* ini_kernel_config(char* path_a_config);


    void destruir_kernel_config(t_kernel_config* kernel_config);

#endif /* INI_CONFIG_H */
