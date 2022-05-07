#include "consola_config.h"

t_consola_config* ini_consola_config(char* path_a_config) {
    t_consola_config* consola_config = malloc(sizeof(t_consola_config));
    
    consola_config -> config = config_create(path_a_config);
    consola_config -> ip_kernel = config_get_string_value(consola_config -> config,"IP_KERNEL");
	consola_config -> puerto_kernel = config_get_string_value(consola_config -> config,"PUERTO_KERNEL");

    return consola_config;
}


void destruir_consola_config(t_consola_config* consola_config) {
    config_destroy(consola_config -> config);

    free((void*) consola_config -> ip_kernel);
    free((void*) consola_config -> puerto_kernel);

    free((void*) consola_config);
}