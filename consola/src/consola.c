#include "consola.h"


int main(int argc, const char *argv[]) {

	int conexion_kernel;
	char* ip_kernel;
	char* puerto_kernel;

	t_log* logger;
	t_config* config;

	// Crear logger y loggear inicio de consola
	logger = iniciar_logger("cfg/consola.log", "Consola");
	log_info(logger,"Consola iniciada");

	// Obtener configs
	config = iniciar_config("cfg/consola.config");

	ip_kernel = config_get_string_value(config,"IP_KERNEL");
	puerto_kernel = config_get_string_value(config,"PUERTO_KERNEL");

	conexion_kernel = crear_socket_cliente(ip_kernel,puerto_kernel);

	/* ----------------------------------------------------------
	Voy a crear una instrucción de ejemplo para mandarla a kernel 
	------------------------------------------------------------ */
	// Armo una lista de instrucciones random
	t_instruccion* instruccion_ejemplo = crear_instruccion(EXIT);
	t_lista_instrucciones* lista_instrucciones_ejemplo = crear_lista_instrucciones();

	agregar_parametro_a_instruccion(instruccion_ejemplo, 4);
	agregar_parametro_a_instruccion(instruccion_ejemplo, 8);

	agregar_instruccion_a_lista(lista_instrucciones_ejemplo, instruccion_ejemplo);

 	enviar_lista_instrucciones(conexion_kernel, lista_instrucciones_ejemplo);

	destruir_lista_instrucciones(lista_instrucciones_ejemplo);
	/*------------------------------------------------------------ */

	terminar_programa(conexion_kernel, logger, config);
}


void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_socket_cliente(conexion);
	log_destroy(logger);
	config_destroy(config);
}
