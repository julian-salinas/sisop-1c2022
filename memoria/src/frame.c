#include "frame.h"

void generarFrames(void* memoria, uint32_t tamanio_memoria, uint32_t tamanio_frame) {
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


uint32_t leer_contenido_frame_n(uint32_t nro_frame) {
	uint32_t dato;
	sem_wait(mutex_memoria);
		t_frame* frame = get_frame(nro_frame);
		memcpy(&dato, frame -> puntero_frame, sizeof(uint32_t));
	sem_post(mutex_memoria);
	return dato;
}


void escribir_frame(t_frame* frame, uint32_t dato) {
	sem_wait(mutex_memoria);
		memcpy(frame -> puntero_frame, &dato, sizeof(uint32_t));
	sem_post(mutex_memoria);
}


void escribir_frame_n(uint32_t nro_frame, uint32_t dato) {
	sem_wait(mutex_memoria);
		t_frame* frame = get_frame(nro_frame);
		memcpy(frame -> puntero_frame, &dato, sizeof(uint32_t));
	sem_post(mutex_memoria);
}


void liberar_frame(t_frame* frame) {
	frame -> bit_ocupado = 0;
}


void liberar_frame_n(uint32_t nro_frame) {
	t_frame* frame = get_frame(nro_frame);
	frame -> bit_ocupado = 0;
}

void ocupar_frame_n(uint32_t nro_frame){
	t_frame* frame = get_frame(nro_frame);
	frame -> bit_ocupado = 1;
}


uint32_t leer_direccion_memoria(int32_t direccion_fisica) {
	uint32_t dato;
	sem_wait(mutex_memoria);
		memcpy(&dato, memoria + direccion_fisica, sizeof(uint32_t));
	sem_post(mutex_memoria);
	return dato;
} 


void escribir_direccion_memoria(int32_t direccion_fisica, uint32_t dato) {
	sem_wait(mutex_memoria);
		memcpy(memoria + direccion_fisica, &dato, sizeof(uint32_t));;
	sem_post(mutex_memoria);
}
