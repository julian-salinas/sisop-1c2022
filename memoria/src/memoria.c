#include "memoria.h"

int main(void) {

	// Inicializar config
	memoria_config = ini_memoria_config("cfg/memoria.config");

	// Crear logger
	logger = log_create("cfg/memoria.log", "memoria", 1, LOG_LEVEL_DEBUG);
	log_info(logger,"Memoria iniciada");

	// Inicializar estructuras de tablas de páginas
	inicializar_tablas_de_paginas();
	
	// Inicializar semáforos de memoria
	mutex_swap = malloc(sizeof(sem_t));
	sem_init(mutex_swap, 0, 1);

	mutex_memoria = malloc(sizeof(sem_t));
	sem_init(mutex_memoria, 0, 1);

	// Inicializar memoria
	memoria = inicializar_memoria();
	diccionario_swap = dictionary_create();

	// Elegir entre CLOCK y CLOCK_MEJORADO
	elegir_algoritmo_reemplazo(memoria_config -> algoritmo_reemplazo);

	diccionario_clocks = dictionary_create();

	generarFrames(memoria, memoria_config -> tamanio_memoria, memoria_config -> tamanio_pagina);

	int server_fd = crear_socket_servidor(memoria_config -> ip_memoria, memoria_config -> puerto_escucha, logger);

	log_info(logger, "Memoria lista para recibir al cliente");

	while(server_listen(logger, "memoria", server_fd, (void*)(*procesar_conexion)));

	terminar_programa("Memoria", server_fd, logger);
	
	return EXIT_SUCCESS;
}


void* inicializar_memoria(void) {
	void* tmp = malloc(memoria_config -> tamanio_memoria);

	return tmp;
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


t_entrada_segundo_nivel* algoritmo_clock_mejorado(uint32_t PID) {

	t_list* entradas_en_memoria = get_entradas_en_memoria_proceso(PID);
	int contador_clock_proceso = (int) dictionary_get(diccionario_clocks, int_a_string(PID));

	while (1) {
		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, i); // Posible víctima
			contador_clock_proceso = aumentar_contador_clock(contador_clock_proceso, list_size(entradas_en_memoria));
			if ((!entrada -> bit_uso) && (!entrada -> bit_modificado)) {
				swappear(PID, entrada);
				return entrada;
			}
		}

		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, i); // Posible víctima
			contador_clock_proceso = aumentar_contador_clock(contador_clock_proceso, list_size(entradas_en_memoria));
			if (!entrada -> bit_uso) {
				swappear(PID, entrada);
				return entrada;
			}
			entrada -> bit_uso = 0;
		}
	}
}


t_entrada_segundo_nivel* algoritmo_clock(uint32_t PID) {

	t_list* entradas_en_memoria = get_entradas_en_memoria_proceso(PID);
	int contador_clock_proceso = (int) dictionary_get(diccionario_clocks, int_a_string(PID));

	while (1) {
		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, i); // Posible víctima
			aumentar_contador_clock(contador_clock_proceso, list_size(entradas_en_memoria));
			if (!entrada -> bit_uso) {
				swappear(PID, entrada);
				return entrada;
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
