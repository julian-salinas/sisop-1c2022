#include "planificador_kernel.h"

int calcular_estimacion(t_PCB* pcb){
    //inicializo config para el alfa
    t_kernel_config* kernel_config = ini_kernel_config("cfg/kernel.config");
    int estimacion_actual = 0, tiempo_ejecucion = 0;
    float alfa = kernel_config -> alfa;

    estimacion_actual = pcb -> estimacion_rafaga;

    //falta que llegue la info del tiempo_ejecucion desde CPU

    estimacion_actual = alfa * tiempo_ejecucion + (1 - alfa) * estimacion_actual;  

    pcb ->  estimacion_rafaga = estimacion_actual;

    destruir_kernel_config(kernel_config);

    return estimacion_actual;
}
