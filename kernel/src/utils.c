#include "utils.h"

void iterator(char* value) {
	log_info(logger,"%s", value);
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}


void* procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
	t_list* lista;

    free(args);

    codigo_operacion cop;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &cop, sizeof(codigo_operacion), 0) != sizeof(codigo_operacion)) {
            log_info(logger, "DISCONNECT!");
            return;
        }

        switch (cop) {

			// Aca van los codigos de operacion
			case MENSAJE:
				recibir_mensaje(cliente_socket);
				break;

			case PAQUETE:
				lista = recibir_paquete(cliente_socket);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;

            // Errores
            case -1:
                log_error(logger, "Cliente desconectado de %s...", server_name);
                return;

            default:
                log_error(logger, "Algo anduvo mal en el server de %s", server_name);
                log_info(logger, "Cop: %d", cop);
                return;
        }
    }

    log_warning(logger, "El cliente se desconecto de %s server", server_name);
    return;
}
