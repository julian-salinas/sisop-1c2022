#include "tabla_paginas.h"

void inicializar_tablas_de_paginas(void) {
    tablas_primer_nivel = dictionary_create();
    tablas_segundo_nivel = dictionary_create();

    mutex_tablas_primer_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_tablas_primer_nivel, 0, 1);

    mutex_tablas_segundo_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_tablas_segundo_nivel, 0, 1);    

    mutex_id_tablas = malloc(sizeof(sem_t));
    sem_init(mutex_id_tablas, 0, 1);

    id_tablas_segundo_nivel = 0;
}


t_tabla_primer_nivel* crear_tabla_primer_nivel(int PID) {
    t_tabla_primer_nivel* tmp = malloc(sizeof(t_tabla_primer_nivel));
    
    tmp -> id_tabla = PID;
    tmp -> entradas = list_create();
    char* str_id = int_a_string(PID);

    sem_wait(mutex_tablas_primer_nivel);
        dictionary_put(tablas_primer_nivel, str_id, (void*) tmp);
    sem_post(mutex_tablas_primer_nivel);

    free(str_id);

    return tmp;
}


t_tabla_segundo_nivel* crear_tabla_segundo_nivel(void) {
    t_tabla_segundo_nivel* tmp = malloc(sizeof(t_tabla_segundo_nivel));

    tmp -> entradas = list_create();

    sem_wait(mutex_id_tablas);
        char* str_id = int_a_string(id_tablas_segundo_nivel);
        tmp -> id_tabla = id_tablas_segundo_nivel;
        id_tablas_segundo_nivel++;
    sem_post(mutex_id_tablas);


    sem_wait(mutex_tablas_segundo_nivel);
        dictionary_put(tablas_segundo_nivel, str_id, (void*) tmp);
    sem_post(mutex_tablas_segundo_nivel);
    
    free(str_id);
    
    return tmp;    
}


void destruir_entrada_segundo_nivel(t_entrada_segundo_nivel* entrada) {
    free(entrada);
}


void destruir_tabla_primer_nivel(t_tabla_primer_nivel* tabla) {
    list_destroy_and_destroy_elements(tabla -> entradas, (void*) *(destruir_entrada_segundo_nivel));
    free(tabla);
}


void destruir_tabla_segundo_nivel(t_tabla_segundo_nivel* tabla) {
    list_destroy(tabla -> entradas);
    free(tabla);
} 


t_entrada_segundo_nivel* crear_entrada_segundo_nivel(int32_t nro_pagina, int32_t nro_frame) {
    t_entrada_segundo_nivel* tmp = malloc(sizeof(t_entrada_segundo_nivel));
    
    tmp -> nro_pagina = nro_pagina;
    tmp -> nro_frame = nro_frame;
    tmp -> bit_presencia = 0;
    tmp -> bit_uso = 0;
    tmp -> bit_modificado = 0;
    
    return tmp;     
}


void agregar_entrada_primer_nivel(t_tabla_primer_nivel* tabla, int32_t id_tabla_segundo_nivel) {
    list_add(tabla -> entradas, (void*) id_tabla_segundo_nivel);
}


void agregar_entrada_segundo_nivel(t_tabla_segundo_nivel* tabla, int32_t nro_pagina) {
    t_entrada_segundo_nivel* entrada = malloc(sizeof(t_entrada_segundo_nivel));
    
    entrada -> nro_pagina = nro_pagina;
    entrada -> nro_frame = -1;
    entrada -> bit_presencia = 0;
    entrada -> bit_uso = 0;
    entrada -> bit_modificado = 0;

    list_add(tabla -> entradas, (void*) entrada);
}


t_tabla_primer_nivel* get_tabla_primer_nivel(uint32_t id) {
    char* str_id = int_a_string(id);

    sem_wait(mutex_tablas_primer_nivel);
        t_tabla_primer_nivel* tabla = (t_tabla_primer_nivel*) dictionary_get(tablas_primer_nivel, str_id); 
    sem_post(mutex_tablas_primer_nivel);
    
    free(str_id);
    return tabla;
}


t_tabla_segundo_nivel* get_tabla_segundo_nivel(uint32_t id) {
    char* str_id = int_a_string(id);

    sem_wait(mutex_tablas_segundo_nivel);
        t_tabla_segundo_nivel* tabla = (t_tabla_segundo_nivel*) dictionary_get(tablas_segundo_nivel, str_id); 
    sem_post(mutex_tablas_segundo_nivel);

    free(str_id);
    
    return tabla;
}


t_list* get_entradas_en_memoria_proceso(uint32_t PID) {
	t_list* entradas_en_memoria = list_create();
	t_tabla_primer_nivel* tp_lvl1 = get_tabla_primer_nivel(PID);

	for (uint32_t i = 0;i<list_size(tp_lvl1->entradas);i++) {
		t_tabla_segundo_nivel* tp_lvl2 = get_tabla_segundo_nivel((uint32_t) list_get(tp_lvl1->entradas, i));

		for (uint32_t j=0;j<list_size(tp_lvl2->entradas);j++) {
			t_entrada_segundo_nivel* entrada = list_get(tp_lvl2->entradas, j);
			if (entrada -> bit_presencia) {
				list_add(entradas_en_memoria, entrada);
			}
		}
	}

	return entradas_en_memoria;
}


int get_cantidad_entradas_proceso(uint32_t PID) {
    int contador = 0;
    t_tabla_primer_nivel* tp_lvl1 = get_tabla_primer_nivel(PID);
    
	for (uint32_t i=0;i< list_size(tp_lvl1->entradas);i++) {
		t_tabla_segundo_nivel* tp_lvl2 = get_tabla_segundo_nivel((uint32_t) list_get(tp_lvl1 -> entradas, i));
        contador += list_size(tp_lvl2 -> entradas);
	}

    return contador;
}


int get_nro_tabla_segundo_nivel_pagina(int32_t nro_tp_lvl1, int32_t nro_pagina) {
    printf("Tabla de primer nivel NO obtenida");
	t_tabla_primer_nivel* tp_lvl1 = get_tabla_primer_nivel(nro_tp_lvl1);

	for (uint32_t i = 0;i<list_size(tp_lvl1->entradas);i++) {
		t_tabla_segundo_nivel* tp_lvl2 = get_tabla_segundo_nivel((uint32_t) list_get(tp_lvl1->entradas, i));
        printf("Se obtuvo tabla %d de segundo nivel", i);

		for (uint32_t j=0;j<list_size(tp_lvl2->entradas);j++) {
        	t_entrada_segundo_nivel* entrada = (t_entrada_segundo_nivel*) list_get(tp_lvl2->entradas, j);
			printf("Se obtuvo entrada numero %d", j);
            if (entrada -> nro_pagina == nro_pagina) {
				return tp_lvl2 -> id_tabla;
			}
		}
	}

    printf("algo anda mal");
    return 0;
}


t_entrada_segundo_nivel* get_entrada_de_pagina(t_tabla_segundo_nivel* tabla_segundo_nivel, uint32_t nro_pagina) {

    t_entrada_segundo_nivel* entrada;

    for (uint32_t i = 0; i < list_size(tabla_segundo_nivel -> entradas); i++){
        entrada = (t_entrada_segundo_nivel*) list_get(tabla_segundo_nivel -> entradas, i);
        
        if (entrada -> nro_pagina == nro_pagina) {
            return entrada;
        }
    }

    return NULL;
}