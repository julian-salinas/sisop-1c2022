#include "tabla_paginas.h"


void inicializar_tablas_de_paginas(void) {
    contador_id_primer_nivel = 0;
    contador_id_segundo_nivel = 0;
    tablas_primer_nivel = dictionary_create();
    tablas_segundo_nivel = dictionary_create();

    mutex_id_primer_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_id_primer_nivel, 0, 1);

    mutex_id_segundo_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_id_segundo_nivel, 0, 1); 

    mutex_tablas_primer_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_tablas_primer_nivel, 0, 1);

    mutex_tablas_segundo_nivel = malloc(sizeof(sem_t));
    sem_init(mutex_tablas_segundo_nivel, 0, 1);    
}


t_tabla_primer_nivel* crear_tabla_primer_nivel(void) {
    t_tabla_primer_nivel* tmp = malloc(sizeof(t_tabla_primer_nivel));

    sem_wait(mutex_id_primer_nivel);
        tmp -> id_tabla = contador_id_primer_nivel;
        contador_id_primer_nivel++;
    sem_post(mutex_id_primer_nivel);

    tmp -> entradas = list_create();

    char* string_id = string_from_format("%d", tmp -> id_tabla);

    sem_wait(mutex_tablas_primer_nivel);
        dictionary_put(tablas_primer_nivel, string_id, (void*) tmp);
    sem_post(mutex_tablas_primer_nivel);

    free(string_id);

    return tmp;
}


t_tabla_segundo_nivel* crear_tabla_segundo_nivel(void) {
    t_tabla_segundo_nivel* tmp = malloc(sizeof(t_tabla_segundo_nivel));

    sem_wait(mutex_id_segundo_nivel);
        tmp -> id_tabla = contador_id_segundo_nivel;
        contador_id_segundo_nivel++;
    sem_post(mutex_id_segundo_nivel);

    tmp -> entradas = list_create();

    // for (size_t i = 0; i < memoria_config -> paginas_por_tabla; i++)
    // {
    //     agregar_entrada_segundo_nivel(tmp);
    // }

    char* string_id = string_from_format("%d", tmp -> id_tabla);

    sem_wait(mutex_tablas_segundo_nivel);
        dictionary_put(tablas_segundo_nivel, string_id, (void*) tmp);
    sem_post(mutex_tablas_segundo_nivel);

    free(string_id);

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


t_entrada_segundo_nivel* crear_entrada_segundo_nivel(int id_marco) {
    t_entrada_segundo_nivel* tmp = malloc(sizeof(t_entrada_segundo_nivel));
    
    tmp -> id_marco = id_marco;
    tmp -> bit_presencia = 0;
    tmp -> bit_uso = 0;
    tmp -> bit_modificado = 0;
    
    return tmp;     
}


void agregar_entrada_primer_nivel(t_tabla_primer_nivel* tabla, int32_t id_tabla_segundo_nivel) {
    if (list_size(tabla -> entradas) >= memoria_config -> paginas_por_tabla) {
        log_error(logger, "No se puede agregar una entrada más a la tabla %d, ya tiene %d entradas", 
                  tabla -> id_tabla, 
                  memoria_config -> paginas_por_tabla);
        return;
    }

    list_add(tabla -> entradas, (void*) id_tabla_segundo_nivel);
}


void agregar_entrada_segundo_nivel(t_tabla_segundo_nivel* tabla) {
    if (list_size(tabla -> entradas) >= memoria_config -> paginas_por_tabla) {
        log_error(logger, "No se puede agregar una entrada más a la tabla %d, ya tiene %d entradas", 
                  tabla -> id_tabla, 
                  memoria_config -> paginas_por_tabla);
        return;
    }
    
    // TODO: Obtener marco de página
    // TODO: Crear entrada usando el marco obtenido
    void* entrada = NULL;
    
    list_add(tabla -> entradas, (void*) entrada);
}

t_tabla_primer_nivel* get_tabla_primer_nivel(uint32_t id) {
    char* str_id = string_itoa(id);

    sem_wait(mutex_tablas_primer_nivel);
        t_tabla_primer_nivel* tabla = dictionary_get(tablas_primer_nivel, str_id); 
    sem_post(mutex_tablas_primer_nivel);
    
    free(str_id);
    return tabla;
}


t_tabla_segundo_nivel* get_tabla_segundo_nivel(uint32_t id) {
    char* str_id = string_itoa(id);

    sem_wait(mutex_tablas_segundo_nivel);
        t_tabla_segundo_nivel* tabla = dictionary_get(tablas_segundo_nivel, str_id); 
    sem_post(mutex_tablas_segundo_nivel);

    free(str_id);
    return tabla;
}


void crear_pagina(uint32_t id_tabla) {
    
    sem_wait(mutex_tablas_segundo_nivel);
        t_tabla_segundo_nivel* tabla = get_tabla_segundo_nivel(id_tabla);
    sem_post(mutex_tablas_segundo_nivel);

    agregar_entrada_segundo_nivel(tabla);

    void* contenido = malloc(memoria_config -> tamanio_pagina);

    // TODO: Swappear página
}