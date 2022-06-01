#include "parser.h"

t_lista_instrucciones* parsear_pseudocodigo(FILE* pseudo_file) {
    t_lista_instrucciones* lista_instrucciones = crear_lista_instrucciones();

    size_t read; // Cantidad de caracteres leídos
	size_t len = 0; // Tamaño de la línea leída
	char* line = NULL; // Línea leída

	while((read = getline(&line, &len, pseudo_file)) != -1) {
        // Por cada línea que leo, obtengo los tokens, armo la instrucción con sus parámetros y la agrego a la lista 
		t_list* lines = list_create();
		char* t = strtok(line, "\n"); 
		char** tokens = string_split(t, " "); 

        // Agregar a la lista el indentificador y los parámetros de la instrucción 
		list_add(lines, (void*) tokens[0]);
		log_info(logger_consola, "Se agregó el operador %s", tokens[0]); 

		int i = 1;
        
        while(tokens[i] != NULL) { 
			list_add(lines, (void*) atoi(tokens[i]));
			log_info(logger_consola, "Se agregó el parametro %d", atoi(tokens[i])); 
            i++;
        }

        t_identificador identificador = mapear_identificador(list_get(lines, 0));

        // Si hubo algun problema de sintaxis o algun identificador inválido, terminar ejecución del programa
        if (identificador == -1) { 
            log_error(logger_consola, "Se encontró un identificador inválido.");
            return NULL;
        }

        t_instruccion* instruccion = crear_instruccion(identificador);
		agregar_parametros(identificador, instruccion, lines);
		agregar_instruccion_a_lista(lista_instrucciones, instruccion); 

    }

    return lista_instrucciones;
}