
#ifndef UTILS_H
#define UTILS_H

    #include <commons/log.h>
    #include <sys/socket.h>
    #include "networking.h"
 
    /**
     * @DESC: Loggear que módulo está siendo finalizado, liberar conexion y destruir logger 
     */ 
    void terminar_programa(char* module_name, int conexion, t_log* logger);

#endif /* UTILS_H */