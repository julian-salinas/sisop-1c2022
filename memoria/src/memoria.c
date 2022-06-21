#include "memoria.h"

int main(void) {

	// Inicializar config
	memoria_config = ini_memoria_config("cfg/memoria.config");

	// Inicializar estructuras de tablas de páginas
	inicializar_tablas_de_paginas();
	
	// Inicializar memoria
	t_memoria* memoria = inicializar_memoria();

	// Crear logger
	logger = log_create("cfg/memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Memoria iniciada");

	int server_fd = iniciar_servidor(logger, "memoria", memoria_config -> ip_memoria, memoria_config -> puerto_escucha);	
	log_info(logger, "Memoria lista para recibir al cliente");


	while(server_listen(logger, "memoria", server_fd, (void*)(*procesar_conexion)));

	terminar_programa("Memoria", server_fd, logger);
	
	return EXIT_SUCCESS;
}

t_memoria* inicializar_memoria(void) {
	t_memoria* memoria = malloc(sizeof(t_memoria));
	memoria -> memoria = malloc(memoria_config -> tamanio_memoria);

	return memoria;
}