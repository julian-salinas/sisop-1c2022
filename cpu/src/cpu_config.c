#include "cpu_config.h"

t_cpu_config* ini_cpu_config(char* path_a_config) {
    t_cpu_config* cpu_config = malloc(sizeof(t_cpu_config));
    
    cpu_config -> config = config_create(path_a_config);
    cpu_config -> entradas_tlb = config_get_int_value(cpu_config -> config, "ENTRADAS_TLB");
    cpu_config -> reemplazo_tlb = config_get_string_value(cpu_config -> config, "REEMPLAZO_TLB");
    cpu_config -> retardo_noop = config_get_int_value(cpu_config -> config, "RETARDO_NOOP");
    cpu_config -> ip_memoria = config_get_string_value(cpu_config -> config, "IP_MEMORIA");
    cpu_config -> puerto_memoria = config_get_string_value(cpu_config -> config, "PUERTO_MEMORIA");
    cpu_config -> puerto_escucha_dispatch = config_get_int_value(cpu_config -> config, "PUERTO_ESCUCHA_DISPATCH");
    cpu_config -> puerto_escucha_interrupt = config_get_int_value(cpu_config -> config, "PUERTO_ESCUCHA_INTERRUPT");

    return cpu_config;
}


void destruir_cpu_config(t_cpu_config* cpu_config) {
    config_destroy(cpu_config -> config);
    free((void*) cpu_config -> entradas_tlb);   
    free((void*) cpu_config -> reemplazo_tlb);   
    free((void*) cpu_config -> retardo_noop);   
    free((void*) cpu_config -> ip_memoria);   
    free((void*) cpu_config -> puerto_memoria);   
    free((void*) cpu_config -> puerto_escucha_dispatch);   
    free((void*) cpu_config -> puerto_escucha_interrupt);     
    free((void*) cpu_config);   
}