#include "instrucciones.h"

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
    t_lista_instrucciones* tmp = list_create();
    return tmp;
}


void destruir_lista_instrucciones(t_lista_instrucciones *lista_instrucciones) {
    list_destroy(lista_instrucciones);
}


void agregar_instruccion_a_lista(t_lista_instrucciones* lista_instrucciones, t_instruccion* instruccion) {
    list_add(lista_instrucciones, instruccion);
}


void agregar_a_buffer_INSTRUCCION(t_buffer* buffer, t_instruccion* instruccion) {
    agregar_a_buffer_UINT8(buffer, instruccion -> identificador);
    agregar_a_buffer_LIST(buffer, instruccion -> parametros, (void*) agregar_a_buffer_INT32);
}


void enviar_lista_instrucciones(int socket, t_lista_instrucciones* lista_instrucciones) {
    t_paquete* paquete = crear_paquete(INSTRUCCIONES, sizeof(t_list));
    agregar_a_buffer_LIST(paquete -> payload, lista_instrucciones, (void*) agregar_a_buffer_INSTRUCCION);
    enviar_paquete(socket, paquete);
    destruir_paquete(paquete);
}


t_proceso* crear_proceso(size_t tamanio, t_lista_instrucciones* lista_instrucciones) {
    t_proceso* tmp = malloc(sizeof(t_proceso));
    
    tmp -> tamanio = tamanio;
    tmp -> lista_instrucciones = malloc(sizeof(t_lista_instrucciones));
    tmp -> lista_instrucciones = lista_instrucciones;

    return tmp;
}


void destruir_proceso(t_proceso* proceso) {
    destruir_lista_instrucciones(proceso -> lista_instrucciones);
    free(proceso);
}


void enviar_proceso(int socket, t_proceso* proceso) {
    t_paquete* paquete = crear_paquete(INSTRUCCIONES, sizeof(t_proceso));
    agregar_a_buffer_UINT32(paquete -> payload, proceso -> tamanio);
    agregar_a_buffer_LIST(paquete -> payload, proceso -> lista_instrucciones, (void*) agregar_a_buffer_INSTRUCCION);
    enviar_paquete(socket, paquete);
    destruir_paquete(paquete);
}


t_instruccion* buffer_take_INSTRUCCION(t_buffer* buffer) {
    t_instruccion* tmp = malloc(sizeof(t_instruccion));

    uint8_t identificador;
    identificador = buffer_take_UINT8(buffer);

    t_list* parametros = malloc(sizeof(t_list));
    parametros = buffer_take_LIST(buffer, (void*) buffer_take_UINT32);

    tmp -> identificador = identificador;
    tmp -> parametros = malloc(sizeof(t_list));
    tmp -> parametros = parametros;

    return tmp;
}


t_proceso* buffer_take_PROCESO(t_buffer* buffer) {
    uint32_t tamanio_proceso;
    tamanio_proceso = buffer_take_UINT32(buffer);

    t_lista_instrucciones* lista_instrucciones;
    lista_instrucciones = buffer_take_LIST(buffer, (void*)*buffer_take_INSTRUCCION);

    t_proceso* tmp = crear_proceso(tamanio_proceso, lista_instrucciones);

    return tmp;    
}