#include "cpu.h"

int main(void) {

    //ini logger
	logger = iniciar_logger("cfg/cpu.log", "CPU");
	log_info(logger,"Modulo CPU iniciado");

	//ini config
	cpu_config = ini_cpu_config("cfg/cpu.config");
	log_info(logger,"Config CPU load");

    //memoria y kernel
    ini_conexiones();

    ini_tlb();
    
    ini_semaforos();
    
    ini_threads();
    
    //libera memoria y kernel
    liberar_conexiones();
    finalizar_semaforos_cpu();

    return EXIT_SUCCESS;
} //end MAIN

void liberar_conexiones(void){
    liberar_socket_cliente(conexion_memoria);
    //liberar_socket_cliente(conexion_kernel);
}

void ini_conexiones(void){
   	// obtener valores ip y puerto del archivo config del módulo memoria
	conexion_memoria = crear_socket_cliente(cpu_config->ip_memoria, cpu_config->puerto_memoria);
	log_info(logger,"Conexión cpu-memoria ok.");

    //Conexión con kernel		
    //conexion_kernel = crear_socket_cliente(cpu_config->ip_memoria, cpu_config->puerto_escucha_dispatch);
        //Conexión con test

    
 }

void finalizar_semaforos_cpu(void) {
    free(mutex_interrupt);
}

void ini_semaforos(void){
    mutex_interrupt = malloc(sizeof(sem_t));
	sem_init(mutex_interrupt, 0, 1);
}

void* atender_dispatch(void *arg){
     server_cpu_dispatch = iniciar_servidor(logger, "CPU Dispatch",IP_MEMORIA, "8001");
     obtener_config_memoria();
     while(server_listen(logger, "CPU Dispatch", server_cpu_dispatch, (void*)(*procesar_conexion)));
}

void* atender_interrupt(void *arg){
    server_cpu_interrupt = iniciar_servidor(logger, "CPU Interrupt",IP_MEMORIA, "8005");   
    while(server_listen(logger, "CPU Interrupt", server_cpu_interrupt, (void*)(*procesar_conexion)));
}

void ini_threads(void) {

    pthread_create(&thread_dispatch, NULL, (void*) atender_dispatch, NULL);
    pthread_create(&thread_interrupt, NULL, (void*) atender_interrupt, NULL);
    pthread_join(thread_dispatch, NULL);
    pthread_join(thread_interrupt, NULL);
 }

void obtener_config_memoria(void){

	t_paquete* paquete_config = crear_paquete(CONEXION_CPU_MEMORIA, sizeof(uint8_t)*2);
	enviar_paquete(conexion_memoria,paquete_config);
	destruir_paquete(paquete_config);
	//liberar_socket_cliente(conexion_memoria);
	log_info(logger, "Request config memoria enviada.");
}

void ini_tlb(void){
    tlb= inicializar_tlb();
    generarEntradasTlb(tlb);
}