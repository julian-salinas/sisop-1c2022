#include "cpu.h"

int main(void) {

	int conexion_memoria;
	char* ip_memoria;
	char* puerto_memoria;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	config = iniciar_config("cfg/cpu.config");

	// obtener valores ip y puerto del archivo cpu.config
	ip_memoria = config_get_string_value(config, "IP_MEMORIA");
	puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

	conexion_memoria = crear_socket_cliente(ip_memoria, puerto_memoria);

	enviar_config(config, conexion_memoria);

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
	agregar_a_paquete(paquete, valor_noop, strlen(valor_noop + 1) * sizeof(char));

	enviar_paquete(paquete, socket_cliente);

	destruir_paquete(paquete);
	free(valor_tlb);
	free(valor_noop);
}