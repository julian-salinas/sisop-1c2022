#include "procesar_conexion.h"

void procesar_conexion(void *args)
{

    t_procesar_conexion_args *casted_args = (t_procesar_conexion_args *)args;
    t_log *logger = casted_args->log;
    int socket = casted_args->fd;
    char *nombre_servidor = casted_args->server_name;
    free(casted_args);

    int header = recibir_header(socket);
    log_info(logger, "Se recibió el cod operacion %d  - %s", header, nombre_servidor);
    t_PCB *pcb;

    switch (header)
    {

    case NUEVO_PROCESO:
        pcb = socket_create_PCB(socket);
        agregar_a_new(pcb); // Se agrega proceso a cola NEW y se actualiza su estado
        break;

    case -1:
        log_error(logger, "Cliente desconectado de %s", nombre_servidor);
        break;

    default:
        log_error(logger, "El codigo de operacion %d es incorrecto - %s", header, nombre_servidor);
        break;
    }
}

void procesar_conexion_dispatch(void *args)
{
    uint32_t header;
    t_PCB *pcb;

    while (1)
    {
        header = recibir_header(conexion_cpu_dispatch);
        log_info(logger, "Se recibió header de CPU:%d", header);
        sem_wait(mutex_socket_cpu_dispatch);
        switch (header)
        {

        case PROCESO_FINALIZADO:

            sem_wait(mutex_proceso_corriendo);
                proceso_corriendo = false;
            sem_post(mutex_proceso_corriendo);

            pcb = socket_get_PCB(conexion_cpu_dispatch);
            sem_post(mutex_socket_cpu_dispatch);
            log_info(logger, "Proceso finalizado: %d", pcb->PID);

            pcb -> estado = EXIT;

            sem_post(sem_cpu_disponible);

            sem_wait(mutex_socket_memoria);
            enviar_pcb(conexion_memoria, PROCESO_FINALIZADO, pcb); // Avisarle a memoria para que desaloje al proceso
            sem_post(mutex_socket_memoria);

            enviar_header(pcb->socket_cliente, PROCESO_FINALIZADO); // Avisarle a consola que terminó la ejecución
            sem_post(sem_multiprogramacion);                        // Se libera multiprog. después de sacar al proceso de memoria

            if (algoritmo_elegido == SJF)
            {
                sem_wait(mutex_cola_ready);
                if (!queue_is_empty(cola_ready))
                {
                    sem_post(mutex_cola_ready);

                    ordenar_cola_ready();
                    log_info(logger, "Se reordenó la cola READY usando el algoritmo SJF luego de interrupcion.");

                    ready_a_running(); // Tomar un proceso de la cola ready y cambiar su estado
                }
                else
                {
                    sem_post(mutex_cola_ready);
                }
            }

            break;

        case PROCESO_BLOQUEADO:;
            pcb = socket_get_PCB(conexion_cpu_dispatch); // Obtener pcb del proceso bloqueado
            sem_post(mutex_socket_cpu_dispatch);

            sem_wait(mutex_proceso_corriendo);
            proceso_corriendo = false;
            sem_post(mutex_proceso_corriendo);

            sem_post(sem_cpu_disponible);

            log_info(logger, "Se recibió proceso bloqueado por %d ms", pcb->tiempo_bloqueo);

            ajustar_estimacion(pcb);

            running_a_blocked(pcb); // Pasar a cola blocked

            // Iniciar hilo que se va a encargar de suspender al proceso en caso de que se zarpe de tiempo
            pthread_t thread_suspension;
            pthread_create(&thread_suspension, 0, (void *)func_suspension, (void *)pcb);
            pthread_detach(thread_suspension);

            if (algoritmo_elegido == SJF)
            {
                sem_wait(mutex_cola_ready);
                if (!queue_is_empty(cola_ready))
                {
                    sem_post(mutex_cola_ready);

                    ordenar_cola_ready();
                    log_info(logger, "Se reordenó la cola READY usando el algoritmo SJF.");

                    ready_a_running(); // Tomar un proceso de la cola ready y cambiar su estado
                }
                else
                {
                    sem_post(mutex_cola_ready);
                }
            }

            break;

        case INTERRUPCION:
            sem_wait(mutex_proceso_corriendo);
            proceso_corriendo = false;
            sem_post(mutex_proceso_corriendo);
            pcb = socket_get_PCB(conexion_cpu_dispatch);
            sem_post(mutex_socket_cpu_dispatch);

            log_info(logger, "Se recibió proceso interrumpido: PID:%d", pcb->PID);

            pcb -> tiempo_restante = pcb -> estimacion_rafaga - pcb -> tiempo_ejecucion;

            running_a_ready(pcb);

            ordenar_cola_ready();
            
            log_info(logger, "Se reordenó la cola READY usando el algoritmo SJF.");

            ready_a_running(); // Tomar un proceso de la cola ready y cambiar su estado

            break;

        case INTERRUPCION_RECHAZADA:
            sem_post(mutex_socket_cpu_dispatch);
            log_info(logger, "Interrupcion rechazada - no hay procesos en cpu");
            break;

        case -1:
            sem_post(mutex_socket_cpu_dispatch);
            log_error(logger, "¡Hubo un problema con la conexión de CPU!");
            return;

        default:
            sem_post(mutex_socket_cpu_dispatch);
            log_error(logger, "El codigo de operacion es incorrecto - %d", header);
            return;
        }
    }
}

t_PCB *crear_PCB(t_proceso *proceso, int socket)
{

    t_PCB *pcb = malloc(sizeof(t_PCB));
    sem_wait(mutex_pid);
    pcb->PID = contador_id_proceso;
    sem_post(mutex_pid);
    pcb->tamanio = proceso->tamanio;
    pcb->lista_instrucciones = proceso->lista_instrucciones;
    pcb->program_counter = 0;
    pcb->tabla_paginas = -1;
    pcb->estimacion_rafaga = kernel_config->estimacion_inicial;
    pcb->tiempo_ejecucion = 0.0;
    pcb->socket_cliente = socket;
    pcb->tiempo_bloqueo = -1;
    pcb->estado = NEW;
    pcb->tiempo_restante = 0.0;

    sem_wait(mutex_pid);
    contador_id_proceso++;
    sem_post(mutex_pid);

    return pcb;
}

t_PCB *socket_create_PCB(int socket)
{
    t_buffer *payload = recibir_payload(socket);
    t_proceso *proceso = buffer_take_PROCESO(payload);
    t_PCB *pcb = crear_PCB(proceso, socket);
    destruir_buffer(payload);

    return pcb;
}
