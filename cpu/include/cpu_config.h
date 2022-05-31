#ifndef CPU_CONFIG
#define CPU_CONFIG

    #include "networking.h"
    #include "serializacion.h"

    typedef struct
    {
        t_config* config;
        int entradas_tlb; //Cantidad de entradas de la TLB
        char* reemplazo_tlb; //Algoritmo de reemplazo para las entradas de la TLB
        int retardo_noop; //Tiempo en milisegundos que se deberá esperar al ejecutar una instrucción NoOp
        char* ip_memoria; //IP a la cual se deberá conectar con la Memoria
        char* puerto_memoria; //Puerto al cual se deberá conectar con la Memoria
        char* puerto_escucha_dispatch; //Puerto en el cual se escuchará la conexión del Kernel para mensajes de dispatch
        char* puerto_escucha_interrupt; //Puerto en el cual se escuchará la conexión del Kernel para mensajes de interrupciones
    } t_cpu_config;

    t_cpu_config* ini_cpu_config(char* path_a_config);

    void destruir_cpu_config(t_cpu_config* cpu_config);

#endif /* MEMORIA_CONFIG */