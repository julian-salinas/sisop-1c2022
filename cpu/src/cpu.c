#include "cpu.h"

int main(void) {

	logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	config = iniciar_config("cfg/cpu.config");
	
	// obtener valores ip y puerto del archivo cpu.config
	ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");
	conexion_memoria = crear_socket_cliente(ip_memoria, puerto_memoria);
	enviar_config(config, conexion_memoria);
	liberar_socket_cliente(conexion_memoria);

	//conexion con kernel
	int server_fd = crear_socket_servidor(ip_memoria, pueto_escucha_dispatch);//la ip es la misma que kernel
	log_info(logger, "cpu lista para recibir al kernel");
	int cliente_fd = esperar_cliente(server_fd);
	log_info(logger, "Se conecto el kernel!");

	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		log_info(logger, cod_op);
		/*switch (cod_op) {
		case CONFIGS:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}*/
	}

	terminar_programa(conexion_memoria, logger, config);
}


void terminar_programa(int conexion, t_log* logger, t_config* config) {
	liberar_socket_cliente(conexion); // liberar conexion
	log_destroy(logger);
	config_destroy(config);
}


////////// funcion de prueba para testear el envío de paquetes /////////////////////
void enviar_config(t_config* config, int socket_cliente) {
	// leo algunos valores para mandarlos
	char* valor_tlb = config_get_string_value(config, "REEMPLAZO_TLB");
	char* valor_noop = config_get_string_value(config, "RETARDO_NOOP");

	// primero, crear un paquete vacío
	t_paquete* paquete = crear_paquete(CONFIGS, TAMANIO_DEFAULT_BUFFER);
	agregar_a_paquete(paquete, valor_tlb, (strlen(valor_tlb) + 1) * sizeof(char));
	agregar_a_paquete(paquete, valor_noop, (strlen(valor_noop) + 1) * sizeof(char));

	enviar_paquete(socket_cliente,paquete);

	destruir_paquete(paquete);
	free(valor_tlb);
	free(valor_noop);
}

void iterator(char* value) {
	log_info(logger,"%s", value);
}