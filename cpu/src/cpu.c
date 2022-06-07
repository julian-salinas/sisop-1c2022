#include "cpu.h"

int main(void) {

	logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	//ini config
	cpu_config = ini_cpu_config("cfg/cpu.config");
	log_info(logger,"Config CPU load");

	// obtener valores ip y puerto del archivo config del módulo memoria
	conexion_memoria = crear_socket_cliente(cpu_config->ip_memoria, cpu_config->puerto_memoria);
	log_info(logger,"Conexión cpu-memoria ok.");
    obtener_config_memoria();
   //Conexión con kernel		
   conexion_kernel = crear_socket_cliente(cpu_config->ip_memoria, cpu_config->puerto_escucha_dispatch);
    log_info(logger,"Conexión cpu-kernel ok.");
    
   int server_cpu_dispatch = iniciar_servidor(logger, "CPU Dispatch",IP_MEMORIA, "8001");
   server_cpu_interrupt = iniciar_servidor(logger, "CPU Interrupt",IP_MEMORIA, "8005");
   
   while(1){	
	int header;
   int cliente_dispatch = esperar_cliente(server_cpu_dispatch);
    header = recibir_header(cliente_dispatch);
    log_info(logger, "cod op %d", header);
    switch (header) {

        case CONEXION_CPU_MEMORIA:
        {
            /* TODO
            / recibir paquete y guardar paginas por tabla y tamanio pagina */
            t_buffer* payload = recibir_payload(cliente_dispatch);
            paginas_por_tabla = buffer_take_UINT8(payload);
            tamanio_pagina = buffer_take_UINT8(payload);
            //funciona 
            log_info(logger, "Se recibió configuración de memoria.");
            printf("Páginas por tabla: %u\n",paginas_por_tabla);
            printf("Tamaño de página: %u\n",tamanio_pagina);
            break; 
        }
        case PCB:
            log_info(logger, "Se recibió pcb del Kernel.");
            t_buffer* buffer = recibir_payload(cliente_dispatch);
            t_PCB* pcb = buffer_take_PCB(buffer);  
            //acá debería ir un mutex???
            log_info(logger, "Voy a ejecutar un ciclo de instruccion.");
            ejecutar_ciclo_instruccion(pcb);
            break;
        case INTERRUPCION:
            interrupcion=1;
            break;

        case -1:
            log_error(logger, "Cliente desconectado de s...");
            return;

        default:
            log_error(logger, "Algo anduvo mal en el server de s");
            log_info(logger, "Cop: %d", header);
            return;
    }//fin switch

	}//fin ciclo infinito

	
	//destruir_cpu_config(cpu_config);

	return EXIT_SUCCESS;
}

void obtener_config_memoria(){

	t_paquete* paquete_config = crear_paquete(CONEXION_CPU_MEMORIA, sizeof(uint8_t)*2);
	enviar_paquete(conexion_memoria,paquete_config);
	destruir_paquete(paquete_config);
	//liberar_socket_cliente(conexion_memoria);
	log_info(logger, "Request config memoria enviada.");
}

int esperar_cliente(int socket_servidor)
{
	// Quitar esta línea cuando hayamos terminado de implementar la funcion
	// assert(!"no implementado!");

	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}