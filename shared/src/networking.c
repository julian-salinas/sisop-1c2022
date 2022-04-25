#include "networking.h"

int crear_socket_cliente(char* ip, char* puerto) {
    struct addrinfo hints;
    struct addrinfo *server_info;
    
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(ip, puerto, &hints, &server_info);

    // Crear el socket
    int socket_cliente = 0;
	socket_cliente = socket(server_info->ai_family, 
                            server_info->ai_socktype,
                            server_info->ai_protocol);

	// Conectar el socket
	connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

    // Liberar memoria
	freeaddrinfo(server_info);

	return socket_cliente;
}


void liberar_socket_cliente(int socket_cliente) {
    close(socket_cliente);
}


int crear_socket_servidor(char *ip, char* puerto) {
	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	// Crear el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	
	// Asociar el socket a un puerto
	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	// Escuchar las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
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

//de tp0
int iniciar_servidor(void)
{
	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP_MEMORIA, PUERTO_MEMORIA, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	for(p=servinfo; p != NULL; p->ai_next)
	{
		socket_servidor = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if(socket_servidor == 1)
		{
			continue;
		}
		// Asociamos el socket a un puerto
		if(bind(socket_servidor,p->ai_addr,p->ai_addrlen) == 1)
		{
			close(socket_servidor);
			continue;
		}
		break;
	}

	// Escuchamos las conexiones entrantes
	listen(socket_servidor,SOMAXCONN);

	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}


void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

void enviar_mensaje(char* mensaje, int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = MENSAJE;
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = strlen(mensaje) + 1;
	paquete->buffer->stream = malloc(paquete->buffer->size);
	memcpy(paquete->buffer->stream, mensaje, paquete->buffer->size);

	int bytes = paquete->buffer->size + 2*sizeof(int);

	void* a_enviar = serializar_paquete(paquete, bytes);

	send(socket_cliente, a_enviar, bytes, 0);

	free(a_enviar);
	eliminar_paquete(paquete);
}
