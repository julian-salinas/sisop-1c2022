#ifndef ESTRUCTURA_H
#define ESTRUCTURA_H
   /**
     * @DESC: Acá creariamos todas las estructuras que compartan los modulos
     */ 

    typedef struct {
        int PID; // Process ID
        size_t tamanio; // en bytes, el mismo no cambia en ejecución
        t_lista_instrucciones lista_instrucciones;
        int program_counter; // número de la próxima instrucción a ejecutar.
        unit32_t tabla_paginas; //  identificador de tabla de páginas del proceso en memoria
        int estimacion_rafaga; // Valor estimado del tiempo del proceso en cpu
    } t_PCB;

#endif /* ESTRUCTURA_H */