#include "test_config.h"

t_test_config* ini_test_config(char* path_a_config) {
    test_config* test_config = malloc(sizeof(test_config));
    test_config -> config = config_create(path_a_config);
    test_config -> ip_kernel = config_get_string_value(test_config -> config, "IP_KERNEL");
    test_config -> ip_memoria = config_get_string_value(test_config -> config, "IP_MEMORIA");
    test_config -> ip_cpu = config_get_string_value(test_config -> config, "IP_CPU");
    test_config -> puerto_kernel = config_get_string_value(test_config -> config, "PUERTO_KERNEL");
    test_config -> puerto_memoria = config_get_string_value(test_config -> config, "PUERTO_MEMORIA");
    test_config -> puerto_cpu_dispatch = config_get_string_value(test_config -> config, "PUERTO_CPU_DISPATCH");
    test_config -> puerto_cpu_interrupt = config_get_string_value(test_config -> config, "PUERTO_CPU_INTERRUPT");
    return test_config;
}
