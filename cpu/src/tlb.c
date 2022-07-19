#include "tlb.h"
#include "cpu.h"


void inicializar_tlb(void)
{
	tlb = list_create();
	generar_entradas_tlb();
}


void elegir_algoritmo(char *algoritmo)
{
	if (strcmp(algoritmo, "FIFO") == 0)
	{
		algoritmo_elegido = FIFO;
		log_info(logger, "Se eligio el algoritmo FIFO");
	}

	if ((strcmp(algoritmo, "LRU") == 0))
	{
		algoritmo_elegido = LRU;
		log_info(logger, "Se eligio el algoritmo LRU");
	}
}


void generar_entradas_tlb(void) 
{
	for (size_t i = 0; i < cpu_config->entradas_tlb; i++)
	{
		t_entrada_tlb *entrada = malloc(sizeof(t_entrada_tlb));
		entrada->pagina = -1;
		entrada->marco = 0;
		entrada->instante_carga = 0;
		entrada->instante_ultima_referencia = 0;

		list_add(tlb, entrada);
		log_info(logger, "Se creó entrada de tlb %d", i);
	}
}


// Función para liberar la lista de entradas
void liberar_tlb(void) 
{
	list_destroy(tlb);
}


void ordenar_tlb(void) 
{
	if (algoritmo_elegido == LRU)
	{
		list_sort(tlb, (void *)*(algoritmo_LRU));
	}

	else
	{
		list_sort(tlb, (void *)*(algoritmo_FIFO));
	}

	t_entrada_tlb* entrada;
	for (size_t i = 0; i < list_size(tlb); i++)
	{
		entrada = list_get(tlb, i);
		log_info(logger, "Entrada - Marco|Pagina %d|%d - IUR %u - IC %u", entrada -> marco, entrada -> pagina,entrada -> instante_ultima_referencia, entrada -> instante_carga);
	}
	
}


bool algoritmo_LRU(t_entrada_tlb *entrada1, t_entrada_tlb *entrada2)
{
	return (entrada1->instante_ultima_referencia >= entrada2->instante_ultima_referencia);
}


bool algoritmo_FIFO(t_entrada_tlb *entrada1, t_entrada_tlb *entrada2)
{
	return (entrada1->instante_carga >= entrada2->instante_carga);
}


//Aca no se si conviene eliminar las entradas (limpiar_tlb) o simplemente iterar la lista y poner todo en 0 (limpiar_tlb2)
void limpiar_tlb(void) 
{
	list_clean_and_destroy_elements(tlb, (void*)(*free));
}


void limpiar_tlb2(void)
{
	for (uint32_t j = 0; j < list_size(tlb); j++)
	{
		t_entrada_tlb *entrada = list_get(tlb, j);
		entrada->pagina = -1;
		entrada->marco = 0;
		entrada->instante_carga = 0;
		entrada->instante_ultima_referencia = 0;
	}
}


// Busca la entrada y si la encuentra devuleve el marco y actualiza instante de referencia
int32_t buscar_entrada_tlb(uint32_t numero_pagina) 
{
	for (uint32_t j = 0; j < list_size(tlb); j++)
	{
		t_entrada_tlb* entrada = (t_entrada_tlb*) list_get(tlb, j);

		if (entrada->pagina == numero_pagina)
		{
			log_warning(logger, "Se encontró entrada marco|pagina %d|%d", entrada -> marco, entrada -> pagina);

			if (algoritmo_elegido == LRU)
			{
				log_info(logger, "Anterior TS: %u", entrada -> instante_ultima_referencia);
				entrada -> instante_ultima_referencia = time(NULL);
				log_info(logger, "Actual TS: %u", entrada -> instante_ultima_referencia);
				ordenar_tlb();
			}
			return entrada->marco;
		}
	}
	return -1;
}


// Se fija que entradas no estan ocupadas y las agrega la entrada ahi, si no hay ninguna
// entonces reemplaza la ultima entrada que esta ordenada segun el algoritmo
// finalmente ordena la lista
void agregar_entrada_tlb(uint32_t numero_pagina, uint32_t numero_marco)
{

	time_t quehoraeee = time(NULL);
	for (uint32_t j = 0; j < list_size(tlb); j++)
	{
		t_entrada_tlb *entrada = list_get(tlb, j);
		if (entrada->pagina == -1)
		{
			entrada->pagina = numero_pagina;
			entrada->marco = numero_marco;
			entrada->instante_carga = quehoraeee;
			entrada->instante_ultima_referencia = quehoraeee;
			ordenar_tlb();
			return;
		}
	}

	t_entrada_tlb *entrada = list_get(tlb, list_size(tlb) - 1);

	entrada->pagina = numero_pagina;
	entrada->marco = numero_marco;
	entrada->instante_carga = quehoraeee;
	entrada->instante_ultima_referencia = quehoraeee;

	ordenar_tlb();
}
