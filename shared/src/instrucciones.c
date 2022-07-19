#include "instrucciones.h"
#include "estructura.h"

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

    if(instruccion -> identificador == NO_OP){
        t_instruccion* inst;
        for (int i = 0; i < list_get(instruccion -> parametros, 0); i++)
        {
            inst = crear_instruccion(NO_OP);
            list_add(lista_instrucciones, inst);
        }
        return;
        
    }
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
