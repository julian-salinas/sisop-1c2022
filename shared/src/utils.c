#include "utils.h"

void terminar_programa(char* module_name, int conexion, t_log* logger){
    log_info(logger, "Módulo %s finalizado.", module_name);
    liberar_socket_cliente(conexion);
    log_destroy(logger);
}