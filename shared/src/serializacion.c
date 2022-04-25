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


void* serializar_paquete(t_paquete* paquete, size_t bytes){
    void* magic = malloc(bytes);
    int desplazamiento = 0;

    memcpy(magic + desplazamiento, &(paquete -> header), sizeof(int));
    desplazamiento+= sizeof(int);

    memcpy(magic + desplazamiento, &(paquete-> payload -> tamanio), sizeof(int));
    desplazamiento += sizeof(int);
    
    memcpy(magic + desplazamiento, paquete -> payload -> stream, paquete -> payload -> tamanio);
    desplazamiento += paquete -> payload -> tamanio;

    return magic;
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
