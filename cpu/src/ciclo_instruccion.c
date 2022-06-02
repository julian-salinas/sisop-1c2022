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

clock_t start_t, end_t;

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

void devolver_pcb(t_PCB* pcb){
    end_t=clock();
    pcb->program_counter++;
    pcb->tiempo_ejecucion += start_t-end_t; 
    //quizás dividir por 1000 para pasarlo a milisegundos??
    enviar_pcb(conexion_kernel,pcb);
}

//TODO MMU 

void ejecutar_ciclo_instruccion(t_PCB* pcb){
    t_instruccion* instruccion;
    int direccion_logica, sleep_time;
    uint32_t valor;
    
    start_t=clock();
    do{ 
    //Fetch    
    instruccion = list_get(pcb->lista_instrucciones, pcb->program_counter);

    //Decode
    switch (instruccion->identificador){
        case NO_OP:
            sleep_time = cpu_config->retardo_noop * parametro_instruccion(instruccion->parametros,0) / 1000; 
            //Execute
            sleep(sleep_time);
        break;
        case I_O:
            // se bloquea
            pcb->estado=BLOCKED;
            pcb->tiempo_bloqueo=parametro_instruccion(instruccion->parametros,0);
            devolver_pcb(pcb);
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
            pcb->estado=EXIT;
            devolver_pcb(pcb);
        break;
        }

        pcb->program_counter++;

    }while(!interrupcion);//Check Interrupt //Ciclo de instruccion

    //Regreso por interrupcion
    pcb->estado=SUSPENDED_BLOCKED;
    devolver_pcb(pcb);

}