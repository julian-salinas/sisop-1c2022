#include "cpu.h"

int main(void) {

	logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	config = iniciar_config("cfg/cpu.config");
	
	// obtener valores ip y puerto del archivo cpu.config
	obtener_config_memoria();	

	//iniciar servidor cpu
	int server_cpu = iniciar_servidor(logger, "CPU",IP_MEMORIA, "8030");
	log_info(logger, "CPU lista como servidor");
	int corte=1;
	while (corte==1) {
		int cliente_fd = esperar_clientes(logger, "CPU", server_cpu);
		int cod_op = recibir_header(cliente_fd);
		switch (cod_op) {
		case CONEXION_CPU_MEMORIA:
			//recibe cantidad de entradas por tabla de páginas y tamaño de página;
			//recibir_config_memoria
			log_info(logger, "Llegó configuración de memoria ok.\n");
			corte=0;	
			break;
		case PAQUETE:
			//lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			//list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

	terminar_programa(conexion_memoria, logger, config);
} //end main


void terminar_programa(int conexion, t_log* logger, t_config* config) {
	liberar_socket_cliente(conexion); // liberar conexion
	log_destroy(logger);
	config_destroy(config);
}

void obtener_config_memoria(){
	ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	conexion_memoria = crear_socket_cliente(ip_memoria, puerto_memoria);
	log_info(logger, "Socket cliente creado.");
	t_paquete* req_config_memoria=ini_conexion_cpu_memoria();
	request_config_memoria(req_config_memoria,conexion_memoria);
	liberar_socket_cliente(conexion_memoria);
	log_info(logger, "Request config memoria enviada.");

}

t_paquete* ini_conexion_cpu_memoria(){
	t_paquete* tmp = crear_paquete(CONEXION_CPU_MEMORIA, sizeof(uint8_t)*2);
	return tmp;
}

void request_config_memoria(t_paquete* paquete, int socket_cliente) {
	enviar_paquete(conexion_memoria,paquete);
	destruir_paquete(paquete);
}