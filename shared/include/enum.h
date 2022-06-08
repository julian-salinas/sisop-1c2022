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
        NUEVO_PROCESO, // esto es lo que le manda consola a kernel
        CPU_OK,
        MEMORIA_OK,
        NUEVO_PROCESO_READY,
        PROCESO_FINALIZADO,
        PROCESO_BLOQUEADO,
        SOL_TABLA_PAGINAS,
    } codigo_operacion;

#endif /* ENUM_H */