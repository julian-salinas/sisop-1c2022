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

	// Valor que va a servir para el uso del algoritmo clock, es el puntero a sig. página a analizar
	contador_clock = 0;

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


void generarFrames(void* memoria, uint32_t tamanio_memoria, uint32_t tamanio_frame){
	int cantidad_frames = tamanio_memoria / tamanio_frame;
	void* aux = memoria;
	int contador_frame = 0;

	mutex_lista_frames = malloc(sizeof(sem_t));
	sem_init(mutex_lista_frames, 0, 1);

	lista_frames = list_create();

	for (size_t i = 0; i < cantidad_frames; i++)
	{
		t_frame* frame = malloc(sizeof(t_frame));
		frame -> bit_ocupado = 0;
		frame -> puntero_frame = aux;
		frame -> numero_frame = contador_frame;

		list_add(lista_frames, frame);
		log_info(logger, "Se creó el frame %d que apunta a la dirección %p", frame -> numero_frame, frame -> puntero_frame);

		aux += tamanio_frame;
		contador_frame ++;
	}	
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


t_frame* get_frame_libre(void) {
	for(uint32_t i = 0; i < list_size(lista_frames); i++){
		if(frame_esta_libre(i)) return get_frame(i);
	}

	// No hay frames libres
    return NULL;
}

int get_posicion_frame_libre(void) {
	for(uint32_t i = 0; i < list_size(lista_frames); i++){
		if(frame_esta_libre(i)) return i;
	}

	// No hay frames libres
    return 0;
}


int frame_esta_libre(uint32_t posicion_frame) {
	return get_frame(posicion_frame) -> bit_ocupado == 0;
}


t_frame* get_frame(uint32_t posicion_frame) {
	sem_wait(mutex_lista_frames);
		t_frame* frame = (t_frame*) list_get(lista_frames, posicion_frame);
	sem_post(mutex_lista_frames);

	return frame;
}


int algoritmo_clock_mejorado(void) {

	t_frame* frame;

    while(1) {

		// Primera vuelta: ver si encontramos frames que no estan en uso y no fueron modificados
        for (uint32_t i = 0; i < list_size(lista_frames); i++) {

			// Obtener frame a analizar
			frame = (t_frame*) get_frame(contador_clock);
            
			// Si se cumple bit_ocupado == 0 y bit_modificado == 0, devolver frame
			if (!frame -> bit_ocupado && !frame -> bit_modificado) {
                
				// Avanzar para la proxima vez que se use
				aumentar_contador_clock();

				// Devolver frame encontrado
                return contador_clock;
            }

			// Avanzar y volver a analizar
			aumentar_contador_clock();       
		}

		// Segunda vuelta: ver si encontramos frames que están en uso pero no fueron modificados
        for (uint32_t i = 0; i < list_size(lista_frames); i++) {
			
			// Obtener frame a analizar
			frame = (t_frame*) get_frame(contador_clock);

			// Si no está modificado, retornarlo
            if (!frame -> bit_modificado){
				// Avanzar para la proxima vez que se use
                aumentar_contador_clock();
                
				return contador_clock;
            }

			// Si está modificado, setear su bit de ocupado en 0 (independientemente su valor anterior)
            frame -> bit_ocupado = 0;

			// Avanzar y volver a analizar
            aumentar_contador_clock();
        }
    }
}


int algoritmo_clock(void) {

	t_frame* frame;

	while(1) {
		for (uint32_t i = 0; i < list_size(lista_frames); i++) {
			frame = (t_frame*) get_frame(contador_clock);  // Obtener frame a analizar
			
			// Si se cumple bit_ocupado == 0, devolver frame
			if (!frame -> bit_ocupado) {
				aumentar_contador_clock();  // Avanzar para la proxima vez que se use
				return contador_clock;  // Devolver posicion del frame encontrado
			}

			// Avanzar y volver a analizar
			aumentar_contador_clock();       
		}
	}
}


void aumentar_contador_clock(void) {
	if (contador_clock + 1 >= list_size(lista_frames)) {
		contador_clock = 0;
	} 
	else {
		contador_clock++;
	}
}
