#include "memoria.h"

int main(void) {

	// Inicializar config
	memoria_config = ini_memoria_config("cfg/memoria.config");

	// Crear logger
	logger = log_create("cfg/memoria.log", "memoria", 1, LOG_LEVEL_TRACE);
	log_info(logger,"Memoria iniciada");

	// Inicializar estructuras de tablas de páginas
	inicializar_tablas_de_paginas();
	log_info(logger, "Se inicializaron las tablas de páginas EXITOSAMENTE :smirk:");
	
	// Inicializar semáforos de memoria
	mutex_swap = malloc(sizeof(sem_t));
	sem_init(mutex_swap, 0, 1);

	mutex_cantidad_accesos_swap = malloc(sizeof(sem_t));
	sem_init(mutex_cantidad_accesos_swap, 0, 1);

	int cantidad_accesos_swap = 0;

	mutex_memoria = malloc(sizeof(sem_t));
	sem_init(mutex_memoria, 0, 1);

	// Inicializar memoria
	memoria = inicializar_memoria();
	diccionario_swap = dictionary_create();
	log_info(logger, "Se inicializó puntero a memoria EXITOSAMENTE :smirk:");

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

	else if (strcmp(algoritmo, "CLOCK-M") == 0) {
		algoritmo_reemplazo = CLOCK_MEJORADO;
		log_info(logger, "Se eligió el algoritmo de reemplazo CLOCK MEJORADO");
	}
}


t_entrada_segundo_nivel* algoritmo_clock_mejorado(uint32_t PID) {

	t_list* entradas_en_memoria = get_entradas_en_memoria_proceso(PID);
	char* str_pid = int_a_string(PID);
	int contador_clock_proceso = (int) dictionary_get(diccionario_clocks, str_pid);

	if (contador_clock_proceso >= memoria_config -> marcos_por_proceso) {
		contador_clock_proceso = 0;
	} 

	while (1) {
		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, contador_clock_proceso); // Posible víctima
			contador_clock_proceso = aumentar_contador_clock(contador_clock_proceso, memoria_config -> marcos_por_proceso);
			if ((!entrada -> bit_uso) && (!entrada -> bit_modificado)) {
				log_trace(logger, "Se va a reemplazar la entrada con la página %d", entrada -> nro_pagina);
				dictionary_put(diccionario_clocks, str_pid, contador_clock_proceso);
				free(str_pid);
				list_destroy(entradas_en_memoria);
				return entrada;
			}
		}

		contador_clock_proceso = 0;

		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, contador_clock_proceso); // Posible víctima
			contador_clock_proceso = aumentar_contador_clock(contador_clock_proceso, memoria_config -> marcos_por_proceso);
			if (!entrada -> bit_uso) {
				log_trace(logger, "Se va a reemplazar la entrada con la página %d", entrada -> nro_pagina);
				dictionary_put(diccionario_clocks, str_pid, contador_clock_proceso);
				free(str_pid);
				list_destroy(entradas_en_memoria);
				return entrada;
			}

			entrada -> bit_uso = 0;
		}

		contador_clock_proceso = 0;

	}
}


t_entrada_segundo_nivel* algoritmo_clock(uint32_t PID) {

	t_list* entradas_en_memoria = get_entradas_en_memoria_proceso(PID);
	char* str_pid = int_a_string(PID);
	int contador_clock_proceso = (int) dictionary_get(diccionario_clocks, str_pid);

	while (1) {
		for (uint32_t i = 0; i < list_size(entradas_en_memoria); i++) {
			t_entrada_segundo_nivel* entrada = list_get(entradas_en_memoria, contador_clock_proceso); // Posible víctima
			contador_clock_proceso = aumentar_contador_clock(contador_clock_proceso, memoria_config -> marcos_por_proceso);
			if (!entrada -> bit_uso) {
				dictionary_put(diccionario_clocks, str_pid, contador_clock_proceso);
				free(str_pid);
				log_trace(logger, "Se va a reemplazar la entrada con la página %d", entrada -> nro_pagina);
				list_destroy(entradas_en_memoria);
				return entrada;
			}
			entrada -> bit_uso = 0;
		}
		contador_clock_proceso = 0;
	}
}


int aumentar_contador_clock(int contador_actual, int max) {
	if ((contador_actual + 1) >= max) {
		return 0;
	} 
	else {
		return contador_actual + 1;
	}
}


t_entrada_segundo_nivel* obtener_entrada_por_DF(int32_t direccion_fisica){
	t_tabla_segundo_nivel* tabla;
    t_entrada_segundo_nivel* entrada;
	char* str;
	
	sem_wait(mutex_tablas_segundo_nivel);
		int dict_size = dictionary_size(tablas_segundo_nivel);
	sem_post(mutex_tablas_segundo_nivel);

	for (size_t i = 0; i < dict_size; i++)
	{
		str = int_a_string(i);

		sem_wait(mutex_tablas_segundo_nivel);
			tabla = (t_tabla_segundo_nivel*) dictionary_get(tablas_segundo_nivel, str);
		sem_post(mutex_tablas_segundo_nivel);

		free(str);

        for (size_t i = 0; i < list_size(tabla -> entradas); i++)
        {
            entrada = ((t_entrada_segundo_nivel*) list_get(tabla -> entradas, i));

			if ((entrada -> nro_frame == -1) || (!entrada -> bit_presencia)) {
				continue;
			}

            int marco = entrada -> nro_frame;
            int inicio = marco * memoria_config -> tamanio_pagina;
            int final = inicio + memoria_config -> tamanio_pagina;

			if (i == 0) {
				final-=1;
			}

            if ((inicio <= direccion_fisica) && (direccion_fisica <= final)){
                return entrada;
            }
        }
        
	}

	return NULL;
}