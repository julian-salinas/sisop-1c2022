#include "testCpu.h"

    /**
    * @DESC: test para cpu
    */

int main(int argc, char** argv) {

	logger = iniciar_logger("cfg/test.log", "Test");

	log_info(logger, "Test iniciado");

    conexion_cpu_dispatch = crear_socket_cliente("127.0.0.1","8001");
	log_info(logger, "Socket dispatch ok");

    int conexion_cpu_interrupt = crear_socket_cliente("127.0.0.1","8005");
	log_info(logger, "Socket interrupt ok");

    /**
    * @DESC:Proceso 1 con 3 instrucciones NO_OP, I_O, EXIT
    */

    int size_proceso = 8;
    t_instruccion* instruccion;
    t_lista_instrucciones* lista_instrucciones = crear_lista_instrucciones();
   
    instruccion = crear_instruccion(NO_OP);
    agregar_parametro_a_instruccion(instruccion, 3);
    agregar_instruccion_a_lista(lista_instrucciones, instruccion);

    instruccion = crear_instruccion(NO_OP);
    agregar_parametro_a_instruccion(instruccion, 5);
    agregar_instruccion_a_lista(lista_instrucciones, instruccion);

    instruccion = crear_instruccion(NO_OP);
    agregar_parametro_a_instruccion(instruccion, 7);
    agregar_instruccion_a_lista(lista_instrucciones, instruccion);
    
    instruccion = crear_instruccion(EXIT);
    agregar_instruccion_a_lista(lista_instrucciones, instruccion);

	t_proceso* proceso = crear_proceso(size_proceso, lista_instrucciones);
     t_PCB* pcb = crear_PCB(proceso, conexion_cpu_dispatch);
    enviar_pcb(conexion_cpu_dispatch, PCB, pcb);

    enviar_header(conexion_cpu_interrupt, INTERRUPCION);


    // Iniciar servidor para que se conecte la cpu
	int server_fd = iniciar_servidor(logger, "Test", "127.0.0.1","8009");

	log_info(logger, "Test listo para recibir clientes");
	
	while(server_listen(logger, "Test", server_fd, (void*)(*procesar_conexion)));

    liberar_socket_cliente(conexion_cpu_dispatch);
    liberar_socket_cliente(conexion_cpu_interrupt);

	return EXIT_SUCCESS;

}

t_PCB* crear_PCB(t_proceso* proceso, int fd) {
    
    t_PCB* pcb = malloc(sizeof(t_PCB));
    pcb -> PID = contador_id_proceso;
    pcb -> tamanio = proceso -> tamanio;
    pcb -> lista_instrucciones = proceso -> lista_instrucciones;
    pcb -> program_counter = 0;
    pcb -> tabla_paginas = -1;
    pcb -> estimacion_rafaga = 0;
    pcb -> tiempo_ejecucion = 0;
    pcb -> socket_cliente = fd;
    pcb -> estado = NEW;

    contador_id_proceso++;

    return pcb;

}