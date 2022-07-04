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
}


t_tabla_primer_nivel* crear_tabla_primer_nivel(int id_tabla) {
    t_tabla_primer_nivel* tmp = malloc(sizeof(t_tabla_primer_nivel));

    tmp -> entradas = list_create();
    char* str_id = int_to_string(id_tabla);

    sem_wait(mutex_tablas_primer_nivel);
        dictionary_put(tablas_primer_nivel, int_to_string(id_tabla), (void*) tmp);
    sem_post(mutex_tablas_primer_nivel);

    free(str_id);

    return tmp;
}


t_tabla_segundo_nivel* crear_tabla_segundo_nivel(void) {
    t_tabla_segundo_nivel* tmp = malloc(sizeof(t_tabla_segundo_nivel));

    tmp -> entradas = list_create();

    sem_wait(mutex_id_tablas);
        char* str_id = int_to_string(id_tablas_segundo_nivel);
    sem_post(mutex_id_tablas);

    sem_wait(mutex_tablas_segundo_nivel);
        dictionary_put(tablas_segundo_nivel, str_id, (void*) tmp);
    sem_post(mutex_tablas_segundo_nivel);
    
    free(str_id);
    
    return tmp;    
}


void destruir_tabla_primer_nivel(t_tabla_primer_nivel* tabla) {
    list_destroy(tabla -> entradas);
    free(tabla);
}


void destruir_tabla_segundo_nivel(t_tabla_segundo_nivel* tabla) {
    list_destroy(tabla -> entradas);
    free(tabla);
} 


t_entrada_segundo_nivel* crear_entrada_segundo_nivel(int nro_frame) {
    t_entrada_segundo_nivel* tmp = malloc(sizeof(t_entrada_segundo_nivel));
    
    tmp -> nro_frame = nro_frame;
    tmp -> bit_presencia = 0;
    tmp -> bit_uso = 0;
    tmp -> bit_modificado = 0;
    
    return tmp;     
}


void agregar_entrada_primer_nivel(t_tabla_primer_nivel* tabla, int32_t id_tabla_segundo_nivel) {
    list_add(tabla -> entradas, (void*) id_tabla_segundo_nivel);
}


void agregar_entrada_segundo_nivel(t_tabla_segundo_nivel* tabla) {
    t_entrada_segundo_nivel* entrada = malloc(sizeof(t_entrada_segundo_nivel));
    
    entrada -> nro_frame = -1;
    entrada -> bit_presencia = 0;
    entrada -> bit_uso = 0;
    entrada -> bit_modificado = 0;

    list_add(tabla -> entradas, (void*) entrada);
}


t_tabla_primer_nivel* get_tabla_primer_nivel(uint32_t id) {
    char* str_id = int_to_string(id);

    sem_wait(mutex_tablas_primer_nivel);
        t_tabla_primer_nivel* tabla = dictionary_get(tablas_primer_nivel, str_id); 
    sem_post(mutex_tablas_primer_nivel);
    
    free(str_id);
    return tabla;
}


t_tabla_segundo_nivel* get_tabla_segundo_nivel(uint32_t id) {
    char* str_id = int_to_string(id);

    sem_wait(mutex_tablas_segundo_nivel);
        t_tabla_segundo_nivel* tabla = dictionary_get(tablas_segundo_nivel, str_id); 
    sem_post(mutex_tablas_segundo_nivel);

    free(str_id);
    return tabla;
}
