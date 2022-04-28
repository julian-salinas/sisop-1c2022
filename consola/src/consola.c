#include <stdio.h>
#include <stdlib.h>
#include "consola.h"


int main(void) {

	int conexion_kernel;
	char* ip_kernel;
	char* puerto_kernel;
	char* valor_prueba;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger("cfg/consola.log", "Consola");
	log_info(logger,"Consola iniciada");

	config = iniciar_config("cfg/consola.config");

	ip_kernel = config_get_string_value(config,"IP_KERNEL");
	puerto_kernel = config_get_string_value(config,"PUERTO_KERNEL");
	valor_prueba = "hola";
	printf("%s", ip_kernel);
	printf("%s", puerto_kernel);

	conexion_kernel = crear_socket_cliente(ip_kernel,puerto_kernel);

	enviar_mensaje(valor_prueba,conexion_kernel);

	terminar_programa(conexion_kernel, logger, config);

}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_socket_cliente(conexion);
	log_destroy(logger);
	config_destroy(config);
}
