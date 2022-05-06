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
    #include<commons/txt.h>
    #include "utils.h"

    // Include de las shared
    #include "networking.h"
    #include "config.h"
    #include "logging.h"
    #include "instrucciones.h"

    #define CONFIG_PATH "/home/utnso/tp2022/tp-2022-1c-Grupo-Final-Final/consola/consola.config"

    void terminar_programa(int, t_log*, t_config*);

    /*
     * @DESC: Mapea un string contra su enum correspondiente
     * @param identificador: string a mapear
     */ 
    t_identificador mapear_identificador(char* identificador);

    /*
     * @DESC: Agrega parámetros a una instruccion según identificador
     */ 
    void agregar_parametros(t_identificador identificador, t_instruccion** instruccion, t_list* parametros);

#endif /* CONSOLA_H */
