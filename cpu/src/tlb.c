#include "tlb.h"
#include "cpu.h"

void inicializar_tlb(void){
		tlb = list_create();
		generar_entradas_tlb();
}

void elegir_algoritmo(char* algoritmo) {
    
    if(strcmp(algoritmo, "FIFO") == 0) {
        algoritmo_elegido = FIFO;
        log_info(logger, "Se eligio el algoritmo FIFO");
    }

    if((strcmp(algoritmo,"LRU") == 0)) {
        algoritmo_elegido = LRU;
        log_info(logger, "Se eligio el algoritmo LRU");
    }
}

void generar_entradas_tlb(void){

	for (size_t i = 0; i < cpu_config->entradas_tlb; i++)
	{
		t_entrada_tlb* entrada = malloc(sizeof(t_entrada_tlb));
		entrada -> pagina = -1;
		entrada -> marco = 0;
		entrada -> instante_carga = 0;
        entrada -> instante_ultima_referencia = 0;


		list_add(tlb, entrada);
		log_info(logger, "Se creó entrada de tlb %d", i);
	}

}

 // Función para liberar la lista de entradas
 void liberar_tlb(void){
	 list_destroy(tlb);
 }

void ordenar_tlb(t_lista_entradas* lista_entradas) {

	// log_warning(logger, "Entradas de la TLB ANTES DE ORDENAR:");
	// for (size_t i = 0; i < list_size(lista_entradas); i++)
	// {
	// 	t_entrada_tlb* e = list_get(lista_entradas, i);
	// 	log_error(logger, "Entrada %d: pag %d,marco %d - TSR:%f - TSC:%f", i, e -> pagina, e -> marco, e -> instante_ultima_referencia, e -> instante_carga);
	// }

    if (algoritmo_elegido == LRU) {
        list_sort(lista_entradas, (void*) *(algoritmo_LRU));
    }

	else {
		list_sort(lista_entradas, (void*) *(algoritmo_FIFO));
	}

	// log_warning(logger, "Entradas de la TLB DESPUES DE ORDENAR:");
	// for (size_t i = 0; i < list_size(lista_entradas); i++)
	// {
	// 	t_entrada_tlb* e = list_get(lista_entradas, i);
	// 	log_error(logger, "Entrada %d: pag %d,marco %d - TSR:%f - TSC:%f", i, e -> pagina, e -> marco, e -> instante_ultima_referencia, e -> instante_carga);
	// }
}


bool algoritmo_LRU(t_entrada_tlb* entrada1, t_entrada_tlb* entrada2) {
	return (entrada1 -> instante_ultima_referencia <= entrada2 -> instante_ultima_referencia);
}


bool algoritmo_FIFO(t_entrada_tlb* entrada1, t_entrada_tlb* entrada2) {
	return (entrada1 -> instante_carga >= entrada2 -> instante_carga);
}


//Aca no se si conviene eliminar las entradas (limpiar_tlb) o simplemente iterar la lista y poner todo en 0 (limpiar_tlb2)
void limpiar_tlb(t_lista_entradas* lista_entradas){
	list_clean(lista_entradas);
}

void limpiar_tlb2(void){
	for (uint32_t j=0;j<list_size(tlb);j++) {
		t_entrada_tlb* entrada = list_get(tlb, j);
		entrada -> pagina = -1;
		entrada -> marco = 0;
		entrada -> instante_carga = 0;
        entrada -> instante_ultima_referencia = 0;
	}
}

//Busca la entrada y si la encuentra devuleve el marco y actualiza instante de referencia

int32_t buscar_entrada_tlb(t_lista_entradas* lista_entradas, uint32_t numero_pagina){
	for (uint32_t j=0;j<list_size(lista_entradas);j++) {
			t_entrada_tlb* entrada = list_get(lista_entradas, j);
			if (entrada -> pagina == numero_pagina) {
				if (algoritmo_elegido == LRU) {
					entrada -> instante_ultima_referencia = time(NULL);
				}
				return entrada -> marco;
			}
	}
	return -1;
}


//se fija que entradas no estan ocupadas y las agrega la entrada ahi, si no hay ninguna entonces reemplaza la ultima entrada que esta ordenada segun el algoritmo
// finalmente ordena la lista 
void agregar_entrada_tlb(t_lista_entradas* lista_entradas, uint32_t numero_pagina, uint32_t numero_marco){

	// log_error(logger, "Entradas de la TLB ANTES de agregar %d|%d:", numero_pagina, numero_marco);
	// for (size_t i = 0; i < list_size(lista_entradas); i++)
	// {
	// 	t_entrada_tlb* e = list_get(lista_entradas, i);
	// 	log_error(logger, "Entrada %d: pag %d,marco %d", i, e -> pagina, e -> marco);
	// }
	

	for (uint32_t j=0;j<list_size(lista_entradas);j++) {
		t_entrada_tlb* entrada = list_get(lista_entradas, j);
		if (entrada -> pagina == -1) {
			entrada -> pagina = numero_pagina;
			entrada -> marco = numero_marco;
			entrada -> instante_carga = time(NULL);
			log_warning(logger, "Se cargó entrada con instante de carga %f", entrada -> instante_carga);
			entrada -> instante_ultima_referencia = time(NULL);
			log_warning(logger, "Se cargó entrada con instante de ultima referencia %f", entrada -> instante_ultima_referencia);
			ordenar_tlb(lista_entradas);
			return;
		}
	}

	t_entrada_tlb* entrada = list_get(lista_entradas, list_size(lista_entradas) - 1);

	entrada -> pagina = numero_pagina;
	entrada -> marco = numero_marco;
	entrada -> instante_carga = time(NULL);
	entrada -> instante_ultima_referencia = time(NULL);
	
	ordenar_tlb(lista_entradas);

	// log_error(logger, "Entradas de la TLB:");
	// for (size_t i = 0; i < list_size(lista_entradas); i++)
	// {
	// 	t_entrada_tlb* e = list_get(lista_entradas, i);
	// 	log_error(logger, "Entrada %d: pag %d,marco %d", i, e -> pagina, e -> marco);
	// }
}
