#include "utils-memoria.h"

void ini_config(void)
{
	t_config* config_memoria = config_create("cfg/memoria.config");

	if (config_memoria == NULL) {
		perror("Archivo de configuracion de memoria no encontrado.");
		exit(EXIT_FAILURE);
	}

	config_data.puerto_escucha = config_get_int_value(config_memoria, "PUERTO_ESCUCHA");
	config_data.tam_memoria = config_get_int_value(config_memoria, "TAM_MEMORIA");
	config_data.tam_pagina = config_get_int_value(config_memoria, "TAM_PAGINA");
	config_data.paginas_por_tabla = config_get_int_value(config_memoria, "PAGINAS_POR_TABLA");
	config_data.retardo_memoria = config_get_int_value(config_memoria, "RETARDO_MEMORIA");
	config_data.algoritmo_reemplazo = config_get_string_value(config_memoria, "ALGORITMO_REEMPLAZO");
	config_data.marcos_por_proceso = config_get_int_value(config_memoria, "MARCOS_POR_PROCESO");
	config_data.retardo_swap = config_get_int_value(config_memoria, "RETARDO_SWAP");
	config_data.path_swap = config_get_string_value(config_memoria, "PATH_SWAP");

}

