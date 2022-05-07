#include "kernel.h"

int main(void) {

	// Crear logger
	t_log* logger = log_create("cfg/kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Kernel iniciado");

	// Inicializar configuración
	t_kernel_config* kernel_config = ini_kernel_config("cfg/kernel.config");

	// Conexión con memoria
	conexion_memoria = crear_socket_cliente(kernel_config -> ip_memoria, kernel_config -> puerto_memoria);
	
	// Conexion con CPU
	conexion_cpu = crear_socket_cliente(kernel_config -> ip_cpu, kernel_config -> puerto_cpu_dispatch);
	
	// Iniciar servidor para que se conecte la consola
	int server_fd = iniciar_servidor(logger, "Kernel", kernel_config -> ip_kernel, kernel_config -> puerto_escucha);

	log_info(logger, "Kernel lista para recibir al cliente");
	
	while(server_escuchar(logger, "Kernel", server_fd, (void*)(*procesar_conexion)));

	terminar_programa(conexion_memoria, conexion_cpu, logger, kernel_config);

	//liberar memoria
	return EXIT_SUCCESS;
}

void terminar_programa(int conexion, int otraConexion, t_log* logger, t_kernel_config* config) {
	liberar_socket_cliente(conexion);
	liberar_socket_cliente(otraConexion);
	log_destroy(logger);
	destruir_kernel_config(config);
}

