
#ifndef UTILS_H
#define UTILS_H

    #include <commons/log.h>
    #include <sys/socket.h>
    #include <commons/string.h>
    #include "networking.h"
 
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

#endif /* UTILS_H */