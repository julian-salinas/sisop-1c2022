#include "tlb.h"
#include "cpu.h"

void inicializar_tlb(t_tlb* tlb){

	int indice = 0;
	t_lista_entradas* lista_entradas = list_create();

	for (size_t i = 0; i < cpu_config->entradas_tlb; i++)
	{
		t_entrada_tlb* entrada = malloc(sizeof(t_entrada_tlb));
		entrada -> pagina = -1;
		entrada -> marco = -1;
		entrada -> instante_carga = time(NULL);
        entrada -> instante_ultima_referencia = time(NULL);

		list_add(lista_entradas, entrada);
		log_info(logger, "Se creó entrada de tlb %d, instante_carga %d", i, entrada->instante_carga);
	}

	tlb = lista_entradas;
}

int estaLlena(t_tlb* tlb){
	int i=0;
	int ocupado=1;
	t_entrada_tlb* entrada;
	do {
		entrada = (t_entrada_tlb*) list_get(tlb, i);
		if (entrada->pagina == -1) {
				ocupado=0;
				}
		i++;
	}while((ocupado) && (i<cpu_config->entradas_tlb));

	return ocupado;
}



void reemplazoFIFO(t_tlb* tlb, int pagina, int marco){

	//buscar de la lista la entrada que su instante_carga sea menor
	t_entrada_tlb* entrada = list_get_minimum(tlb, (void*) *(primeraEntrada));
    entrada->pagina = pagina;
	entrada->marco = marco;
	entrada->instante_carga = time(NULL);
	entrada -> instante_ultima_referencia = time(NULL);

}

//para aplicar FIFO: el instante de carga es un timestamp expresado en segundos (int)
// por lo cual el primero entrado tendra un int menor
bool primeraEntrada(t_entrada_tlb* entrada1, t_entrada_tlb* entrada2){
	  return (entrada1->instante_carga <= entrada2->instante_carga);
}

void agregarEntradaTlb(int pagina, int marco){

	//algoritmo FIFO
	if((strcmp(cpu_config->reemplazo_tlb,"FIFO")) == 0){
		reemplazoFIFO(tlb, pagina, marco);
	}

	//algoritmo LRU
	if((strcmp(cpu_config->reemplazo_tlb,"LRU")) == 0){
		//TODO
	}


}