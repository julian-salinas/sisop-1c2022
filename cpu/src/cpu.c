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

	//iniciar servidor cpu
	int server_cpu = iniciar_servidor(logger, "CPU",IP_MEMORIA, "8001");
	log_info(logger, "CPU lista como servidor");

	while(server_listen(logger, "CPU", server_cpu, (void*)(*procesar_conexion)));

	terminar_programa("CPU", server_cpu, logger);
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