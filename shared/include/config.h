#ifndef CONFIG_H
#define CONFIG_H

    #include<commons/config.h>

    /**
     * @DESC: Usar la función config_create para crear y devuelve un puntero a una estructura t_config
     * @param path: puntero a char con el archivo de configuración a ser leído
     * @return: Devuelve un puntero a t_config
     */ 
    t_config* iniciar_config(char* path);

#endif /* CONFIG_H */