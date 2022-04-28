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


	//ini servidor
	logger = iniciar_logger("cfg/memoria.log", "memoria");
	log_info(logger, IP_MEMORIA);
	log_info(logger, PUERTO_MEMORIA);
	int server_fd = crear_socket_servidor(IP_MEMORIA, PUERTO_MEMORIA);
	log_info(logger, "Memoria lista para recibir al cliente");
	int cliente_fd = esperar_cliente(server_fd);
	log_info(logger, "Se conecto un cliente!");
	t_list* lista;

	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		//log_info(logger, "Recibí un código de operación.");
		// t_paquete* paquete = malloc(sizeof(t_paquete));
		// paquete->payload = malloc(sizeof(t_buffer));	
		// Primero recibimos el codigo de operacion
		// recv(cliente_fd, &(paquete->header), sizeof(uint8_t), 0);		
		//int cod_op = paquete->header;
		log_info(logger, cod_op);
		switch (cod_op) {
		case CONFIGS:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
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
	config_destroy(config_memoria);
	return EXIT_SUCCESS;
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}

