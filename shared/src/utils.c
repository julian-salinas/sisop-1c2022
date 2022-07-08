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


int hexa_a_int(void* puntero) {
    char* puntero_str = string_from_format("%p", puntero);
    int decimal = 0;
    int base = 1;

    for (int i = strlen(puntero_str) - 1; i >= 0; i--) {
        if(puntero_str[i] >= '0' && puntero_str[i] <= '9')
        {
            decimal += (puntero_str[i] - 48) * base;
            base *= 16;
        }
        else if(puntero_str[i] >= 'A' && puntero_str[i] <= 'F')
        {
            decimal += (puntero_str[i] - 55) * base;
            base *= 16;
        }
        else if(puntero_str[i] >= 'a' && puntero_str[i] <= 'f')
        {
            decimal += (puntero_str[i] - 87) * base;
            base *= 16;
        }
    }

    return decimal;
}
