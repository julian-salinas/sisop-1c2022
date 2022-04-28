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

    #include<stdio.h>
    #include<stdlib.h>
    #include<commons/log.h>
    #include<commons/string.h>
    #include<commons/config.h>
    #include<readline/readline.h>
    #include "utils.h"

    // Include de las shared
    #include "networking.h"
    #include "config.h"
    #include "logging.h"

    #define CONFIG_PATH "/home/utnso/tp2022/tp-2022-1c-Grupo-Final-Final/consola/consola.config"

    void terminar_programa(int, t_log*, t_config*);

#endif /* CONSOLA_H */
