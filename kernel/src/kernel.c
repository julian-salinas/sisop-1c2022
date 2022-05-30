#include "kernel.h"

int main(void) {

	inicializar_semaforos();

	// Inicializo contador de Procesos para PCB
	contador_id_proceso = 1;
	
	// Crear logger
	t_log* logger = log_create("cfg/kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Kernel iniciado");

	// Inicializar configuración
	t_kernel_config* kernel_config = ini_kernel_config("cfg/kernel.config");

	// Incializo estimacion de rafaga para PCB
	estimacion_rafaga_inicial = kernel_config -> estimacion_inicial;

	// Conexión con memoria
	conexion_memoria = crear_socket_cliente(kernel_config -> ip_memoria, kernel_config -> puerto_memoria);
	
	// Conexion con CPU
	conexion_cpu = crear_socket_cliente(kernel_config -> ip_cpu, kernel_config -> puerto_cpu_dispatch);
	
	// Iniciar servidor para que se conecte la consola
	int server_fd = iniciar_servidor(logger, "Kernel", kernel_config -> ip_kernel, kernel_config -> puerto_escucha);

	log_info(logger, "Kernel lista para recibir al cliente");
	
	while(server_listen(logger, "Kernel", server_fd, (void*)(*procesar_conexion)));

	finalizar_kernel(conexion_memoria, conexion_cpu, logger, kernel_config);
	

	return EXIT_SUCCESS;
}

void finalizar_kernel(int conexion, int otraConexion, t_log* logger, t_kernel_config* config) {
	liberar_socket_cliente(conexion);
	liberar_socket_cliente(otraConexion);
	log_destroy(logger);
	destruir_kernel_config(config);
}

void inicializar_semaforos() {
	mutex_contador_id_proceso = malloc(sizeof(pthread_mutex_t));

	pthread_mutex_init(mutex_contador_id_proceso, NULL);
}