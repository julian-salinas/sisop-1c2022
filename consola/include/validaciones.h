#ifndef VALIDACIONES_H
#define VALIDACIONES_H

    #include <stdio.h>
    #include <stdlib.h>

    #include "consola.h"
    #include "networking.h"
    #include "logging.h"

    /**
     * @DESC: Libera variables alojadas en el heap de consola
     * @param socket_servidor: conexion con kernel
     * @param consola_config: variable que contiene configs de consola
     */ 
    void free_consola(int socket_servidor, t_consola_config* consola_config);
    

    /** 
     * @DESC: Valida que la cantidad de argumentos enviados al momento de instanciar una consola
     *        es mayor o igual a 3
     * @param argc: numero entero que recibe main como primer parámetro
     * @return: en caso de error, retorna 0, en caso de exito, retorna 1
     */ 
    int validar_argumentos(int argc);


    /**
     * @DESC: Valida que se pueda abrir un archivo. Recibe la variable del archivo y el path al mismo (para logging)
     * @param file: variable con el archivo abierto
     * @param path: path al archivo de pseudocódigo
     * @return: en caso de error, retorna 0, en caso de exito, retorna 1
     */ 
    int validar_apertura_archivo(FILE* file, char* path);    

#endif /* VALIDACIONES_H */