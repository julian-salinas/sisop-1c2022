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

	generarFrames(memoria, memoria_config -> tamanio_memoria, memoria_config -> tamanio_pagina);

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

void generarFrames(t_memoria* memoria, uint32_t tamanio_memoria, uint32_t tamanio_frame){
	int cantidad_frames = tamanio_memoria / tamanio_frame;
	void* aux = memoria -> memoria;
	int contador_frame = 0;
	t_lista_frames* lista_frames = list_create();

	for (size_t i = 0; i < cantidad_frames; i++)
	{
		t_frame* frame = malloc(sizeof(t_frame));
		frame -> bit_ocupado = 0;
		frame -> puntero_frame = aux;
		frame -> numero_frame = contador_frame;

		list_add(lista_frames, frame);
		log_info(logger, "Se creó el frame %d que apunta a la dirección %p", frame -> numero_frame, frame -> puntero_frame);

		aux += tamanio_frame;
		contador_frame ++;
	}
	
}