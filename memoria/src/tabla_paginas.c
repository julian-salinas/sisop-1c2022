#include "tabla_paginas.h"


t_tabla_primer_nivel* crear_tabla_primer_nivel(void) {
    t_tabla_primer_nivel* tmp = malloc(sizeof(t_tabla_primer_nivel));
    tmp -> cantidad_entradas = 0;
    tmp -> entradas = NULL;
    return tmp;
}


void destruir_tabla_primer_nivel(t_tabla_primer_nivel* tabla) {
    free(tabla -> entradas);
    free(tabla);
}


t_tabla_segundo_nivel* crear_tabla_segundo_nivel(void) {
    t_tabla_segundo_nivel* tmp = malloc(sizeof(t_tabla_segundo_nivel));
    tmp -> cantidad_entradas = 0;
    tmp -> entradas = NULL;
    return tmp;    
}


void destruir_tabla_segundo_nivel(t_tabla_segundo_nivel* tabla) {
    free(tabla -> entradas);
    free(tabla);
} 
