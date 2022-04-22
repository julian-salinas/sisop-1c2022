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
    return (void*) tamanio;
}


void destruir_paquete(t_paquete* paquete) {
    destruir_buffer(paquete -> payload);
    free(paquete);
}


void agregar_a_paquete(t_paquete* paquete, void* valor_a_agregar, size_t tamanio) {
    agregar_a_buffer(paquete -> payload, valor_a_agregar, tamanio);
}
