#include "consola.h"


int main(int argc, char** argv) {

	/*
	BORRAR LOS PARÁMETROS DEL MAIN Y AGREGAR LAS SIGUIENTES VARIABLES PARA PROBAR:
		int argc = 2;
		char** argv = (char*[]){"4", "/home/utnso/Documentos/test.txt"};
 	*/

	int conexion_kernel;
	char* ip_kernel;
	char* puerto_kernel;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger("cfg/consola.log", "Consola");

	log_info(logger,"Consola iniciada");

	config = iniciar_config("cfg/consola.config");

	ip_kernel = config_get_string_value(config,"IP_KERNEL");
	puerto_kernel = config_get_string_value(config,"PUERTO_KERNEL");

	conexion_kernel = crear_socket_cliente(ip_kernel,puerto_kernel);

	if(argc < 3){
		log_info(logger, "No se recibieron los parámetros correctos. Formato: {./consola} {cantParametros} {tamañoProceso} {pathArchivo}");
		terminar_programa(conexion_kernel, logger, config);
		return 0;
	}

	size_t size_proceso = atoi(argv[1]);
	char* file_path = argv[2];
	ssize_t read; //cantidad de caracteres leídos
	char* line = NULL; //línea leída
	size_t len = 0; //tamaño de la línea leída
	
	t_lista_instrucciones* lista_instrucciones = crear_lista_instrucciones();
	FILE* file_instrucciones = fopen(file_path, "r");
	if (file_instrucciones == NULL){
		log_info(logger, "No se pudo abrir el archivo en el path: %s", file_path);
		terminar_programa(conexion_kernel, logger, config);
		return 0;
	}

	while((read = getline(&line, &len, file_instrucciones)) != -1){
		//Por cada línea que leo, obtengo los tokens, armo la instrucción con sus parámetros y la agrego a la lista
		t_list* lines = list_create();
		char* t = strtok(line, "\n");
		char* token = strtok(t, " ");
		list_add(lines, token);
		while(token != NULL){
			list_add(lines, token);
			token = strtok(NULL, " ");
		}

		t_identificador identificador = mapear_identificador(list_get(lines, 0));
		// if(identificador == NULL) { 
		// 	log_info(logger, "Identificador inválido.");
		// 	terminar_programa(conexion_kernel, logger, config);
		// 	return 0;
		// }
		t_instruccion* instruccion = crear_instruccion(identificador);
		agregar_parametros(identificador, instruccion, lines);
		log_info(logger, "Parámetros agregados.");
		agregar_instruccion_a_lista(lista_instrucciones, instruccion);
	}

	enviar_lista_instrucciones(conexion_kernel, lista_instrucciones);
	log_info(logger, "Lista de instrucciones enviada.");

	destruir_lista_instrucciones(lista_instrucciones);

	terminar_programa(conexion_kernel, logger, config);
}

//TO DO: Pasar esto a shared
void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	log_info(logger, "Consola finalizada.");
	liberar_socket_cliente(conexion);
	log_destroy(logger);
	config_destroy(config);
}
//

t_identificador mapear_identificador(char* identificador){
	
	t_identificador cod_identificador;
		
		if(strcmp(identificador, "NO_OP") == 0){
			cod_identificador = NO_OP;
		}
		else if(strcmp(identificador, "I_O") == 0){
			cod_identificador = I_O;
		}
		else if(strcmp(identificador, "READ") == 0){
			cod_identificador = READ;
		}
		else if(strcmp(identificador, "WRITE") == 0){
			cod_identificador = WRITE;
		}
		else if(strcmp(identificador, "COPY") == 0){
			cod_identificador = COPY;
		}
		else if(strcmp(identificador, "EXIT") == 0){
			cod_identificador = EXIT;
		}
		return cod_identificador;
}

void agregar_parametros(t_identificador identificador, t_instruccion* instruccion, t_list* parametros){

	switch (identificador)
		{
		case NO_OP:
		case READ:
		case I_O:
			agregar_parametro_a_instruccion(instruccion, atoi(list_get(parametros, 1)));
			break;
		case COPY:
		case WRITE:
			agregar_parametro_a_instruccion(instruccion, atoi(list_get(parametros, 1)));
			agregar_parametro_a_instruccion(instruccion, atoi(list_get(parametros, 2)));
			break;
		}
}
