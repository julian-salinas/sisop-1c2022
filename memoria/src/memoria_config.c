#include "memoria_config.h"

t_memoria_config* ini_memoria_config(char* path_a_config) {
    t_memoria_config* memoria_config = malloc(sizeof(t_memoria_config));
    
    memoria_config -> config = config_create(path_a_config);
    memoria_config -> ip_memoria = config_get_string_value(memoria_config -> config, "IP_MEMORIA");
    memoria_config -> puerto_escucha = config_get_string_value(memoria_config -> config, "PUERTO_ESCUCHA");
    memoria_config -> tamanio_memoria = config_get_int_value(memoria_config -> config, "TAM_MEMORIA");
    memoria_config -> tamanio_pagina = config_get_int_value(memoria_config -> config, "TAM_PAGINA");
    memoria_config -> paginas_por_tabla = config_get_int_value(memoria_config -> config, "PAGINAS_POR_TABLA");
    memoria_config -> retardo_memoria = config_get_int_value(memoria_config -> config, "RETARDO_MEMORIA");
    memoria_config -> algoritmo_reemplazo = config_get_string_value(memoria_config -> config, "ALGORITMO_REEMPLAZO");
    memoria_config -> marcos_por_proceso = config_get_int_value(memoria_config -> config, "MARCOS_POR_PROCESO");
    memoria_config -> retardo_swap = config_get_int_value(memoria_config -> config, "RETARDO_SWAP");
    memoria_config -> path_swap = config_get_string_value(memoria_config -> config, "PATH_SWAP");

    return memoria_config;
}


void destruir_memoria_config(t_memoria_config* memoria_config) {
    config_destroy(memoria_config -> config);
    free((void*) memoria_config -> ip_memoria);   
    free((void*) memoria_config -> puerto_escucha);   
    free((void*) memoria_config -> algoritmo_reemplazo);   
    free((void*) memoria_config -> path_swap);   
    free((void*) memoria_config);   
}