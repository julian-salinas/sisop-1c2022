#ifndef UTILS_MEMORIA_H_
#define UTILS_MEMORIA_H_

#include "networking.h"
#include "serializacion.h"

typedef struct
{
	int puerto_escucha; // Puerto por el cual se escuchará la conexión de módulo
	int tam_memoria; // Tamaño expresado en bytes
	int tam_pagina; // Tamaño de las páginas en bytes
	int paginas_por_tabla; // Cantidad de entradas de cada tabla de páginas
	int retardo_memoria; // Tiempo en milisegundos que se deberá esperar para dar una respuesta al CPU
	char* algoritmo_reemplazo; // Algoritmo de reemplazo de páginas (CLOCK/CLOCK-M)
	int marcos_por_proceso; // Cantidad de marcos permitidos por proceso en asignación fija
	int retardo_swap; // Tiempo en milisegundos que se deberá esperar para cada operación del SWAP (leer/escribir)
	char* path_swap; // Carpeta donde se van a encontrar los archivos de SWAP de cada proceso.
} Memoria_config;

Memoria_config config_data;
t_config* config_memoria;

void ini_config(void);

#endif /* UTILS_MEMORIA_H_ */
