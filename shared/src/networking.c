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


void guard_syscall(int returncode, t_log* logger) {
    if (returncode == -1) {
        int error = errno;
        char* buf = malloc(100);
        strerror_r(error, buf, 100);
        log_error(logger, "Error: %s", buf);
        free(buf);
    }
}


int crear_socket_servidor(char *ip, char* puerto, t_log* logger) {
	int socket_servidor = 0;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &servinfo);

	// Crear el socket de escucha del servidor
    guard_syscall(socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol), logger);
    guard_syscall(setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)), logger);

	// Asociar el socket a un puerto
	guard_syscall(bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen), logger);
	
    // Escuchar las conexiones entrantes
	guard_syscall(listen(socket_servidor, SOMAXCONN), logger);

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


int server_listen(t_log* logger, char* server_name, int server_socket, void(*func_procesar_conexion)(void)) {
    int cliente_socket = esperar_clientes(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) func_procesar_conexion, (void*) args);
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


int send_all(int socket, void *buffer, size_t size){
    while (size > 0){
        int i = send(socket, buffer, size, 0);
        if (i == 0) return 0;
        if (i < 0) return -1;
        buffer += i;
        size -= i;
    }
    return 1;
}


int recv_all(int socket, void *destino, size_t size){
    while (size > 0){
        int i = recv(socket, destino, size, 0);
        if (i == 0) {
            return 0;
        } 
        
        if (i < 0) {
            return -1;
        } 

        destino += i;
        size -= i;
    }
    return 1;
}


void socket_send(int socket, void* source, size_t size){
    send_all(socket, source, size);
}


void enviar_header(int socket, uint8_t header){
    uint8_t tmpHeader = header;
    socket_send(socket, (void*)&tmpHeader, sizeof(uint8_t));
}


bool socket_get(int socket, void* dest, size_t size){
    if(size != 0){
        int rc = recv_all(socket, dest, size);
        if(rc < 1) return false;
    }
    return true;
}


void enviar_paquete(int socket, t_paquete* paquete){
    enviar_header(socket, paquete->header);
    socket_send(socket, (void*) &paquete->payload->offset, sizeof(uint32_t));
    socket_send(socket, (void*)paquete->payload->stream, paquete->payload->offset);
}


uint8_t recibir_header(int socket){
    uint8_t header;
    socket_get(socket, &header, sizeof(uint8_t));
    return header;
}


t_buffer* recibir_payload(int socket) {
    uint32_t tamanio_buffer;

    if(!socket_get(socket, &tamanio_buffer, sizeof(uint32_t)))
        return NULL;

    t_buffer* buffer = crear_buffer(tamanio_buffer);

    if(!socket_get(socket, buffer -> stream, tamanio_buffer)){
        destruir_buffer(buffer);
        buffer = NULL;

    }

    return buffer;
}


t_paquete* recibir_paquete(int socket){
    uint8_t header = recibir_header(socket);
    uint32_t tamanio_buffer;

    if(!socket_get(socket, &tamanio_buffer, sizeof(uint32_t)))
        return NULL;
    
    t_paquete* paquete = crear_paquete(header, tamanio_buffer);
    
    if(!socket_get(socket, paquete->payload->stream, tamanio_buffer)){
        destruir_paquete(paquete);
        paquete = NULL;
    }

    return paquete;
}


void omitir_header(int socket_cliente) {
    uint8_t header;
    socket_get(socket_cliente, &header, sizeof(uint8_t));
}

