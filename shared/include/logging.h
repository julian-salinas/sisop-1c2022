#ifndef LOGGING_H
#define LOGGING_H

    #include<stdio.h>
    #include<stdlib.h>
    #include<string.h>
    #include<commons/log.h>


    /**
     * @DESC: Usa la función log_create para crear un nuevo logger con los siguientes valores
     *       - is_active_console: 1
     *       - t_log_level: LOG_LEVEL_INFO
     * 
     * @param file: Path al archivo .log donde se va a loggear, si el archivo no existe, se crea uno
     * @param nombre_proceso: nombre del proceso que empleará el logger
     * @return t_log*: Devuelve un puntero a t_log con un logger listo para utilizar
     */
    t_log* iniciar_logger(char* file, char* process_name);



#endif /* LOGGING_H */