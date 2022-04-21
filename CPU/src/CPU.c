/*
 ============================================================================
 Name        : CPU.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "CPU.h"

int main(void) {

	int conexion_memoria;
	char* ip_memoria;
	char* puerto_memoria;
	char* valor_prueba;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	log_info(logger,"CPU iniciada");

	config = iniciar_config();

	ip_memoria = config_get_string_value(config,"IP_MEMORIA");
	puerto_memoria = config_get_string_value(config,"PUERTO_MEMORIA");
	valor_prueba = config_get_string_value(config,"REEMPLAZO_TLB"); //prueba de mandar algo

	conexion_memoria  = crear_conexion(ip_memoria,puerto_memoria);

	enviar_mensaje(valor_prueba,conexion_memoria);

	terminar_programa(conexion_memoria, logger, config);
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("CPU.log","CPU",1,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("CPU.config");

	return nuevo_config;
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}
