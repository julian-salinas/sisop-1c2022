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


void agregar_instruccion_a_buffer(t_buffer* buffer, t_instruccion* instruccion) {
    agregar_a_buffer_UINT8(buffer, instruccion -> identificador);
    agregar_a_buffer_LIST(buffer, instruccion -> parametros, (void*) agregar_a_buffer_INT32);
}

void enviar_lista_instrucciones(int socket, t_lista_instrucciones* lista_instrucciones) {
    t_paquete* paquete = crear_paquete(INSTRUCCIONES, sizeof(t_list) * list_size(lista_instrucciones));
    agregar_a_buffer_LIST(paquete -> payload, lista_instrucciones, (void*) agregar_instruccion_a_buffer);
    enviar_paquete(socket, paquete);
    destruir_paquete(paquete);
}