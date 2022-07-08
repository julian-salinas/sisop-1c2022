#include "kernel.h"

int main(void) {

	// Inicializar configuración
	kernel_config = ini_kernel_config("cfg/kernel.config");
	
	// Crear logger
	logger = log_create("cfg/kernel.log", "kernel", 1, LOG_LEVEL_INFO);
	log_info(logger,"Kernel iniciado");

	// Definir qué algoritmo se va a usar para planificar
	elegir_algoritmo(kernel_config -> algoritmo_planificacion);

	// Inicializar semáforos y colas
	ini_semaforos();
	inicializar_mutex_colas();
	inicializar_colas();

	// Inicializar contador de ID Procesos para PCB
	contador_id_proceso = 1;

	// Inicializacion de threads
	ini_threads();

	// Incializo estimacion de rafaga para PCB
	estimacion_rafaga_inicial = kernel_config -> estimacion_inicial;

	// Conexión con memoria
	conexion_memoria = crear_socket_cliente(kernel_config -> ip_memoria, kernel_config -> puerto_memoria);
	
	// Conexion con CPU
	conexion_cpu_dispatch = crear_socket_cliente(kernel_config -> ip_cpu, kernel_config -> puerto_cpu_dispatch);
	conexion_cpu_interrupt = crear_socket_cliente(kernel_config -> ip_cpu, kernel_config -> puerto_cpu_interrupt);
	
	

	// Iniciar servidor para que se conecte la consola
	int server_fd = iniciar_servidor(logger, "Kernel", kernel_config -> ip_kernel, kernel_config -> puerto_escucha);

	log_info(logger, "Kernel esperando clientes");
	
	while(server_listen(logger, "Kernel", server_fd, (void*)(*procesar_conexion)));

    liberar_socket_cliente(conexion_memoria);
    liberar_socket_cliente(conexion_cpu_dispatch);
    liberar_socket_cliente(conexion_cpu_interrupt);
    
    destruir_kernel_config(kernel_config);

	return EXIT_SUCCESS;
}
