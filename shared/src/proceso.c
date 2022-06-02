#include "proceso.h"

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
    agregar_a_buffer_PROCESO(paquete -> payload, proceso);
    enviar_paquete(socket, paquete);
    destruir_paquete(paquete);
}

void agregar_a_buffer_PROCESO(t_buffer* buffer, t_proceso* proceso) {
    agregar_a_buffer_UINT32(buffer, proceso -> tamanio);
    agregar_a_buffer_LIST(buffer, proceso -> lista_instrucciones, (void*) agregar_a_buffer_INSTRUCCION);
}

t_proceso* buffer_take_PROCESO(t_buffer* buffer) {
    uint32_t tamanio_proceso;
    tamanio_proceso = buffer_take_UINT32(buffer);

    t_lista_instrucciones* lista_instrucciones;
    lista_instrucciones = buffer_take_LIST(buffer, (void*)*buffer_take_INSTRUCCION);

    t_proceso* tmp = crear_proceso(tamanio_proceso, lista_instrucciones);

    return tmp;    
}

void enviar_pcb(int socket, t_PCB* pcb) {
    t_paquete* paquete = crear_paquete(PCB, sizeof(t_PCB));
    agregar_a_buffer_UINT32(paquete -> payload, pcb -> PID);
    agregar_a_buffer_UINT32(paquete -> payload, pcb -> tamanio);
    agregar_a_buffer_LIST(paquete -> payload, pcb -> lista_instrucciones, (void*) agregar_a_buffer_INSTRUCCION);
    agregar_a_buffer_INT32(paquete -> payload, pcb -> program_counter);
    agregar_a_buffer_INT32(paquete -> payload, pcb -> tabla_paginas);
    agregar_a_buffer_INT32(paquete -> payload, pcb -> estimacion_rafaga);
    agregar_a_buffer_INT32(paquete -> payload, pcb -> tiempo_ejecucion);
    agregar_a_buffer_INT32(paquete -> payload, pcb -> socket_cliente);
    agregar_a_buffer_INT32(paquete -> payload, pcb -> estado);

    enviar_paquete(socket, paquete);
    destruir_paquete(paquete);
}

t_PCB* buffer_take_PCB(t_buffer* buffer) {
    t_PCB* pcb = malloc(sizeof(t_PCB));

    pcb -> PID = buffer_take_UINT32(buffer);
    pcb -> tamanio = buffer_take_UINT32(buffer);
    pcb -> lista_instrucciones = buffer_take_LIST(buffer, (void*)*buffer_take_INSTRUCCION);
    pcb -> program_counter = buffer_take_INT32(buffer);
    pcb -> tabla_paginas = buffer_take_INT32(buffer);
    pcb -> estimacion_rafaga = buffer_take_INT32(buffer);
    pcb -> tiempo_ejecucion = buffer_take_INT32(buffer);
    pcb -> socket_cliente = buffer_take_INT32(buffer);
    pcb -> estado = buffer_take_INT32(buffer);

    return pcb;
}