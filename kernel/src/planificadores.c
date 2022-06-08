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

void finalizar_semaforos_plani() {
    free(mutex_cola_new);
    free(mutex_cola_ready);
    free(mutex_cola_exit);
    free(mutex_cola_blocked);
    free(mutex_cola_suspended_ready);
    free(mutex_proceso_corriendo);
    free(mutex_proceso_buscado);
    
    free(mutex_pid);
    free(mutex_mediano_plazo);
    free(sem_multiprogramacion);
    free(sem_mediano_plazo);
    free(sem_corto_plazo);
    free(sem_largo_plazo);
}


void* func_corto_plazo(void* args){
    t_PCB* procesoAMover;
    
    while (1) {
        sem_wait(sem_corto_plazo);

        if (transicion_running_a_blocked) {
            // Agarrar variable global que contiene PCB y agregarlo a cola blocked
            running_a_blocked(proceso_desalojado);

            // Empezar a contabilizar tiempo bloqueado ¿cuanto tiempo tiene que bloquearse?

        }

        else if (transicion_ready_a_running) {

            if ((algoritmo_elegido == SJF) && (proceso_corriendo)) {
                enviar_header(conexion_cpu, INTERRUPCION);  // Avisar a CPU para que desaloje proceso actual
                t_paquete* resp_cpu = recibir_paquete(conexion_cpu);
                procesoAMover = buffer_take_PCB(resp_cpu -> payload);
                running_a_ready(procesoAMover);
                ordenar_cola_ready();  // Solo es necesario ordenar la cola ready cuando usamos SJF
                log_info(logger, "Se reordenó la cola READY.");
            }

            procesoAMover = ready_a_running(); // Tomar un proceso de la cola ready y cambiar su estado
            enviar_pcb(conexion_cpu, PCB, procesoAMover); // Pasarle el proceso a CPU para que lo ejecute
        }
    }
}


void* func_mediano_plazo(void* args) {

    while (1) {
        sem_wait(sem_multiprogramacion);
        sem_wait(sem_mediano_plazo);
        sem_wait(mutex_mediano_plazo);

            if(cola_esta_vacia(cola_suspended_ready)) {
                sem_post(sem_largo_plazo);
                sem_post(mutex_mediano_plazo);
                continue;
            }

            suspended_ready_a_ready();

            sem_wait(mutex_transicion_ready_a_running);
                transicion_ready_a_running = true;
            sem_post(mutex_transicion_ready_a_running);

            sem_post(sem_corto_plazo);  // Despertar planificador de corto plazo

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
            sem_post(sem_corto_plazo);
            
        } else if (transicion_running_a_exit) {
            running_a_exit(proceso_desalojado);  // Pasar proceso a exit
            enviar_header(proceso_desalojado -> socket_cliente, PROCESO_FINALIZADO); // Informar a consola que finalizó
            transicion_running_a_exit = false; // devolver variable a false
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