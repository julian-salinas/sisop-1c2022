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

	logger = iniciar_logger();
	log_info(logger,"Consola iniciada");

	config = iniciar_config(CONFIG_PATH);

	ip_kernel = config_get_string_value(config,"IP_KERNEL");
	puerto_kernel = config_get_string_value(config,"PUERTO_KERNEL");
	valor_prueba = "hola";
	printf("%s", ip_kernel);
	printf("%s", puerto_kernel);

	conexion_kernel  = crear_conexion(ip_kernel,puerto_kernel);

	enviar_mensaje(valor_prueba,conexion_kernel);

	terminar_programa(conexion_kernel, logger, config);

}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("consola.log","Consola",1,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(char* path)
{
	t_config* nuevo_config = config_create(path);

	return nuevo_config;
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
