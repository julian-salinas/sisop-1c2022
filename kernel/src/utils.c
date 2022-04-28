#include "../include/utils.h"

// int iniciar_servidor(void)
// {
// 	int socket_servidor;

// 	struct addrinfo hints, *servinfo, *p;

// 	memset(&hints, 0, sizeof(hints));
// 	hints.ai_family = AF_UNSPEC;
// 	hints.ai_socktype = SOCK_STREAM;
// 	hints.ai_flags = AI_PASSIVE;

// 	getaddrinfo(IP_KERNEL, PUERTO_KERNEL, &hints, &servinfo);

// 	// Creamos el socket de escucha del servidor
// 	for(p = servinfo; p != NULL; p->ai_next)
// 	{
// 		socket_servidor = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
// 		if(socket_servidor == 1)
// 		{
// 			continue;
// 		}
// 		// Asociamos el socket a un puerto
// 		if(bind(socket_servidor,p->ai_addr,p->ai_addrlen) == 1)
// 		{
// 			close(socket_servidor);
// 			continue;
// 		}
// 		break;
// 	}

// 	// Escuchamos las conexiones entrantes
// 	listen(socket_servidor,SOMAXCONN);

// 	freeaddrinfo(servinfo);
// 	log_trace(logger, "Listo para escuchar a mi cliente");

// 	return socket_servidor;
// }


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
        if (socket_servidor == -1) // fallo de crear socket
            continue;

        if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
            // Si entra aca fallo el bind
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

    // Aviso al logger
    log_info(logger, "Escuchando en %s:%s (%s)\n", ip, puerto, name);

    freeaddrinfo(servinfo);

    return socket_servidor;
}



void iterator(char* value) {
	log_info(logger,"%s", value);
}


// int esperar_cliente(int socket_servidor)
// {
// 	// Aceptamos un nuevo cliente
// 	struct sockaddr_in dir_cliente;
// 	int tam_direccion = sizeof(struct sockaddr_in);

// 	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);
// 	log_info(logger, "Se conecto un cliente!");

// 	return socket_cliente;
// }


int esperar_clientes(t_log* logger, const char* name, int socket_servidor) {
    struct sockaddr_in dir_cliente;
    socklen_t tam_direccion = sizeof(struct sockaddr_in);

    int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

    log_info(logger, "Cliente conectado (a %s)\n", name);

    return socket_cliente;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

static void* procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* logger = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
	t_list* lista;
    free(args);

    op_code cop;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &cop, sizeof(op_code), 0) != sizeof(op_code)) {
            log_info(logger, "DISCONNECT!");
            return;
        }

        switch (cop) {

			// Aca van los codigos de operacion

			case MENSAJE:
				recibir_mensaje(cliente_socket);
				break;
			case PAQUETE:
				lista = recibir_paquete(cliente_socket);
				log_info(logger, "Me llegaron los siguientes valores:\n");
				list_iterate(lista, (void*) iterator);
				break;

            // Errores
            case -1:
                log_error(logger, "Cliente desconectado de %s...", server_name);
                return;
            default:
                log_error(logger, "Algo anduvo mal en el server de %s", server_name);
                log_info(logger, "Cop: %d", cop);
                return;
        }
    }

    log_warning(logger, "El cliente se desconecto de %s server", server_name);
    return;
}

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_clientes(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}