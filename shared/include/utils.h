
#ifndef UTILS_H
#define UTILS_H

    #include <commons/log.h>
    #include <sys/socket.h>
    #include <commons/string.h>
    #include "networking.h"
    #include <semaphore.h>
 
    /**
     * @DESC: Loggear que módulo está siendo finalizado, liberar conexion y destruir logger 
     */ 
    void terminar_programa(char* module_name, int conexion, t_log* logger);

    /**
     * @DESC: Pasar un numerito entero a un stringcito
     */
    char* int_a_string(int numerito); 

    /**
     * @DESC: Realiza la división a/b y retorna el resultado de redondear ese valor para arriba
     */ 
    int round_div_up(int a, int b);

    /**
     * @DESC: Hace la conversión de hexadecimal a int
     * @param puntero: Va a agarrar su dirección de memoria para convertirla en decimal
     * @return: conversión a entero
     */ 
    int hexa_a_int(void* puntero);


    void mutex_log_info(sem_t* semaforo, t_log* logger, char* mensaje);

    void mutex_log_warning(sem_t* semaforo, t_log* logger, char* mensaje);

    void mutex_log_error(sem_t* semaforo, t_log* logger, char* mensaje);

#endif /* UTILS_H */