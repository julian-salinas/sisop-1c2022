#include "tlb.h"
#include "cpu.h"

t_tlb* inicializar_tlb(void){
    t_tlb* tlb = malloc(sizeof(t_tlb));
	tlb -> tlb = malloc(cpu_config->entradas_tlb*sizeof(t_entrada_tlb));

	return tlb;
}

void generarEntradasTlb(t_tlb* tlb){

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

}