#include "frame.h"


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


t_lista_frames* get_entradas_en_memoria_proceso(uint32_t id_proceso) {
	t_list* entradas_en_memoria = list_create();
	t_tabla_primer_nivel* tp_lvl1 = get_tabla_primer_nivel(id_proceso);

	for (uint32_t i = 0; i < memoria_config -> paginas_por_tabla; i++) {
		t_tabla_segundo_nivel* tp_lvl2 = get_tabla_segundo_nivel((uint32_t) list_get(tp_lvl1 -> entradas, i));

		for (uint32_t j = 0; j < memoria_config -> paginas_por_tabla; j++) {
			t_entrada_segundo_nivel* entrada = list_get(tp_lvl2 -> entradas, j);
			if (entrada -> bit_presencia) {
				list_add(entradas_en_memoria, entrada);
			}
		}
	}

	return entradas_en_memoria;
}