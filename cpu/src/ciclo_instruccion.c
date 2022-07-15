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
        log_info(logger, "Decode instrucción.");
        log_info(logger, "program counter %d", pcb->program_counter);
        log_info(logger, "cantidad de instrucciones: %d", list_size(pcb->lista_instrucciones));
        finCicloInstruccion = 0;
        pcb -> program_counter++;
        //Decode
        switch (instruccion->identificador)
        {
        case NO_OP:
            //Execute
            param = parametro_instruccion(instruccion -> parametros, 0);
            log_info(logger, "parametro_instruccion recibido: %d", param);
            sleep_time = cpu_config->retardo_noop * param;
            log_info(logger, "Ejecutando NO_OP.");
            log_info(logger, "tiempo de sleep %d", sleep_time);

            for (size_t i = 0; i < sleep_time; i++)
            {
                usleep(1000);
            }

            log_info(logger, "Terminó usleep");
            break;
        case I_O:
            // se bloquea
            log_info(logger, "PCB se bloquea.");
            pcb->estado = BLOCKED;
            pcb->tiempo_bloqueo = parametro_instruccion(instruccion->parametros, 0);
            log_info(logger, "Pre devolver PCB.");
            devolver_pcb(pcb, PROCESO_BLOQUEADO, socket_cliente);
            break;
        case READ:
            direccion_logica = parametro_instruccion(instruccion->parametros, 0);
            valor = buscar_operando(direccion_logica, pcb -> PID);
            //Execute
            log_info(logger, "LEYENDO %d", valor);
            break;
        case WRITE:
            direccion_logica = parametro_instruccion(instruccion->parametros, 0);
            valor = parametro_instruccion(instruccion->parametros, 1);
            //Execute
            log_info(logger, "ESCRIBIENDO %d", valor);
            escribir_operando(direccion_logica, valor, pcb->PID);
            break;
        case COPY:
            //Fetch operands
            //traer_operandos(instruccion, direccion_logica, valor, pcb -> PID);
            //Execute
            //escribir_operando(direccion_logica, valor, pcb->PID);
            log_info(logger, "COPYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY");
            direccion_logica = parametro_instruccion(instruccion -> parametros, 0);
            valor = buscar_operando(direccion_logica, pcb -> PID);
            escribir_operando(parametro_instruccion(instruccion -> parametros, 1), valor, pcb -> PID);
            break;

        case EXIT:
            // Syscall finalización de proceso
            finCicloInstruccion = 1;
            log_info(logger, "Finaliza proceso. EXIT");
            pcb->estado = PROCESO_FINALIZADO;
            devolver_pcb(pcb, PROCESO_FINALIZADO, socket_cliente);
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

            log_info(logger, "DEVOLVIENDO POR INTERRUPCION");
            finCicloInstruccion = 1;
            //Regreso por interrupcion
            // pcb->estado = BLOCKED;
            devolver_pcb(pcb, INTERRUPCION, socket_cliente);
            interrupcion = 0;
            log_info(logger, "YA DEVOLVÍ POR INTERRUPCION");
        }
        sem_post(mutex_interrupt);
    log_info(logger, "Linea 121");
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
    log_info(logger, "devolviendo pcb %d - PID:%d", header, pcb->PID);
    log_info(logger, "tiempo de ejecucion %f", pcb->tiempo_ejecucion);
    enviar_pcb(socket_cliente, header, pcb);
    log_info(logger, "pcb devolvido");
    //limpieza de tlb
    limpiar_tlb2();

    finCicloInstruccion = 1;
}