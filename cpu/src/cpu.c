#include "cpu.h"

int main(void) {

	t_log* logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	config = iniciar_config("cfg/cpu.config");
	
	// obtener valores ip y puerto del archivo config del módulo memoria
	obtener_config_memoria(logger);

	//iniciar servidor cpu
	int server_cpu = iniciar_servidor(logger, "CPU",IP_MEMORIA, "8001");
	log_info(logger, "CPU lista como servidor");

	while(server_listen(logger, "CPU", server_cpu, (void*)(*procesar_conexion)));

	terminar_programa("CPU", server_cpu, logger);

	return EXIT_SUCCESS;
}

void obtener_config_memoria(t_log* logger){
	ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	conexion_memoria = crear_socket_cliente(ip_memoria, puerto_memoria);
	log_info(logger, "Socket cliente creado.");
	t_paquete* paquete = crear_paquete(CONEXION_CPU_MEMORIA, sizeof(uint8_t)*2);
	enviar_paquete(conexion_memoria,paquete);
	destruir_paquete(paquete);
	liberar_socket_cliente(conexion_memoria);
	log_info(logger, "Request config memoria enviada.");
}