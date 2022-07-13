#include "planificadores.h"


void ajustar_estimacion(t_PCB* pcb) {
    pcb -> estimacion_rafaga = pcb -> tiempo_ejecucion * kernel_config -> alfa + 
                                (1 - kernel_config -> alfa) * pcb -> estimacion_rafaga;
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
    free(mutex_cola_blocked);
    free(mutex_cola_suspended_ready);
    
    free(mutex_pid);
    free(mutex_mediano_plazo);
    free(sem_multiprogramacion);
    free(sem_largo_plazo);
}


void func_corto_plazo(void* args) {
    t_PCB* procesoAMover;

    while (1) {
        if (algoritmo_elegido == FIFO) {
            sem_wait(sem_cpu_disponible);
            sem_wait(sem_procesos_en_ready);
            ready_a_running();
        }

        else {
            sem_wait(sem_procesos_en_ready);

            if (proceso_corriendo) {
                sem_wait(mutex_socket_cpu_interrupt);
                    enviar_header(conexion_cpu_interrupt, INTERRUPCION);  // Avisar a CPU para que desaloje proceso actual
                    omitir_header(conexion_cpu_interrupt); // No nos interesa el header que se recibe, solo queremos el PCB
                    free(procesoAMover);
                    procesoAMover = socket_get_PCB(conexion_cpu_interrupt);
                sem_post(mutex_socket_cpu_interrupt);
                // Agregar tiempo restante al PCB
                procesoAMover -> tiempo_restante = procesoAMover -> estimacion_rafaga - procesoAMover -> tiempo_ejecucion;

                running_a_ready(procesoAMover);
            }

            ordenar_cola_ready();
            log_info(logger, "Se reordenó la cola READY usando el algoritmo SJF.");
            
            sem_wait(mutex_cola_ready);
            // Imprimir cola ready
            for (int i = 0; i < queue_size(cola_ready); i++) {
                log_warning(logger, "Proceso posicion %d: %d", i, ((t_PCB*) list_get(cola_ready -> elements, i)) -> PID);
            }
            sem_post(mutex_cola_ready);

            ready_a_running(); // Tomar un proceso de la cola ready y cambiar su estado
        }
    }
}



void func_mediano_plazo(void* args) {

    while (1) {
        sem_wait(sem_multiprogramacion); // Hay grado de multiprogramación disponible
        sem_wait(sem_procesos_esperando); // Hay algo para planificar
        sem_wait(mutex_mediano_plazo);
            if(queue_is_empty(cola_suspended_ready)) {
                sem_post(sem_largo_plazo);
                sem_post(mutex_mediano_plazo);
                continue; 
            }
            suspended_ready_a_ready();
        sem_post(mutex_mediano_plazo);
    }
}


void func_largo_plazo(void* args){
    while (1) {
        // No es necesario hacer un wait multiprogramación, ya la hizo la función del planif. mediano plazo
        sem_wait(sem_largo_plazo);
        sem_wait(mutex_mediano_plazo);
            new_a_ready();
        sem_post(mutex_mediano_plazo);
    }
}


void ordenar_cola_ready(void) {
    if (algoritmo_elegido == SJF) {
        sem_wait(mutex_cola_ready);
            list_sort(cola_ready -> elements, (void*) *(algoritmo_SJF));
        sem_post(mutex_cola_ready);
    }
}


bool algoritmo_SJF(t_PCB* proceso1, t_PCB* proceso2) {
    double p1 = proceso1 -> tiempo_restante ? proceso1 -> tiempo_restante : proceso1 -> estimacion_rafaga;
    double p2 = proceso2 -> tiempo_restante ? proceso2 -> tiempo_restante : proceso2 -> estimacion_rafaga;

    return (p1 <= p2);
}


void func_io(void* args) {

    while (1) {
        // Hacer un wait del semaforo que indica que hay procesos en blocked
        sem_wait(sem_procesos_bloqueados);

        sem_wait(mutex_cola_blocked);
            t_PCB* proceso = (t_PCB*) queue_pop(cola_blocked);
        sem_post(mutex_cola_blocked);

        int tiempo_bloqueo = proceso -> tiempo_bloqueo * 1000; // microseg -> miliseg
        
        log_info(logger, "Arranca IO del proceso ID:%d", proceso -> PID);

        usleep(tiempo_bloqueo);

        sem_wait(mutex_suspension);

            if (proceso -> estado == BLOCKED) {
                sem_wait(mutex_mediano_plazo);
                    blocked_a_ready(proceso);
                sem_post(mutex_mediano_plazo);
            }

            if (proceso -> estado == SUSPENDED_BLOCKED) {
                sem_wait(mutex_mediano_plazo);
                    suspended_blocked_a_suspended_ready(proceso);
                sem_wait(mutex_mediano_plazo);
            }

        sem_post(mutex_suspension);
    }
}


void func_suspension(void* args) {
    t_PCB* proceso = (t_PCB*) args;

    usleep(kernel_config -> tiempo_maximo_bloqueado * 1000);

    sem_wait(mutex_suspension);
        // Si el proceso sigue estando realizando IO, suspenderlo
        if (proceso -> estado == BLOCKED) {
            blocked_a_suspended_blocked(proceso);
        }

    sem_post(mutex_suspension);
}