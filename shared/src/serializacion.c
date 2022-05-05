#include "serializacion.h"


t_buffer* crear_buffer(size_t tamanio) {
    t_buffer* tmp = malloc(sizeof(t_buffer));
    tmp -> offset = 0;
    tmp -> tamanio = tamanio;
    tmp -> stream = malloc(tamanio);
    return tmp;
}


void destruir_buffer(t_buffer* buffer) {
    free(buffer -> stream);
    free(buffer);
}


void agregar_a_buffer(t_buffer* buffer, void* valor_a_agregar, size_t tamanio) {
    ajustar_tamanio_memoria_buffer(buffer, tamanio);

    // ayuda - header de memcpy: void *memcpy(void *dest, const void * src, size_t n)
    // https://www.tutorialspoint.com/c_standard_library/c_function_memcpy.htm

    memcpy(buffer -> stream + buffer->offset, valor_a_agregar, tamanio);
    buffer -> offset += tamanio;
}


void ajustar_tamanio_memoria_buffer(t_buffer* buffer, size_t size){
    while(buffer -> tamanio < buffer -> offset + size){
        buffer -> tamanio += TAMANIO_DEFAULT_BUFFER;
        buffer -> stream = realloc(buffer -> stream, buffer -> tamanio);
    }
}


t_paquete* crear_paquete(uint8_t header, size_t tamanio) {
    t_paquete* tmp = malloc(sizeof(t_paquete));
    tmp -> header = header;
    tmp -> payload = crear_buffer(tamanio);
    return tmp;
}


void destruir_paquete(t_paquete* paquete) {
    destruir_buffer(paquete -> payload);
    free(paquete);
}


void agregar_a_paquete(t_paquete* paquete, void* valor_a_agregar, size_t tamanio) {
    agregar_a_buffer(paquete -> payload, valor_a_agregar, tamanio);
}

//------------- Agregar distintos Tipos de datos a un Paquete -------------//

/* -------------------------- int 32 bytes -------------------------- */

void agregar_a_buffer_INT32(t_buffer* buffer, int32_t valor){
    agregar_a_buffer(buffer, (void*) &valor, sizeof(int32_t));
}

/* ---------------------- unsigned int 32 bytes ---------------------- */


void agregar_a_buffer_UINT32(t_buffer* buffer, uint32_t valor){
    agregar_a_buffer(buffer, (void*) &valor, sizeof(uint32_t));
}


/* ---------------------- unsigned int 8 bytes ---------------------- */

void agregar_a_buffer_UINT8(t_buffer* buffer, uint8_t valor){
    agregar_a_buffer(buffer, (void*) &valor, sizeof(uint8_t));
}


/* -------------------------- strings -------------------------- */

void agregar_a_buffer_STRING(t_buffer* buffer, char* valor){
    uint32_t tamanio = string_length((char*) valor) + 1;
    agregar_a_buffer_UINT32(buffer, tamanio);
    agregar_a_buffer(buffer, (void*) valor, tamanio);
}


/* -------------------------- lista -------------------------- */

void agregar_a_buffer_LIST(t_buffer* buffer, t_list* source, void(*agregar_a_buffer_TIPO)(t_buffer*, void*)){
    void _magia_negra(void* elem) {
        agregar_a_buffer_TIPO(buffer, elem);
    };

    uint32_t tamanioLista = source -> elements_count;
    agregar_a_buffer_UINT32(buffer, tamanioLista);
    list_iterate(source, _magia_negra);
}


// ------------------------- Tomar valores del buffer ------------------------- //

void buffer_take(t_buffer* buffer, void** dest, size_t tamanio){
    if(*dest == NULL) *dest = calloc(1, tamanio);
    memcpy(*dest, buffer->stream + buffer->offset, tamanio);
    buffer->offset += tamanio;
}


/* ------------------------ int 32 bytes ------------------------ */

int32_t buffer_take_INT32(t_buffer* buffer){
    int32_t tmp;
    int32_t* puntero_a_tmp = &tmp;
    buffer_take(buffer, (void**) &puntero_a_tmp, sizeof(int32_t));
    return tmp;
}

/* ---------------------- unsigned int 32 bytes ---------------------- */

uint32_t buffer_take_UINT32(t_buffer* buffer){
    uint32_t tmp;
    uint32_t* puntero_a_tmp = &tmp;
    buffer_take(buffer, (void**) &puntero_a_tmp, sizeof(uint32_t));
    return tmp;
}


/* ---------------------- unsigned int 8 bytes ---------------------- */

uint8_t buffer_take_UINT8(t_buffer* buffer){
    uint8_t tmp;
    uint8_t* puntero_a_tmp = &tmp;
    buffer_take(buffer, (void**) &puntero_a_tmp, sizeof(uint8_t));
    return tmp;
}


/* ------------------------- strings ------------------------- */

void buffer_take_STRING_P(t_buffer* buffer, void** dest){
    uint32_t tamanio = buffer_take_UINT32(buffer);
    buffer_take(buffer, dest, tamanio);
}

char* buffer_take_STRING(t_buffer* buffer){
    char* tmp = NULL;
    uint32_t tamanio = buffer_take_UINT32(buffer);
    buffer_take(buffer, (void**) &tmp, tamanio);
    return tmp;
}


/*
-------------------- Comunicación entre cpu y memoria ------------------------------------
*/

t_paquete* serializar_config_cpu_memoria(uint8_t paginas_por_tabla, uint8_t tam_pagina) {
    t_paquete* paquete= crear_paquete(CONEXION_CPU_MEMORIA, sizeof(uint8_t)*2);
    agregar_a_buffer_UINT8(paquete->payload, paginas_por_tabla);
    agregar_a_buffer_UINT8(paquete->payload, tam_pagina);
    return paquete;
}

void deserializar_config_cpu_memoria(void* stream, uint8_t* paginas_por_tabla, uint8_t* tam_pagina) {
    memcpy(paginas_por_tabla, stream, sizeof(uint8_t));
    memcpy(tam_pagina, stream+sizeof(uint8_t), sizeof(uint8_t));
}
