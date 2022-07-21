#include "ciclo_instruccion.h"
#include "cpu.h"
#include "mmu.h"
#include "tlb.h"
#include "proceso.h"       //aca está definido t_PCB, hay que moverlo?
#include "instrucciones.h" //aca está el enum t_identificador y funciones
#include <time.h>
#include <math.h>

/*
typedef struct {
uint32_t PID;
size_t tamanio; 
t_lista_instrucciones* lista_instrucciones;
int program_counter;
int32_t tabla_paginas;
int estimacion_rafaga;
double tiempo_ejecucion;
int socket_cliente;
estado_proceso estado;
} t_PCB;
*/

time_t start_t, end_t;

void ejecutar_ciclo_instruccion(t_PCB *pcb, int socket_cliente) {
    t_instruccion *instruccion;
    int direccion_logica, sleep_time;
    uint32_t valor;
    start_t = time(NULL);
    finCicloInstruccion = 0;
    int param;
    do
    {
        //Fetch
        instruccion = list_get(pcb->lista_instrucciones, pcb->program_counter);
        log_info(logger, "PID:%d - Decode instrucción - Program counter %d", pcb -> PID, pcb -> program_counter);
        // log_info(logger, "cantidad de instrucciones: %d", list_size(pcb->lista_instrucciones));
        finCicloInstruccion = 0;
        pcb -> program_counter++;
        //Decode
        switch (instruccion->identificador)
        {
        case NO_OP:
            //Execute
            log_info(logger, "Ejecutando NO_OP - Retardo: %d", cpu_config -> retardo_noop);

            for (size_t i = 0; i < cpu_config->retardo_noop; i++)
            {
                usleep(1000);
            }

            log_info(logger, "Finalizó NO_OP");
            break;
        case I_O:
            // se bloquea
            log_warning(logger, "SUSPENSIONNNNNNNNNNNNNNNNNNNN %d", pcb -> PID);
            log_info(logger, "Ejecutando I/O - Proceso %d se bloquea", pcb -> PID);
            pcb->tiempo_bloqueo = parametro_instruccion(instruccion->parametros, 0);
            devolver_pcb(pcb, PROCESO_BLOQUEADO, socket_cliente);
            destruir_PCB(pcb);
            break;
        case READ:
            log_info(logger, "Ejecutando READ");
            direccion_logica = parametro_instruccion(instruccion->parametros, 0);
            valor = buscar_operando(direccion_logica, pcb -> PID);
            //Execute
            log_info(logger, "Se leyó el valor: %d", valor);
            break;
        case WRITE:
            log_info(logger, "Ejecutando WRITE");
            direccion_logica = parametro_instruccion(instruccion->parametros, 0);
            valor = parametro_instruccion(instruccion->parametros, 1);
            //Execute
            log_info(logger, "Se escribió el valor: %d", valor);
            escribir_operando(direccion_logica, valor, pcb->PID);
            break;
        case COPY:
            //Fetch operands
            //traer_operandos(instruccion, direccion_logica, valor, pcb -> PID);
            //Execute
            //escribir_operando(direccion_logica, valor, pcb->PID);
            log_info(logger, "Ejecutando COPY");
            direccion_logica = parametro_instruccion(instruccion -> parametros, 0);
            valor = buscar_operando(direccion_logica, pcb -> PID);
            log_info(logger, "Se va a copiar el valor %d en direccion lógica %d", valor, direccion_logica);
            escribir_operando(parametro_instruccion(instruccion -> parametros, 1), valor, pcb -> PID);
            break;

        case EXIT:
            // Syscall finalización de proceso
            log_info(logger, "Ejecutando EXIT - Finaliza proceso %d", pcb -> PID);
            finCicloInstruccion = 1;
            devolver_pcb(pcb, PROCESO_FINALIZADO, socket_cliente);
            destruir_PCB(pcb);
            break;
        }

        sem_wait(mutex_interrupt);
        
        if (interrupcion == 1)
        {
            log_warning(logger, "Condicional: interrupcion == 1");
            
            if (finCicloInstruccion == 1) {
                enviar_header(socket_cliente, INTERRUPCION_RECHAZADA);
                sem_post(mutex_interrupt);
                interrupcion = 0;
                continue;
            }

            log_info(logger, "Interrumpiendo proceso PID:%d", pcb -> PID);
            finCicloInstruccion = 1;
            //Regreso por interrupcion
            // pcb->estado = BLOCKED;
            devolver_pcb(pcb, INTERRUPCION, socket_cliente);
            interrupcion = 0;
            log_info(logger, "Proceso PID:%d interrumpido", pcb -> PID);
            destruir_PCB(pcb);
        }
        sem_post(mutex_interrupt);
    } while (!finCicloInstruccion); //Check Interrupt //Ciclo de instruccion
}

// Escribe en una dirección lógica un determinada un valor
void escribir_operando(int direccion_logica, uint32_t valor_1, uint32_t PID)
{
    uint32_t marco, desplazamiento;

    //mmu carga el marco y el desplazamiento
    int direccion_fisica = mmu(direccion_logica, marco, desplazamiento, PID);

    t_paquete *paquete = crear_paquete(ESCRIBIR_EN_MEMORIA, sizeof(uint32_t) * 2);
    // agregar_a_buffer_UINT32(paquete->payload, (uint32_t)marco);
    // agregar_a_buffer_UINT32(paquete->payload, (uint32_t)desplazamiento);
    agregar_a_buffer_INT32(paquete->payload, (int32_t)direccion_fisica);
    agregar_a_buffer_UINT32(paquete->payload, (uint32_t)valor_1);
    enviar_paquete(conexion_memoria, paquete);
    destruir_paquete(paquete);

    uint8_t resp_memoria = recibir_header(conexion_memoria);
    if (resp_memoria != MEMORIA_OK) {
        log_error(logger, "Falló la escritura");
    }
    else {
        log_info(logger, "Escritura OK");
    }
}

// Busca en una dirección lógica determinada un valor
uint32_t buscar_operando(int direccion_logica, uint32_t PID)
{

    uint32_t marco, desplazamiento;

    //mmu carga el marco y el desplazamiento
    int direccion_fisica = mmu(direccion_logica, marco, desplazamiento, PID);

    uint32_t operando = acceso_a_memoria_3(LEER_MEMORIA, direccion_fisica);

    return operando;
}

int parametro_instruccion(t_list *parametros_instruccion, int indice)
{
    return (int)list_get(parametros_instruccion, indice);
}

void traer_operandos(t_instruccion *instruccion, int direccion_logica, uint32_t valor_2, uint32_t PID)
{
    //busca en memoria y carga en direccion_logica, valor_2 los valores correspondientes
    int direccion_logica_aux = parametro_instruccion(instruccion->parametros, 1);
    valor_2 = buscar_operando(direccion_logica_aux, PID);
    direccion_logica = parametro_instruccion(instruccion->parametros, 0);
}

// devuelve la pcb a kernel
void devolver_pcb(t_PCB *pcb, codigo_operacion header, int socket_cliente)
{
    end_t = time(NULL);
    pcb->tiempo_ejecucion += end_t - start_t;
    log_info(logger, "Devolviendo PCB - Header %d - PID:%d - Tiempo de ejecucion:%f", header, pcb->PID, pcb->tiempo_ejecucion);
    enviar_pcb(socket_cliente, header, pcb);
    //limpieza de tlb
    log_warning(logger, "Limpiando TLB");
    limpiar_tlb2();

    finCicloInstruccion = 1;
}