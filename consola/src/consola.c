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

	// // Validar que se haya enviado tamanio del proceso y path a archivo de pseudocódigo
	// if (argc < 3) {
	// 	log_info(logger, "No se recibieron los parámetros correctos. Formato: {./consola} {cantParametros} {tamañoProceso} {pathArchivo}");
	// 	terminar_programa("Consola", conexion_kernel, logger);
	// 	destruir_consola_config(consola_config);
	// 	return 0;
	// }

	size_t size_proceso = 10; //atoi(argv[1]);
	char* file_path = "pseudo/enunciado.txt"; // (char*) argv[2];
	
	/*------------------ Parsear archivo de pseudocódigo ------------------*/
	size_t read; //cantidad de caracteres leídos
	char* line = NULL; //línea leída
	size_t len = 0; //tamaño de la línea leída
	
	t_lista_instrucciones* lista_instrucciones = crear_lista_instrucciones();
	FILE* file_instrucciones = fopen(file_path, "r");
	
	// Verificar que se haya podido acceder al archivo
	if (file_instrucciones == NULL) {
		log_error(logger, "No se pudo abrir el archivo en el path: %s", file_path);
		terminar_programa("Consola", conexion_kernel, logger);
		destruir_consola_config(consola_config);
		return 0;
	}

	// Leer líneas del archivo
	while((read = getline(&line, &len, file_instrucciones)) != -1){

		//Por cada línea que leo, obtengo los tokens, armo la instrucción con sus parámetros y la agrego a la lista 
		t_list* lines = list_create();
		char* t = strtok(line, "\n"); 
		char** tokens = string_split(t, " "); 

		// Agregar a la lista el indentificador y los parámetros de la instrucción 
		int param, i = 0;

		while(tokens[i] != NULL){ 
			log_info(logger, "Se agregó el token %s", tokens[i]); 

			if (i >= 1) {
				param = atoi(tokens[i]);
				list_add(lines, param);
			}

			else {
				param = (void*) tokens[i];
				list_add(lines, (void*) tokens[i]);
			}

            i++;
        }

        t_identificador identificador = mapear_identificador(list_get(lines, 0));

        // Si hubo algun problema de sintaxis o algun identificador inválido, terminar ejecución del programa
        if(identificador == -1) { 
            log_info(logger, "Identificador inválido.");
            terminar_programa("Consola", conexion_kernel, logger);
			destruir_consola_config(consola_config);
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
	destruir_consola_config(consola_config);
	terminar_programa("Consola", conexion_kernel, logger);
}