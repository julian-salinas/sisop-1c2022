#ifndef PARSER_H
#define PARSER_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <commons/string.h>
    #include <commons/txt.h>

    #include "consola.h"
    #include "identificador.h"
    #include "instrucciones.h"

    /**
     * @DESC: Parsea pseudocódigo y retorna lista de instrucciones con las líneas
     *        del mismo
     * @param pseudo_file: archivo abierto y ya validado que contiene pseudocódigo
     * @return: lista de instrucciones
     */ 
    t_lista_instrucciones* parsear_pseudocodigo(FILE* pseudo_file);

#endif /* PARSER_H */