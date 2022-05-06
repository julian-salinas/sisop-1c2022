#include "kernel.h"

int main(void) {

	logger = log_create("cfg/kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Kernel iniciado");

	config = iniciar_config("cfg/kernel.config");

	// conexion con memoria
	ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	conexion_memoria = crear_socket_cliente(ip_memoria, puerto_memoria);
	enviar_config(config, conexion_memoria);
	
	//conexion con CPU
	ip_cpu = config_get_string_value(config, "IP_CPU");
	puerto_cpu_dispach = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
	conexion_cpu = crear_socket_cliente(ip_cpu, puerto_cpu_dispach);
	enviar_config(config, conexion_cpu);
	
	//inicio el servidor para que se conecte la consola
	int server_fd = iniciar_servidor(logger, "Kernel", IP_KERNEL, PUERTO_KERNEL);

	log_info(logger, "Kernel lista para recibir al cliente");
	
	while(server_escuchar(logger, "Kernel", server_fd, procesar_conexion)) {
		// recibir de la consola
		// crear_PCB(lo que me da la consola);
		// enviar a la cpu el pcb
	}

	terminar_programa(conexion_memoria, conexion_cpu, logger, config);

	//liberar memoria
	return EXIT_SUCCESS;
}

void terminar_programa(int conexion, int otraConexion, t_log* logger, t_config* config) {
	liberar_socket_cliente(conexion);
	liberar_socket_cliente(otraConexion); // liberar conexion
	log_destroy(logger);
	config_destroy(config);
}

