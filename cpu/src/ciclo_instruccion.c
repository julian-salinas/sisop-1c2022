#include "ciclo_instruccion.h"

#include "cpu.h"
#include "proceso.h" //aca está definido t_PCB, hay que moverlo?
#include "instrucciones.h" //aca está el enum t_identificador y funciones

/*
    typedef struct {
        uint32_t PID;
        size_t tamanio; 
        t_lista_instrucciones* lista_instrucciones;
        int program_counter;
        int32_t tabla_paginas;
        int estimacion_rafaga;
    } t_PCB;
*/

// TODO cuando este memoria
void escribir_operando(int direccion_logica, uint32_t valor_1){

}
// TODO cuando este memoria
uint32_t buscar_operando(int direccion_logica){
    /* busca el operando en una direccion */
    return 999;
}

int parametro_instruccion(t_instruccion* instruccion, int index){
    return (int) list_get(instruccion,index);
}

void traer_operandos(t_instruccion* instruccion, int direccion_logica, uint32_t valor_2){
    //busca en memoria y carga en direccion_logica, valor_2 los valores correspondientes    
    int direccion_logica_aux = parametro_instruccion(instruccion,2);
    valor_2 = buscar_operando(direccion_logica_aux);
    direccion_logica = parametro_instruccion(instruccion,1);
}

void ejecutar_ciclo_instruccion(t_PCB* pcb){
    t_instruccion* instruccion;
    int direccion_logica, sleep_time;
    uint32_t valor;

    do{ 
    //Fetch    
    instruccion = list_get(pcb->lista_instrucciones, pcb->program_counter);

    //Decode
    switch (instruccion->identificador){
        case NO_OP:
            sleep_time = cpu_config->retardo_noop * parametro_instruccion(instruccion,1); 
            //Execute
            sleep(sleep_time);
        break;
        case I_O:
            // se bloquea
        break;
        case READ:
            direccion_logica = parametro_instruccion(instruccion,1);
            valor = buscar_operando(direccion_logica);
            //Execute
            printf("%d",valor);
        break;
        case WRITE:
            direccion_logica = parametro_instruccion(instruccion,1);
            valor=parametro_instruccion(instruccion,2);
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
        break;
        }

        pcb->program_counter++;

    }while(!interrupcion);//Check Interrupt //Ciclo de instruccion

}