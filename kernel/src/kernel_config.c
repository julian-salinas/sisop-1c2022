#include "kernel_config.h"


t_kernel_config* ini_kernel_config(char* path_a_config) {
    t_kernel_config* kernel_config = malloc(sizeof(t_kernel_config));
    t_config* config = config_create(path_a_config);    
    kernel_config -> ip_kernel = config_get_string_value(config, "IP_KERNEL");
    kernel_config -> ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    kernel_config -> puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
    kernel_config -> ip_cpu = config_get_string_value(config, "IP_CPU");
	kernel_config -> puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
	kernel_config -> puerto_cpu_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");
    kernel_config -> puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");
    kernel_config -> algoritmo_planificacion = config_get_string_value(config, "ALGORITMO_PLANIFICACION");
    kernel_config -> estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");
    kernel_config -> alfa = config_get_int_value(config, "ALFA");
    kernel_config -> grado_multiprogramacion = config_get_int_value(config, "GRADO_MULTIPROGRAMACION");
    kernel_config -> tiempo_maximo_bloqueado = config_get_int_value(config, "TIEMPO_MAXIMO_BLOQUEADO");

    return kernel_config;
}


void destruir_kernel_config(t_kernel_config* kernel_config) {
    free((void*) kernel_config -> ip_kernel);   
    free((void*) kernel_config -> ip_memoria);   
    free((void*) kernel_config -> puerto_memoria);   
    free((void*) kernel_config -> ip_cpu);   
    free((void*) kernel_config -> puerto_cpu_dispatch);   
    free((void*) kernel_config -> puerto_cpu_interrupt);   
    free((void*) kernel_config -> puerto_escucha);   
    free((void*) kernel_config -> algoritmo_planificacion);   
    free((void*) kernel_config -> estimacion_inicial);   
    free((void*) kernel_config -> alfa);   
    free((void*) kernel_config -> grado_multiprogramacion);   
    free((void*) kernel_config -> tiempo_maximo_bloqueado);
    free((void*) kernel_config);   
}