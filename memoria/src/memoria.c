/*
 ============================================================================
 Name        : memoria.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "memoria.h"

int main(void) {

	//ini config
	ini_config();

	//ini proceso -> creación estructuras necesarias

	//suspension proceso ->liberar espacio de memoria escribiendo en SWAP

	//acceso a tabla de páginas
	// 1er nivel responder el n° de tabla 2
	// 2do nivel responder el marco (cargar si corresponde y lo que conlleva)

	//acceso a espacio de usuario


	//servidor para CPU
	logger = log_create("memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);

		int server_fd = iniciar_servidor();
		log_info(logger, "Memoria lista para recibir al cliente");
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
			case CONFIGS:
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
	config_destroy(config_memoria);
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

