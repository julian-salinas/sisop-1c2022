#include "ciclo_instruccion.h"

#include "cpu.h"
#include "proceso.h" //aca está definido t_PCB, hay que moverlo?
#include "instrucciones.h" //aca está el enum t_identificador y funciones
#include <time.h>
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

// TODO cuando este memoria
void escribir_operando(int direccion_logica, uint32_t valor_1){

}
// TODO cuando este memoria
uint32_t buscar_operando(int direccion_logica){
    /* busca el operando en una direccion */
    return 999;
}

int parametro_instruccion(t_list* parametros_instruccion, int indice){
    return (int) list_get(parametros_instruccion, indice);
}

void traer_operandos(t_instruccion* instruccion, int direccion_logica, uint32_t valor_2){
    //busca en memoria y carga en direccion_logica, valor_2 los valores correspondientes    
    int direccion_logica_aux = parametro_instruccion(instruccion->parametros,1);
    valor_2 = buscar_operando(direccion_logica_aux);
    direccion_logica = parametro_instruccion(instruccion->parametros,0);
}

void devolver_pcb(t_PCB* pcb, codigo_operacion header){
    end_t=time(NULL);
    pcb->program_counter++;
    pcb->tiempo_ejecucion += end_t-start_t; 
  
    log_info(logger,"devolviendo pcb %d",header);
    log_info(logger,"tiempo de ejecucion %d",pcb->tiempo_ejecucion);
    enviar_pcb(conexion_kernel,header, pcb);
    finCicloInstruccion = 1;
}

//TODO MMU 

void ejecutar_ciclo_instruccion(t_PCB* pcb){
    t_instruccion* instruccion;
    int direccion_logica, sleep_time;
    uint32_t valor;
    start_t=time(NULL);
    finCicloInstruccion = 0;
    do{ 
    //Fetch   
    instruccion = list_get(pcb->lista_instrucciones, pcb->program_counter);
    log_info(logger, "Decode instrucción.");
    log_info(logger, "program counter %d", pcb->program_counter);
    log_info(logger,"cantidad de instrucciones: %d", list_size(pcb->lista_instrucciones));
    //Decode
    switch (instruccion->identificador){
        case NO_OP:
            sleep_time = cpu_config->retardo_noop * parametro_instruccion(instruccion->parametros,0) * 1000; 
            //Execute
            log_info(logger,"tiempo de sleep %d", sleep_time);
            log_info(logger, "Ejecutando NO_OP.");
            usleep(sleep_time);
        break;
        case I_O:
            // se bloquea
            log_info(logger, "PCB se bloquea.");
            pcb->estado=BLOCKED;
            pcb->tiempo_bloqueo=parametro_instruccion(instruccion->parametros,0);
            log_info(logger, "Pre devolver PCB.");
            devolver_pcb(pcb, PROCESO_BLOQUEADO);
        break;
        case READ:
            direccion_logica = parametro_instruccion(instruccion->parametros,0);
            valor = buscar_operando(direccion_logica);
            //Execute
            printf("%d",valor);
        break;
        case WRITE:
            direccion_logica = parametro_instruccion(instruccion->parametros,0);
            valor=parametro_instruccion(instruccion->parametros,1);
            //Execute
            escribir_operando(direccion_logica, valor);
        break;
        case COPY:
            //Fetch operands
            traer_operandos(instruccion, direccion_logica, valor);
            //Execute
            escribir_operando(direccion_logica, valor);
        break;
        case EXIT:
            // Syscall finalización de proceso
            log_info(logger, "Finaliza proceso. EXIT");
            pcb->estado=PROCESO_FINALIZADO;
            devolver_pcb(pcb, PROCESO_FINALIZADO);
              
        break;
        }
        pcb->program_counter++;
        log_info(logger, "program counter %d", pcb->program_counter);
    sem_wait(mutex_interrupt);
    if(interrupcion==1){
        finCicloInstruccion=1;
        //Regreso por interrupcion
        pcb->estado=SUSPENDED_BLOCKED;
        devolver_pcb(pcb, INTERRUPCION);
    }
    sem_post(mutex_interrupt);

    }while(!finCicloInstruccion);//Check Interrupt //Ciclo de instruccion
    

}