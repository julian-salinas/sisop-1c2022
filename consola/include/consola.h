/*
 ============================================================================
 Name        : consola.h
 Author      : AP
 Version     : 
 Description : Archivo que contiene las firmas de todas las funciones de la consola.
 ============================================================================
 */

#ifndef CONSOLA_H_
#define CONSOLA_H_

    #include <stdio.h>
    #include <stdlib.h>
    #include <commons/string.h>
    #include <commons/txt.h>
    
    // Includes del módulo
    #include "consola_config.h"
    #include "identificador.h"
    #include "parser.h"
    #include "validaciones.h"
    // Includes de shared
    #include "networking.h"
    #include "logging.h"
    #include "instrucciones.h"
    #include "proceso.h"
    #include "utils.h"


    t_log* logger_consola;
    
#endif /* CONSOLA_H */
