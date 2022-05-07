#ifndef CONSOLA_CONFIG_H
#define CONSOLA_CONFIG_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<commons/config.h>

    typedef struct {
        t_config* config;
        char* ip_kernel;
        char* puerto_kernel;   
    } t_consola_config;


    t_consola_config* ini_consola_config(char* path_a_config);

    void destruir_consola_config(t_consola_config* consola_config);

#endif /* CONSOLA_CONFIG_H */
