#ifndef ENUM_H
#define ENUM_H
   /**
     * @DESC: Esto es muy inicial todavía, acá vamos a estar
     *        declarando los diferentes códigos de operacion posibles
     *        por ahora, solo dejo uno para poder ir testeando el uso
     *        de paquetes
     */ 
    typedef enum {
        CONFIGS,
        MENSAJE,
        PAQUETE,
        INSTRUCCIONES,
        CONEXION_CPU_MEMORIA,
        PCB,
        BLOQUEO,
        FINALIZACION,
        INTERRUPCION,
        CPU_OK,
        MEMORIA_OK,
        NUEVO_PROCESO_READY,
        SOL_TABLA_PAGINAS,

        /* CODIGOS QUE SÍ O SI SON PARA COMUNICARSE CON KERNEL */
        NUEVO_PROCESO,  // Header para paquete que contiene un proceso (CONSOLA -> KERNEL)
        PROCESO_BLOQUEADO, // Header para paquete que contiene un proceso (CPU -> KERNEL)
        PROCESO_FINALIZADO, // Header para paquete que contiene un proceso (CPU -> KERNEL) 
        DESALOJAR_PROCESO, // Header que se manda sin paquete (KERNEL -> CPU)
    } codigo_operacion;

#endif /* ENUM_H */