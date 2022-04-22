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
