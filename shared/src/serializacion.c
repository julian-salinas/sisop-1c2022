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


void enviar_paquete(t_paquete* paquete, int socket_cliente) {
    size_t bytes = (paquete -> payload -> tamanio) + (2 * sizeof(int));
    void* a_enviar = serializar_paquete(paquete, bytes);

    send(socket_cliente, a_enviar, bytes, 0);
    free(a_enviar);
}


void* serializar_paquete(t_paquete* paquete, size_t bytes){
    void * magic = malloc(bytes);
    int desplazamiento = 0;

    memcpy(magic + desplazamiento, &(paquete -> header), sizeof(int));
    desplazamiento+= sizeof(int);

    memcpy(magic + desplazamiento, &(paquete-> payload -> tamanio), sizeof(int));
    desplazamiento += sizeof(int);
    
    memcpy(magic + desplazamiento, paquete -> payload -> stream, paquete -> payload -> tamanio);
    desplazamiento += paquete -> payload -> tamanio;

    return magic;
}

// Todas las funciones que están despues de esto están prácticamente igual que en tp0 //
int esperar_cliente(int socket_servidor) {
	int socket_cliente;
	socket_cliente = accept(socket_servidor, NULL, NULL);
	return socket_cliente;
}


int recibir_operacion(int socket_cliente) {
	uint8_t cod_op;

	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) {
		return cod_op;
    }
	
    else {
		close(socket_cliente);
		return -1;
	}
}


void* recibir_buffer(int socket_cliente, size_t* tamanio_buffer) {
	void * buffer;

	recv(socket_cliente, tamanio_buffer, sizeof(int), MSG_WAITALL);
	
    buffer = malloc(*tamanio_buffer);
	recv(socket_cliente, buffer, *tamanio_buffer, MSG_WAITALL);

	return buffer;
}


t_list* recibir_paquete(int socket_cliente) {
	size_t tamanio_buffer;
	size_t tamanio_valor; // esta variable va cambiando segun los valores que contiene el buffer
	
    int desplazamiento = 0; // offset
	void* buffer;

	t_list* valores = list_create();
	buffer = recibir_buffer(socket_cliente, &tamanio_buffer);
    
	while(desplazamiento < tamanio_buffer) {
		memcpy(&tamanio_valor, buffer + desplazamiento, sizeof(int));
		desplazamiento += sizeof(int);
		char* valor = malloc(tamanio_valor);

		memcpy(valor, buffer + desplazamiento, tamanio_valor);
		desplazamiento += tamanio_valor;
		list_add(valores, valor);
	}

	free(buffer);
	return valores;
}