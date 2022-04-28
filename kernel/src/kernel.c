#include <stdio.h>
#include <stdlib.h>
#include "../include/kernel.h"

int main(void) {

	logger = log_create("kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);

	//inicio el servidor para que se conecte la consola
	int server_fd = iniciar_servidor(logger, "Kernel", IP_KERNEL, PUERTO_KERNEL);

	log_info(logger, "Kernel lista para recibir al cliente");
	
	while(server_escuchar(logger, "Kernel",server_fd));

	log_destroy(logger);

	// //liberar memoria
	return EXIT_SUCCESS;
}

