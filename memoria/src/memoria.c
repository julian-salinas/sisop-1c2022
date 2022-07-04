#include "memoria.h"


int main(void) {

	// Inicializar config
	memoria_config = ini_memoria_config("cfg/memoria.config");

	// Crear logger
	logger = log_create("cfg/memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Memoria iniciada");

	// Inicializar estructuras de tablas de páginas
	inicializar_tablas_de_paginas();
	
	// Inicializar memoria
	void* memoria = inicializar_memoria();

	// Elegir entre CLOCK y CLOCK_MEJORADO
	elegir_algoritmo_reemplazo(memoria_config -> algoritmo_reemplazo);

	diccionario_clocks = dictionary_create();

	generarFrames(memoria, memoria_config -> tamanio_memoria, memoria_config -> tamanio_pagina);
	
	int server_fd = iniciar_servidor(logger, "memoria", memoria_config -> ip_memoria, memoria_config -> puerto_escucha);	
	
	log_info(logger, "Memoria lista para recibir al cliente");

	while(server_listen(logger, "memoria", server_fd, (void*)(*procesar_conexion)));

	terminar_programa("Memoria", server_fd, logger);
	
	return EXIT_SUCCESS;
}


void* inicializar_memoria(void) {
	void* memoria = malloc(memoria_config -> tamanio_memoria);

	return memoria;
}


void elegir_algoritmo_reemplazo(char* algoritmo) {

	if (strcmp(algoritmo, "CLOCK") == 0) {
		algoritmo_reemplazo = CLOCK;
		log_info(logger, "Se eligió el algoritmo de reemplazo CLOCK");
	}

	else if (strcmp(algoritmo, "CLOCK-M")) {
		algoritmo_reemplazo = CLOCK_MEJORADO;
		log_info(logger, "Se eligió el algoritmo de reemplazo CLOCK MEJORADO");
	}
}


int algoritmo_clock_mejorado(int id_proceso) {

	t_list* entradas_en_memoria = get_entradas_en_memoria_proceso(id_proceso);
	int contador_clock_proceso = (int) dictionary_get(diccionario_clocks, int_a_string(id_proceso));

	while (1) {
		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, i); // Posible víctima
			contador_clock_proceso = aumentar_contador_clock(contador_clock_proceso, list_size(entradas_en_memoria));
			if ((!entrada -> bit_uso) && (!entrada -> bit_modificado)) {
				swappear(entrada);
				return entrada -> nro_frame;
			}
		}

		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, i); // Posible víctima
			contador_clock_proceso = aumentar_contador_clock(contador_clock_proceso, list_size(entradas_en_memoria));
			if (!entrada -> bit_uso) {
				swappear(entrada);
				return entrada -> nro_frame;
			}
			entrada -> bit_uso = 0;
		}
	}
}


int algoritmo_clock(int id_proceso) {

	t_list* entradas_en_memoria = get_entradas_en_memoria_proceso(id_proceso);
	int contador_clock_proceso = (int) dictionary_get(diccionario_clocks, int_a_string(id_proceso));

	while (1) {
		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, i); // Posible víctima
			aumentar_contador_clock(contador_clock_proceso, list_size(entradas_en_memoria));
			if (!entrada -> bit_uso) {
				swappear(entrada);
				return entrada -> nro_frame;
			}
		}
	}
}


int aumentar_contador_clock(int contador_actual, int max) {
	if (contador_actual + 1 >= list_size(lista_frames)) {
		return 0;
	} 
	else {
		return contador_actual + 1;
	}
}
