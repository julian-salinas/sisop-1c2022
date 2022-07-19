#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<commons/config.h>

    typedef struct {
        t_config* config;
        char* ip_kernel;
        char* ip_memoria; 
        char* ip_cpu;
        char* puerto_kernel; 
        char* puerto_memoria; 
        char* puerto_cpu_dispatch; 
        char* puerto_cpu_interrupt;
    } t_test_config;


    t_test_config* ini_test_config(char* path_a_config);


#endif /* TEST_CONFIG_H */