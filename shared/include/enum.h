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
        CPU_OK,
        MEMORIA_OK, // (MEMORIA -> KERNEL) Respuesta de memoria tras crear las tablas de páginas de proceso.
        NUEVO_PROCESO_READY,
        SOL_TABLA_PAGINAS,

        /* CODIGOS QUE SÍ O SI SON PARA COMUNICARSE CON KERNEL */
        NUEVO_PROCESO,  // Header para paquete que contiene un proceso (CONSOLA -> KERNEL)
        PROCESO_BLOQUEADO, // Header para paquete que contiene un proceso (CPU -> KERNEL)
        PROCESO_FINALIZADO, // Header para paquete que contiene un proceso (CPU -> KERNEL) 
        INTERRUPCION, // Header que se manda sin paquete (KERNEL -> CPU)
        EJECUTAR_PROCESO,

        /*ENUMS PARA COMUNICARSE CON MEMORIA */
        PROCESO_SUSPENDIDO, // (KERNEL -> MEMORIA)
        PROCESO_RECHAZADO, // (MEMORIA -> KERNEL) Respuesta de memoria cuando el tamaño del proceso es mayor al máximo permitido

        PRIMER_ACCESO_MEMORIA, // para conocer en qué tabla de páginas de 2do nivel está direccionado el marco
        SEGUNDO_ACCESO_MEMORIA, // en qué marco está la misma
        TERCER_ACCESO_MEMORIA, // acceder a la porción de memoria correspondiente

        ESCRIBIR_EN_MEMORIA, // se pasa un marco un desplazamiento y un valor
        KERNEL,
        CPU
    } codigo_operacion;

#endif /* ENUM_H */