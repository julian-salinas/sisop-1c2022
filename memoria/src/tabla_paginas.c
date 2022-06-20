#include "tabla_paginas.h"


void ini_tablas_de_paginas(void) {
    contador_id_primer_nivel = 0;
    contador_id_segundo_nivel = 0;

    mutex_id_primer_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_id_primer_nivel, 0, 1);

    mutex_id_segundo_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_id_segundo_nivel, 0, 1); 
}


t_tabla_primer_nivel* crear_tabla_primer_nivel(void) {
    t_tabla_primer_nivel* tmp = malloc(sizeof(t_tabla_primer_nivel));

    sem_wait(mutex_id_primer_nivel);
        tmp -> id_tabla = contador_id_primer_nivel;
        contador_id_primer_nivel++;
    sem_post(mutex_id_primer_nivel);

    tmp -> cantidad_entradas = 0;
    tmp -> entradas = dictionary_create();
    return tmp;
}


void destruir_tabla_primer_nivel(t_tabla_primer_nivel* tabla) {
    dictionary_destroy(tabla -> entradas);
    free(tabla);
}


t_tabla_segundo_nivel* crear_tabla_segundo_nivel(void) {
    t_tabla_segundo_nivel* tmp = malloc(sizeof(t_tabla_segundo_nivel));

    sem_wait(mutex_id_segundo_nivel);
        tmp -> id_tabla = contador_id_segundo_nivel;
        contador_id_segundo_nivel++;
    sem_post(mutex_id_segundo_nivel);

    tmp -> cantidad_entradas = 0;
    tmp -> entradas = dictionary_create();
    return tmp;    
}


void destruir_tabla_segundo_nivel(t_tabla_segundo_nivel* tabla) {
    dictionary_destroy(tabla -> entradas);
    free(tabla);
} 


t_entrada_primer_nivel* crear_entrada_primer_nivel(int id_tabla_segundo_nivel) {
    t_entrada_primer_nivel* tmp = malloc(sizeof(t_entrada_primer_nivel));
    
    tmp -> id_tabla_segundo_nivel = id_tabla_segundo_nivel;
    
    return tmp;     
}


t_entrada_segundo_nivel* crear_entrada_segundo_nivel(int id_marco, int bit_presencia, int bit_u, int bit_modificado) {
    t_entrada_segundo_nivel* tmp = malloc(sizeof(t_entrada_segundo_nivel));
    
    tmp -> id_marco = id_marco;
    tmp -> bit_presencia = bit_presencia;
    tmp -> bit_u = bit_u;
    tmp -> bit_modificado = bit_modificado;
    
    return tmp;     
}


void agregar_entrada_primer_nivel(t_tabla_primer_nivel* tabla, t_entrada_primer_nivel* entrada) {
    dictionary_put(tabla -> entradas, 
                   (char*) string_from_format("entrada_%d", dictionary_size(tabla -> entradas)),
                   (void*) entrada);
}


void agregar_entrada_segundo_nivel(t_tabla_segundo_nivel* tabla, t_entrada_segundo_nivel* entrada) {
    dictionary_put(tabla -> entradas, 
                (char*) string_from_format("entrada_%d", dictionary_size(tabla -> entradas)),
                (void*) entrada);
}
