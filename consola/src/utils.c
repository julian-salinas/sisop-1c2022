#include "utils.h"

// Esto de aquí quedó desactualizado
// igual creo que no lo vamos a usar


// void enviar_mensaje(char* mensaje, int socket_cliente)
// {
// 	t_paquete* paquete = malloc(sizeof(t_paquete));

// 	paquete->header = MENSAJE;
// 	paquete->payload = malloc(sizeof(t_buffer));
// 	paquete->payload->tamanio = strlen(mensaje) + 1;
// 	paquete->payload->stream = malloc(paquete->payload->tamanio);
// 	memcpy(paquete->payload->stream, mensaje, paquete->payload->tamanio);

// 	int bytes = paquete->payload->tamanio + 2*sizeof(int);

// 	void* a_enviar = serializar_paquete(paquete, bytes);

// 	send(socket_cliente, a_enviar, bytes, 0);

// 	free(a_enviar);
// 	destruir_paquete(paquete);
// }
