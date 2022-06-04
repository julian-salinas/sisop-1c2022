#include "kernel.h"

int main(void) {

	inicializar_semaforos();
	inicializar_colas();

	// Inicializo contador de Procesos para PCB
	sem_wait(mutex_contador_id_proceso);
	contador_id_proceso = 0;
	sem_post(mutex_contador_id_proceso);
	
	// Crear logger
	logger = log_create("cfg/kernel.log", "kernel", 1, LOG_LEVEL_DEBUG);

	log_info(logger,"Kernel iniciado");

	// Inicializar configuración
	kernel_config = ini_kernel_config("cfg/kernel.config");

	// Incializo estimacion de rafaga para PCB
	estimacion_rafaga_inicial = kernel_config -> estimacion_inicial;

	inicializar_hilos_planificadores();

	// Conexión con memoria
	conexion_memoria = crear_socket_cliente(kernel_config -> ip_memoria, kernel_config -> puerto_memoria);
	
	// Conexion con CPU
	conexion_cpu = crear_socket_cliente(kernel_config -> ip_cpu, kernel_config -> puerto_cpu_dispatch);
	
	// Iniciar servidor para que se conecte la consola
	int server_fd = iniciar_servidor(logger, "Kernel", kernel_config -> ip_kernel, kernel_config -> puerto_escucha);

	log_info(logger, "Kernel lista para recibir al cliente");
	
	while(server_listen(logger, "Kernel", server_fd, (void*)(*procesar_conexion)));

	finalizar_kernel(conexion_memoria, conexion_cpu, logger, kernel_config);

	return EXIT_SUCCESS;
}

void finalizar_kernel(int conexion, int otraConexion, t_log* logger, t_kernel_config* config) {
	liberar_socket_cliente(conexion);
	liberar_socket_cliente(otraConexion);
	log_destroy(logger);
	destruir_kernel_config(config);
}

void inicializar_hilos_planificadores(void) {
	pthread_create(&planif_largo_plazo, NULL, (void*) func_largo_plazo, NULL);
	pthread_detach(planif_largo_plazo);

	pthread_create(&planif_mediano_plazo, NULL, (void*) func_mediano_plazo, NULL);
	pthread_detach(planif_mediano_plazo);

	pthread_create(&planif_corto_plazo, NULL, (void*) func_corto_plazo, NULL);
	pthread_detach(planif_corto_plazo);
}

void inicializar_semaforos() {

	mutex_contador_id_proceso = malloc(sizeof(sem_t));
	sem_init(mutex_contador_id_proceso, 0, 1);

	sem_mediano_plazo = malloc(sizeof(sem_t));
	sem_init(sem_mediano_plazo, 0, 0);

	cont_multiprogramacion = malloc(sizeof(sem_t));
	sem_init(sem_mediano_plazo, 0, kernel_config -> grado_multiprogramacion);
}

void* func_largo_plazo(void* args){;}

void* func_mediano_plazo(void* args){
	
	while (1) {
		sem_wait(sem_mediano_plazo);
		sem_wait(cont_multiprogramacion);

		if (cola_esta_vacia(cola_suspended_ready)) {
			sem_post(sem_largo_plazo);
			continue;
		}

		enviar_header(conexion_cpu, NUEVO_PROCESO);
		
		t_paquete* respuesta_cpu = recibir_paquete(conexion_cpu);
		t_PCB* pcb = buffer_take_PCB(respuesta_cpu -> payload);

		running_a_ready(pcb);

		suspended_ready_a_ready();
		
		sem_post(sem_corto_plazo);
	}
}

void* func_corto_plazo(void* args){
	
	while(1) {
		sem_wait(sem_corto_plazo);

		planificar();

		ready_a_running();	

	}
}

void planificar(void) {	
	if (algoritmo_elegido == SJF) {
		sem_wait(mutex_cola_ready);
			list_sort(cola_ready -> elements, (void*) *(algoritmo_SJF));
		sem_post(mutex_cola_ready);
	}
}

bool algoritmo_SJF(void* proceso1, void* proceso2) {
    return ((t_PCB*) proceso1) -> estimacion_rafaga <= ((t_PCB*) proceso2) -> estimacion_rafaga;
}
