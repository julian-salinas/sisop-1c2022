#include "cpu.h"

int main(void) {

	logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	//ini config
	cpu_config = ini_cpu_config("cfg/memoria.config");
	
	// obtener valores ip y puerto del archivo config del módulo memoria
	obtener_config_memoria(conexion_memoria);	

	//iniciar servidor cpu
	int server_cpu = iniciar_servidor(logger, "CPU",IP_MEMORIA, "8001");
	log_info(logger, "CPU lista como servidor");

	while(server_listen(logger, "CPU", server_cpu, (void*)(*procesar_conexion)));

	terminar_programa("CPU", server_cpu, logger);

	return EXIT_SUCCESS;
}

void obtener_config_memoria(int conexion_memoria){

	conexion_memoria = crear_socket_cliente(cpu_config->ip_memoria,(char*)&cpu_config->puerto_memoria);
	log_info(logger, "Socket cliente creado.");
	t_paquete* paquete = crear_paquete(CONEXION_CPU_MEMORIA, sizeof(uint8_t)*2);
	enviar_paquete(conexion_memoria,paquete);
	destruir_paquete(paquete);
	liberar_socket_cliente(conexion_memoria);
	log_info(logger, "Request config memoria enviada.");

}