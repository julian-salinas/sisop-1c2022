#include "cpu.h"

int main(void) {

	logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	//ini config
	cpu_config = ini_cpu_config("cfg/cpu.config");
	log_info(logger,"Config CPU load");

	// obtener valores ip y puerto del archivo config del módulo memoria
	conexion_memoria = crear_socket_cliente(cpu_config->ip_memoria, cpu_config->puerto_memoria);
	log_info(logger,"Conexión memoria ok.");
	obtener_config_memoria();	

	int pid = fork();

	if(pid == 0){
	//iniciar servidor cpu - Dispatch
	int server_cpu_dispatch = iniciar_servidor(logger, "CPU",IP_MEMORIA, "8001");
	log_info(logger, "CPU Dispatch lista como servidor");

	while(server_listen(logger, "CPU", server_cpu_dispatch, (void*)(*procesar_conexion)));

	terminar_programa("CPU", server_cpu_dispatch, logger);
	}

	else if(pid > 0){
	//iniciar servidor cpu - Interrupt
	int server_cpu_interrupt = iniciar_servidor(logger, "CPU",IP_MEMORIA, "8005");
	log_info(logger, "CPU interrupt lista como servidor");
	//Deberia usar otra funcion en vez de procesar conexion, exclusivamente para atender las interrupciones
	while(server_listen(logger, "CPU", server_cpu_interrupt, (void*)(*procesar_conexion)));
	terminar_programa("CPU", server_cpu_interrupt, logger);
	}

	else {
		perror("forkeando");
	}

	
	destruir_cpu_config(cpu_config);

	return EXIT_SUCCESS;
}

void obtener_config_memoria(){

	t_paquete* paquete_config = crear_paquete(CONEXION_CPU_MEMORIA, sizeof(uint8_t)*2);
	enviar_paquete(conexion_memoria,paquete_config);
	destruir_paquete(paquete_config);
	//liberar_socket_cliente(conexion_memoria);
	log_info(logger, "Request config memoria enviada.");
}