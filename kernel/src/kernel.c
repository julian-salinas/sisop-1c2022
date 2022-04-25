/*
 ============================================================================
 Name        : kernel.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "kernel.h"

int main(void) {

	logger = log_create("kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);

	//inicio el servidor para que se conecte la consola
	int server_fd = iniciar_servidor();
	log_info(logger, "Kernel lista para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);

	t_list* lista;
	
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case MENSAJE:
			recibir_mensaje(cliente_fd);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	//liberar memoria
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}
