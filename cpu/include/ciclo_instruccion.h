#ifndef CICLO_INSTRUCCION
#define CICLO_INSTRUCCION

#include "proceso.h"


void ejecutar_ciclo_instruccion(t_PCB* pcb, int socket_cliente);


void devolver_pcb(t_PCB* pcb, codigo_operacion header, int socket_cliente);
void traer_operandos(t_instruccion* instruccion, int direccion_logica, uint32_t valor_2);

int parametro_instruccion(t_list* parametros_instruccion, int indice);
uint32_t buscar_operando(int direccion_logica);
void escribir_operando(int direccion_logica, uint32_t valor_1, uint32_t PID);

#endif /* CICLO_INSTRUCCION */