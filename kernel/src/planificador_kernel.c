#include "planificador_kernel.h"
#include "kernel.h"

int calcular_estimacion(t_PCB* pcb){
    //inicializo config para el alfa
    int estimacion_actual = 0, tiempo_ejecucion = 0;
    float alfa = kernel_config -> alfa;

    estimacion_actual = pcb -> estimacion_rafaga;

    //falta que llegue la info del tiempo_ejecucion desde CPU

    estimacion_actual = alfa * tiempo_ejecucion + (1 - alfa) * estimacion_actual;  

    pcb ->  estimacion_rafaga = estimacion_actual;

    destruir_kernel_config(kernel_config);

    return estimacion_actual;
}

void elegir_algoritmo(char* algoritmo) {
    
    if(strcmp(algoritmo,"FIFO") == 0) {
        algoritmo_elegido = FIFO;
        log_info(logger, "Se eligio el algoritmo FIFO");
    }

    if(strcmp(algoritmo,"SRT") == 0) {
        algoritmo_elegido = SRT;
        log_info(logger, "Se eligio el algoritmo SRT");
    }
}