#include "consola.h"


int main(int argc, char** argv) {

	/*
	BORRAR LOS PARÁMETROS DEL MAIN Y AGREGAR LAS SIGUIENTES VARIABLES PARA PROBAR:
		int argc = 2;
		char** argv = (char*[]){"4", "/home/utnso/Documentos/test.txt"};
 	*/

	int conexion_kernel;

	t_consola_config* consola_config = ini_consola_config("cfg/consola.config");
	t_log* logger = iniciar_logger("cfg/consola.log", "Consola");

	log_info(logger,"Consola iniciada");

	conexion_kernel = crear_socket_cliente(consola_config -> ip_kernel,consola_config -> puerto_kernel);

	// Validar que se haya enviado tamanio del proceso y path a archivo de pseudocódigo
	// if (argc < 3) {
	// 	log_info(logger, "No se recibieron los parámetros correctos. Formato: {./consola} {cantParametros} {tamañoProceso} {pathArchivo}");
	// 	terminar_programa(conexion_kernel, logger, consola_config);
	// 	return 0;
	// }

	size_t size_proceso = 10; //atoi(argv[1]);
	char* file_path = "pseudo/enunciado.txt"; //argv[2];
	
	/*------------------ Parsear archivo de pseudocódigo ------------------*/
	size_t read; //cantidad de caracteres leídos
	char* line = NULL; //línea leída
	size_t len = 0; //tamaño de la línea leída
	
	t_lista_instrucciones* lista_instrucciones = crear_lista_instrucciones();
	FILE* file_instrucciones = fopen(file_path, "r");
	
	// Verificar que se haya podido acceder al archivo
	if (file_instrucciones == NULL) {
		log_error(logger, "No se pudo abrir el archivo en el path: %s", file_path);
		terminar_programa(conexion_kernel, logger, consola_config);
		return 0;
	}

	// Leer líneas del archivo
	while((read = getline(&line, &len, file_instrucciones)) != -1){
		//Por cada línea que leo, obtengo los tokens, armo la instrucción con sus parámetros y la agrego a la lista
		t_list* lines = list_create();
		
		char* t = strtok(line, "\n");
		char* token = strtok(t, " ");

		// El identificador siempre viene primero, lo agregamos como char* para despues mapearlo
		list_add(lines, token);
		token = strtok(NULL, " ");

		// Agregar a la lista los parámetros de la instrucción
		while(token != NULL){
			log_info(logger, "Se agregó el token %s", token);
			list_add(lines, (void*) atoi(token));
			token = strtok(NULL, " ");
		}

		t_identificador identificador = mapear_identificador(list_get(lines, 0));
		
		// Si hubo algun problema de sintaxis o algun identificador inválido, terminar ejecución del programa
		if(identificador == -1) { 
			log_info(logger, "Identificador inválido.");
			terminar_programa(conexion_kernel, logger, consola_config);
			return 0;
		}

		// Crear la instrucción a enviar
		t_instruccion* instruccion = crear_instruccion(identificador);
		agregar_parametros(identificador, instruccion, lines);
		agregar_instruccion_a_lista(lista_instrucciones, instruccion);
	}

	// Enviar lista de instrucciones a kernel
	t_proceso* proceso = crear_proceso(size_proceso, lista_instrucciones);
	enviar_proceso(conexion_kernel, proceso);

	log_info(logger, "Lista de instrucciones enviada.");

	destruir_proceso(proceso);
	terminar_programa(conexion_kernel, logger, consola_config);
}


// TODO: Pasar esto a shared
void terminar_programa(int conexion, t_log* logger, t_consola_config* config)
{
	log_info(logger, "Consola finalizada.");
	liberar_socket_cliente(conexion);
	log_destroy(logger);
	destruir_consola_config(config);
}


t_identificador mapear_identificador(char* identificador){
	
	t_identificador cod_identificador;
		
		if(strcmp(identificador, "NO_OP") == 0){
			cod_identificador = NO_OP;
		}
		else if(strcmp(identificador, "I/O") == 0){
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
		else {
			cod_identificador = -1;
		}
		return cod_identificador;
}


void agregar_parametros(t_identificador identificador, t_instruccion* instruccion, t_list* parametros){

	switch (identificador)
		{
		// Instrucciones que llevan 1 parámetro
		case NO_OP:
		case READ:
		case I_O: ;
			agregar_parametro_a_instruccion(instruccion, (int) list_get(parametros, 1));
			break;

		// Instrucciones que llevan 2 parámetros
		case COPY:
		case WRITE:
			agregar_parametro_a_instruccion(instruccion, (int) list_get(parametros, 1));
			agregar_parametro_a_instruccion(instruccion, (int) list_get(parametros, 2));
			break;

		// Instruccion que no lleva parámetros
		case EXIT:
			break;
		}
}
