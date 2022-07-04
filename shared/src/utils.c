#include "utils.h"

void terminar_programa(char* module_name, int conexion, t_log* logger){
    log_info(logger, "Módulo %s finalizado.", module_name);
    liberar_socket_cliente(conexion);
    log_destroy(logger);
}

char* int_a_string(int numerito) {
    return string_from_format("%d", numerito);
}

int round_div_up(int a, int b){
    if (a % b == 0) return a / b;
    return a / b + 1;
}