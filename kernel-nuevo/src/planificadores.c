#include "planificadores.h"


int calcular_estimacion(t_PCB* pcb){

    // inicializo config para el alfa
    int estimacion_actual = 0, tiempo_ejecucion = 0;
    float alfa = kernel_config -> alfa;

    estimacion_actual = pcb -> estimacion_rafaga;

    // falta que llegue la info del tiempo_ejecucion desde CPU

    estimacion_actual = alfa * tiempo_ejecucion + (1 - alfa) * estimacion_actual;  

    pcb -> estimacion_rafaga = estimacion_actual;

    destruir_kernel_config(kernel_config);

    return estimacion_actual;
}


void elegir_algoritmo(char* algoritmo) {
    
    if(strcmp(algoritmo, "FIFO") == 0) {
        algoritmo_elegido = FIFO;
        log_info(logger, "Se eligio el algoritmo FIFO");
    }

    if((strcmp(algoritmo,"SRT") == 0) || (strcmp(algoritmo,"SJF") == 0)) {
        algoritmo_elegido = SJF;
        log_info(logger, "Se eligio el algoritmo SRT/SJF");
    }
}


void ini_threads(void) {

    pthread_create(&thread_corto_plazo, 0, (void*) func_corto_plazo, NULL);
    pthread_detach(thread_corto_plazo);

    pthread_create(&thread_mediano_plazo, 0, (void*) func_mediano_plazo, NULL);
    pthread_detach(thread_mediano_plazo);
    
    pthread_create(&thread_largo_plazo, 0, (void*) func_largo_plazo, NULL);
    pthread_detach(thread_largo_plazo);

    pthread_create(&thread_io, 0, (void*) func_io, NULL);
    pthread_detach(thread_io);

}


void* func_corto_plazo(void* args){
    t_PCB* procesoAMover;
    
    while (1) {

        sem_wait(sem_corto_plazo);

        if (transicion_running_a_blocked) {
            // Agarrar variable global que contiene PCB
            // Agregarlo a cola blocked
            running_a_blocked(proceso_desalojado);

            // Empezar a contabilizar tiempo bloqueado

            // pasar transicion running a blocked a false
        }

        else if (transicion_ready_a_running) {

            if (proceso_corriendo) {

                // Avisar a CPU
                enviar_header(conexion_cpu, NUEVO_PROCESO_READY);

                uint8_t resp_cpu = recibir_header(conexion_cpu);

                if (resp_cpu != CPU_OK) {
                    log_error(logger, "Planif. Corto Plazo - Fallo comunicación con CPU");
                    
                    // Recursividad hasta que la CPU responda
                    sem_post(sem_corto_plazo);
                    continue;
                }

                t_buffer* payload = recibir_payload(conexion_cpu);
                procesoAMover = buffer_take_PCB(payload);

                running_a_ready(procesoAMover);
            }

            ordenar_cola_ready();

            log_info(logger, "Cola ready ordenada");

            procesoAMover = ready_a_running();
            
            enviar_pcb(conexion_cpu, procesoAMover);
        }
    }
}


void* func_mediano_plazo(void* args) {

    while (1) {
        sem_wait(sem_multiprogramacion);
        sem_wait(sem_nuevo_proceso);
        sem_wait(mutex_mediano_plazo);

            if(cola_esta_vacia(cola_suspended_ready)) {
                sem_post(sem_largo_plazo);
                sem_post(mutex_mediano_plazo);
                continue;
            }

            suspended_ready_a_ready();

            // Avisar al planificador de corto plazo para que planifique
            sem_post(sem_corto_plazo);

        sem_post(mutex_mediano_plazo);
    }
}


void* func_largo_plazo(void* args){
    // No es necesario hacer un wait multiprogramación, ya la hizo la función del planif. mediano plazo
    while (1) {
        sem_wait(sem_largo_plazo);

        if (transicion_new_a_ready) {
            sem_wait(mutex_mediano_plazo);
                new_a_ready();
            sem_post(mutex_mediano_plazo);

            transicion_ready_a_running = true;
            
            sem_post(sem_corto_plazo);
        }
        else if (transicion_running_a_exit) {
            // Pasar proceso a exit
            running_a_exit(proceso_desalojado);
            // Informar a consola que finalizó
            enviar_header(proceso_desalojado -> socket_cliente, PROCESO_FINALIZADO);
            // devolver variable a false
            transicion_running_a_exit = false;
        }
    }
}


void ordenar_cola_ready(void) {
    if (algoritmo_elegido == SJF) {
        sem_wait(mutex_cola_ready);
            list_sort(cola_ready -> elements, (void*) *(algoritmo_SJF));
        sem_post(mutex_cola_ready);
    }
}


bool algoritmo_SJF(void* proceso1, void* proceso2) {
    return ((t_PCB*) proceso1) -> estimacion_rafaga <= ((t_PCB*) proceso2) -> estimacion_rafaga;
}


void* func_io(void* args) {

    while (1) {
        // Hacer un wait del semaforo que indica que le toca a IO

        // Empezar a contar el tiempo

        // Si tiempo bloqueo < tiempo maximo -> Bloquear unicamente ese tiempo y pasar a ready

        // Si tiempo bloqueo > tiempo maximo
            // Bloquear tiempo maximo (hilo IO)
            // Pasar proceso a suspended blocked (avisar a planificador mediano plazo para que lo haga)
                // hacer un post a mediano plazo
                // variable transicion -> true
            // Bloquear tiempo restante
            // Pasar a suspended-ready (avisar a planificador mediano plazo para que lo haga)
                // hacer un post a mediano plazo
                // variable transicion -> true

        //Hacer un post del semaforo de IO
    }
}