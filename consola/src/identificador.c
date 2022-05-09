#include "identificador.h"

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
