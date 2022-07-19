#include "consola.h"

int main(int argc, char** argv) {

	t_consola_config* consola_config = ini_consola_config("cfg/consola.config");
	logger_consola = iniciar_logger("cfg/consola.log", "Consola");

	log_info(logger_consola, "Consola iniciada");

	int conexion_kernel = crear_socket_cliente(consola_config -> ip_kernel,consola_config -> puerto_kernel);

	if (!validar_argumentos(argc)) {
		free_consola(conexion_kernel, consola_config);
		return 0;
	}

	// Tomar valores enviados por consola por el usuario
	size_t size_proceso = atoi(argv[1]);
	char* file_path = (char*) argv[2];
	
	FILE* file_instrucciones = fopen(file_path, "r");
	
	// Verificar que se haya podido acceder al archivo
	if (!validar_apertura_archivo(file_instrucciones, file_path)) {
		free_consola(conexion_kernel, consola_config);
		return 0;
	}

	/* ------------------ Parsear archivo de pseudocódigo ------------------ */

	t_lista_instrucciones* lista_instrucciones = parsear_pseudocodigo(file_instrucciones);
	
	if (lista_instrucciones == NULL) {
		free_consola(conexion_kernel, consola_config);
		return 0;
	}

	// Enviar lista de instrucciones a kernel
	t_proceso* proceso = crear_proceso(size_proceso, lista_instrucciones);
	enviar_proceso(conexion_kernel, proceso);

	log_info(logger_consola, "Se envió el cod operacion %d", NUEVO_PROCESO);
	log_info(logger_consola, "Lista de instrucciones enviada.");

	uint8_t respuesta_kernel = recibir_header(conexion_kernel);
	log_info(logger_consola, "Respuesta recibida: %d", respuesta_kernel);

	destruir_proceso(proceso);

	free_consola(conexion_kernel, consola_config);

	return 0;

}