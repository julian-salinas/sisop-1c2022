#include "memoria.h"

int main(void) {

	//ini config
	memoria_config = ini_memoria_config("cfg/memoria.config");

	ini_tablas_de_paginas();

	// Crear logger
	logger = log_create("cfg/memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Memoria iniciada");

	int server_fd = iniciar_servidor(logger, "memoria", memoria_config -> ip_memoria, memoria_config -> puerto_escucha);	
	log_info(logger, "Memoria lista para recibir al cliente");

	while(server_listen(logger, "memoria", server_fd, (void*)(*procesar_conexion)));

	terminar_programa("Memoria", server_fd, logger);
	
	return EXIT_SUCCESS;
}
