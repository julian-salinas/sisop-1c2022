#ifndef MEMORIA_CONFIG
#define MEMORIA_CONFIG

    #include <commons/config.h>
    #include <netdb.h>
    #include <stdlib.h>

    typedef struct
    {
        t_config* config;
        char* ip_memoria;
        char* puerto_escucha; // Puerto por el cual se escuchará la conexión de módulo
        uint32_t tamanio_memoria; // Tamaño expresado en bytes
        uint32_t tamanio_pagina; // Tamaño de las páginas en bytes
        uint32_t paginas_por_tabla; // Cantidad de entradas de cada tabla de páginas
        int retardo_memoria; // Tiempo en milisegundos que se deberá esperar para dar una respuesta al CPU
        char* algoritmo_reemplazo; // Algoritmo de reemplazo de páginas (CLOCK/CLOCK-M)
        int marcos_por_proceso; // Cantidad de marcos permitidos por proceso en asignación fija
        int retardo_swap; // Tiempo en milisegundos que se deberá esperar para cada operación del SWAP (leer/escribir)
        char* path_swap; // Carpeta donde se van a encontrar los archivos de SWAP de cada proceso.
    } t_memoria_config;

    t_memoria_config* ini_memoria_config(char* path_a_config);

    void destruir_memoria_config(t_memoria_config* memoria_config);

#endif /* MEMORIA_CONFIG */
