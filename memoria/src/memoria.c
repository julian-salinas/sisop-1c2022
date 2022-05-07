#include "memoria.h"

int main(void) {

	//ini config
	ini_config();

	//ini estructuras

	//suspension proceso ->liberar espacio de memoria escribiendo en SWAP

	//acceso a tabla de páginas
	// 1er nivel responder el n° de tabla 2
	// 2do nivel responder el marco (cargar si corresponde y lo que conlleva)

	//acceso a espacio de usuario


	//ini servidor
	logger = iniciar_logger("cfg/memoria.log", "memoria");
	log_info(logger, IP_MEMORIA);
	log_info(logger, PUERTO_MEMORIA);
	int server_memoria = iniciar_servidor(logger,"Memoria",IP_MEMORIA, PUERTO_MEMORIA);
	log_info(logger, "Memoria lista para recibir al cliente");

	while (1) {
		int cliente_fd = esperar_clientes(logger, "Memoria", server_memoria);
		int cod_op = recibir_header(cliente_fd);

		switch (cod_op) {
			case CONEXION_CPU_MEMORIA:
				//enviar a CPU cantidad de entradas por tabla de páginas y tamaño de página;
				conexion_cpu = crear_socket_cliente(IP_MEMORIA, "8013");
				log_info(logger, "Socket cliente memoria-cpu creado.");
				enviar_config_a_cpu(conexion_cpu);	
				break;
			
			case -1:
				log_error(logger, "el cliente se desconecto. Terminando servidor");
				return EXIT_FAILURE;
			
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				break;
		}
	}

	//terminar_programa
	terminar_programa();
	return EXIT_SUCCESS;
}

void terminar_programa(){
		liberar_socket_cliente(conexion_cpu);
		log_destroy(logger);
		config_destroy(config_memoria);
}

void enviar_config_a_cpu(int socket_cliente){

	t_paquete* paquete = serializar_config_cpu_memoria(config_data.paginas_por_tabla, config_data.tam_pagina);
	log_info(logger, "Serializo ok.");	
	enviar_paquete(socket_cliente,paquete);
	destruir_paquete(paquete);
	log_info(logger, "Config enviado.");

}