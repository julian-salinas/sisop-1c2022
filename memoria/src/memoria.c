#include "memoria.h"

int main(void) {

	//ini config
	t_memoria_config* memoria_config = ini_memoria_config("cfg/memoria.config");

	//suspension proceso ->liberar espacio de memoria escribiendo en SWAP

	// Acceso a tabla de páginas
	// 1er nivel responder el n° de tabla 2
	// 2do nivel responder el marco (cargar si corresponde y lo que conlleva)

	// Acceso a espacio de usuario


	// Crear logger
	t_log* logger = log_create("cfg/memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Memoria iniciada");

	int server_fd = iniciar_servidor(logger, "memoria", memoria_config -> ip_memoria, memoria_config -> puerto_escucha);	
	log_info(logger, "Memoria lista para recibir al cliente");

	while(server_listen(logger, "memoria", server_fd, (void*)(*procesar_conexion)));

	terminar_programa("Memoria", server_fd, logger);
	
	return EXIT_SUCCESS;
}
