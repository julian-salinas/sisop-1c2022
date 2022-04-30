#include "utils.h"

void iterator(char* value) {
	log_info(logger,"%s", value);
}


void* procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int socket_cliente = args->fd;
    char* nombre_servidor = args->server_name;
    
    free(args);

    codigo_operacion op_code;

    while (socket_cliente != -1) {
        t_paquete* paquete;
        op_code = recibir_header(socket_cliente);

        switch (op_code) {
            case PAQUETE:
                log_info(logger, "RECIBIMOS CODIGO PAQUETE");
                paquete = recibir_paquete(socket_cliente, op_code);

                // Acá se lee el valor q mandó consola, esto es un ejemplo
                char* valor_prueba = buffer_take_STRING(paquete -> payload);
                break;

            case -1:
                log_error(logger, "Cliente desconectado de %s...", nombre_servidor);
                return;

            default:
                log_error(logger, "Algo anduvo mal en el server de %s", op_code);
                log_info(logger, "Cop: %d", op_code);
                return;
        }
    }
    log_warning(logger, "El cliente se desconecto de %s server", nombre_servidor);
    return;
}