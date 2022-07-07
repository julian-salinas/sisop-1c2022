#include "tlb.h"
#include "cpu.h"

t_tlb* inicializar_tlb(void){
    t_tlb* tlb = malloc(sizeof(t_tlb));
	tlb -> tlb = malloc(cpu_config->entradas_tlb*sizeof(t_entrada_tlb));

	return tlb;
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

t_lista_entradas* generarEntradasTlb(t_tlb* tlb){

	void* aux = tlb -> tlb;
	int indice = 0;
	t_lista_entradas* lista_entradas = list_create();

	for (size_t i = 0; i < cpu_config->entradas_tlb; i++)
	{
		t_entrada_tlb* entrada = malloc(sizeof(t_entrada_tlb));
		entrada -> pagina = 0;
		entrada -> marco = 0;
		entrada -> instante_carga = 0;
        entrada -> instante_ultima_referencia = 0;


		list_add(lista_entradas, entrada);
		log_info(logger, "Se creó entrada de tlb %d", i);
	}
	return lista_entradas;
}

void ordenar_tlb(t_lista_entradas* lista_entradas) {
    if (algoritmo_elegido == LRU) {
        list_sort(lista_entradas, (void*) *(algoritmo_LRU));
    }
}


bool algoritmo_LRU(t_entrada_tlb* entrada1, t_entrada_tlb* entrada2) {
	return (entrada1 -> instante_ultima_referencia <= entrada2 -> instante_ultima_referencia);
}

void limpiar_tlb(t_lista_entradas* lista_entradas){
	list_clean(lista_entradas);
}

uint32_t buscar_entrada_tlb(t_lista_entradas* lista_entradas, uint32_t numero_pagina){
	numero_pagina_buscada = numero_pagina;
	t_entrada_tlb* entrada_buscada = list_find(lista_entradas,  (void*) _soy_el_numero_buscado);
	if(entrada_buscada != NULL){
		return entrada_buscada -> marco;
	}
	else{
		return 0;
	}
}


//capaz en esta funcion combiene pasarle numero de pagina y marco y que te genere la entrada de una
void reemplazar_entrada_tlb(t_lista_entradas* lista_entradas, t_entrada_tlb* entrada){
	list_replace(lista_entradas, cpu_config->entradas_tlb - 1, entrada);
}



int _soy_el_numero_buscado(t_entrada_tlb* p) {
    return p -> pagina == numero_pagina_buscada;
}

 