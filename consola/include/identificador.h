#ifndef IDENTIFICADOR_H
#define IDENTIFICADOR_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <commons/string.h>
    #include "instrucciones.h"

    /*
     * @DESC: Mapea un string contra su enum correspondiente
     * @param identificador: string a mapear
    */
    t_identificador mapear_identificador(char* identificador);

    /*
     * @DESC: Agrega parámetros a una instruccion según identificador
    */ 
    void agregar_parametros(t_identificador identificador, t_instruccion* instruccion, t_list* parametros);
    

#endif /* CONSOLA_H */
