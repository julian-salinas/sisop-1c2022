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

void agregar_a_buffer_INT32_P(t_buffer* buffer, void* valor){
    agregar_a_buffer(buffer, valor, sizeof(int32_t));
}


void agregar_a_buffer_INT32(t_buffer* buffer, int32_t valor){
    agregar_a_buffer_INT32_P(buffer, (void*) &valor);
}


/* ---------------------- unsigned int 32 bytes ---------------------- */

void agregar_a_buffer_UINT32_P(t_buffer* buffer, void* valor){
    agregar_a_buffer(buffer, valor, sizeof(uint32_t));
}


void agregar_a_buffer_UINT32(t_buffer* buffer, uint32_t valor){
    agregar_a_buffer_UINT32_P(buffer, (void*)&valor);
}


/* ---------------------- unsigned int 8 bytes ---------------------- */

void agregar_a_buffer_UINT8_P(t_buffer* buffer, void* valor){
    agregar_a_buffer(buffer, valor, sizeof(uint8_t));
}


void agregar_a_buffer_UINT8(t_buffer* buffer, uint8_t valor){
    agregar_a_buffer_UINT8_P(buffer, (void*)&valor);
}


/* -------------------------- strings -------------------------- */

void agregar_a_buffer_STRING_P(t_buffer* paquete, void* valor){
    uint32_t tamanio = string_length((char*)valor) + 1;
    agregar_a_buffer_UINT32(paquete, tamanio);
    agregar_a_buffer(paquete, valor, tamanio);
}


void agregar_a_buffer_STRING(t_buffer* buffer, char* valor){
    agregar_a_buffer_STRING_P(buffer, (void*) valor);
}


// ------------------------- Tomar valores del buffer ------------------------- //

void buffer_take(t_buffer* buffer, void** dest, size_t tamanio){
    if(*dest == NULL) *dest = calloc(1, tamanio);
    memcpy(*dest, buffer->stream + buffer->offset, tamanio);
    buffer->offset += tamanio;
}


/* ------------------------ int 32 bytes ------------------------ */

void buffer_take_INT32_P(t_buffer* buffer, void** dest){
    buffer_take(buffer, dest, sizeof(int32_t));
}


int32_t buffer_take_INT32(t_buffer* buffer){
    int32_t tmp;
    int32_t* puntero_a_tmp = &tmp;
    buffer_take_INT32_P(buffer, (void**) &puntero_a_tmp);
    return tmp;
}

/* ---------------------- unsigned int 32 bytes ---------------------- */

void buffer_take_UINT32_P(t_buffer* buffer, void** dest){
    buffer_take(buffer, dest, sizeof(uint32_t));
}


uint32_t buffer_take_UINT32(t_buffer* buffer){
    uint32_t tmp;
    uint32_t* puntero_a_tmp = &tmp;
    buffer_take_UINT32_P(buffer, (void**) &puntero_a_tmp);
    return tmp;
}


/* ---------------------- unsigned int 8 bytes ---------------------- */

void buffer_take_UINT8_P(t_buffer* buffer, void** dest){
    buffer_take(buffer, dest, sizeof(uint8_t));
}


uint8_t buffer_take_UINT8(t_buffer* buffer){
    uint8_t tmp;
    uint8_t* puntero_a_tmp = &tmp;
    buffer_take_UINT8_P(buffer, (void**) &puntero_a_tmp);
    return tmp;
}


/* ------------------------- strings ------------------------- */

void buffer_take_STRING_P(t_buffer* buffer, void** dest){
    uint32_t tamanio = buffer_take_UINT32(buffer);
    buffer_take(buffer, dest, tamanio);
}

char* buffer_take_STRING(t_buffer* buffer){
    char* tmp = NULL;
    buffer_take_STRING_P(buffer, (void**) &tmp);
    return tmp;
}

/*
-------------------- Comunicación entre consola y kernel ------------------------------------
*/

t_instruccion* crear_instruccion(t_identificador identificador) {
    t_instruccion* tmp = malloc(sizeof(t_instruccion));
    
    tmp -> identificador = identificador;
    tmp -> parametros = list_create();
    
    return tmp;
}


void destruir_instruccion(t_instruccion* instruccion) {
    list_destroy(instruccion -> parametros);
    free(instruccion);
}


void agregar_parametro_a_instruccion(t_instruccion* instruccion, int parametro) {
    list_add(instruccion -> parametros, (void*) parametro);
}


t_lista_instrucciones* crear_lista_instrucciones(void) {
    t_lista_instrucciones* tmp = malloc(sizeof(t_lista_instrucciones));
    tmp -> instrucciones = list_create();
    return tmp;
}


void destruir_lista_instrucciones(t_lista_instrucciones *lista_instrucciones) {
    list_destroy(lista_instrucciones -> instrucciones);
    free(lista_instrucciones);
}


void agregar_instruccion_a_lista(t_lista_instrucciones* lista_instrucciones, t_instruccion* instruccion) {
    list_add(lista_instrucciones -> instrucciones, instruccion);
}
