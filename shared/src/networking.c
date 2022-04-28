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


int iniciar_servidor(t_log* logger, const char* name, char* ip, char* puerto) {
    int socket_servidor;
    struct addrinfo hints, *servinfo;

    // Inicializando hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Recibe los addrinfo
    getaddrinfo(ip, puerto, &hints, &servinfo);

    bool conecto = false;

    // Itera por cada addrinfo devuelto
    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next) {
        socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (socket_servidor == -1) // fallo al crear socket
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            // Fallo en el bind
            close(socket_servidor);
            continue;
        }

        // Ni bien conecta uno nos vamos del for
        conecto = true;
        break;
    }

    if(!conecto) {
        free(servinfo);
        return 0;
    }

    listen(socket_servidor, SOMAXCONN); // Escuchando (hasta SOMAXCONN conexiones simultaneas)

    // Loggear resultado
    log_info(logger, "Escuchando en %s:%s (%s)\n", ip, puerto, name);

    freeaddrinfo(servinfo);

    return socket_servidor;
}


int server_escuchar(t_log* logger, char* server_name, int server_socket, void* func_procesar_conexion) {
    int cliente_socket = esperar_clientes(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, func_procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}


int esperar_clientes(t_log* logger, const char* name, int socket_servidor) {
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

    log_info(logger, "Cliente conectado (a %s)\n", name);

    return socket_cliente;
}


// Todas las funciones que están despues de esto están prácticamente igual que en tp0 //
int esperar_cliente(int socket_servidor) {
	int socket_cliente;
	socket_cliente = accept(socket_servidor, NULL, NULL);
	return socket_cliente;
}


void enviar_paquete(t_paquete* paquete, int socket_cliente) {
    size_t bytes = (paquete -> payload -> tamanio) + (2 * sizeof(int));
    void* a_enviar = serializar_paquete(paquete, bytes);

    send(socket_cliente, a_enviar, bytes, 0);
    free(a_enviar);
}


int recibir_operacion(int socket_cliente) {
	t_paquete* paquete = malloc(sizeof(t_paquete));
	if(recv(socket_cliente, &(paquete->header), sizeof(int), MSG_WAITALL) > 0) {
		return paquete->header;
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

	return (void*) buffer;
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


void enviar_lista_instrucciones(t_lista_instrucciones* lista_instrucciones, int socket_cliente) {
    t_paquete* paquete = crear_paquete(INSTRUCCIONES, TAMANIO_DEFAULT_BUFFER);
	agregar_a_paquete(paquete, lista_instrucciones, sizeof(lista_instrucciones));
	serializar_paquete(paquete, paquete -> payload -> tamanio + 2 * sizeof(int));
	enviar_paquete(paquete, socket_cliente);
}


