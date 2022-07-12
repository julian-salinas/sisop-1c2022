#include "mmu.h"

//MMU FALTA USO DE TLB
int mmu(int direccion_logica, uint32_t marco, uint32_t desplazamiento, uint32_t PID)
{
    uint32_t nro_tabla_segundo_nivel;
    uint32_t entrada_tabla_2do_nivel;

    uint32_t numero_pagina = (uint32_t)floor(direccion_logica / tamanio_pagina);
    uint32_t entrada_tabla_1er_nivel = (int32_t)floor(numero_pagina / paginas_por_tabla);

    //CHEQUEO EN TLB
    marco = buscar_entrada_tlb(tlb, numero_pagina);
    if (!marco)
    {
        //SI NO ENCUENTRA
        //primer acceso a memoria
        log_info(logger, "Enviando PID %d y entrada tabla primer nivel %d", PID, entrada_tabla_1er_nivel);
        nro_tabla_segundo_nivel = acceso_a_memoria(PRIMER_ACCESO_MEMORIA, PID, entrada_tabla_1er_nivel);
        // agregar_entrada_tlb(tlb, numero_pagina, nro_tabla_segundo_nivel);
        
        // Segundo acceso a memoria
        marco = acceso_a_memoria_2(SEGUNDO_ACCESO_MEMORIA, nro_tabla_segundo_nivel, numero_pagina, PID);

        // Agregar a tlb -> numero pagina | numero marco
        agregar_entrada_tlb(tlb, numero_pagina, marco);

        return marco * tamanio_pagina + desplazamiento;
    }
    else
    {
        return marco * tamanio_pagina + desplazamiento;

        // //nos ahorramos una entrada
        // //segundo acceso a memoria
        // entrada_tabla_2do_nivel = (uint32_t)numero_pagina % paginas_por_tabla;

        // marco = acceso_a_memoria(SEGUNDO_ACCESO_MEMORIA, nro_tabla_segundo_nivel, entrada_tabla_2do_nivel);
        // desplazamiento = (direccion_logica - (numero_pagina * tamanio_pagina));
    }
}

// funcion auxiliar genérica para accesos a memoria
uint32_t acceso_a_memoria(codigo_operacion header, uint32_t valor1, uint32_t valor2)
{
    uint32_t rta_memoria;

    t_paquete *paquete = crear_paquete(header, sizeof(uint32_t) * 2);
    agregar_a_buffer_UINT32(paquete->payload, (uint32_t)valor1);
    agregar_a_buffer_UINT32(paquete->payload, (uint32_t)valor2);
    enviar_paquete(conexion_memoria, paquete);
    destruir_paquete(paquete);

    uint8_t resp_memoria = recibir_header(conexion_memoria);

    if (resp_memoria != MEMORIA_OK)
    {
        log_info(logger, "Ha ocurrido un error durante el acceso a memoria.");
    }

    t_buffer *payload = recibir_payload(conexion_memoria);
    rta_memoria = buffer_take_UINT32(payload);

    log_info(logger, "Se recibió respuesta de memoria %d", rta_memoria);

    return rta_memoria;
}


// funcion auxiliar genérica para accesos a memoria
uint32_t acceso_a_memoria_2(codigo_operacion header, uint32_t valor1, uint32_t valor2, uint32_t valor3)
{
    uint32_t rta_memoria;

    t_paquete *paquete = crear_paquete(header, sizeof(uint32_t) * 2);
    agregar_a_buffer_UINT32(paquete->payload, (uint32_t)valor1);
    agregar_a_buffer_UINT32(paquete->payload, (uint32_t)valor2);
    agregar_a_buffer_UINT32(paquete->payload, (uint32_t)valor3);
    enviar_paquete(conexion_memoria, paquete);
    destruir_paquete(paquete);

    uint8_t resp_memoria = recibir_header(conexion_memoria);

    if (resp_memoria != MEMORIA_OK)
    {
        log_info(logger, "Ha ocurrido un error durante el acceso a memoria.");
    }

    t_buffer *payload = recibir_payload(conexion_memoria);
    rta_memoria = buffer_take_UINT32(payload);

    log_info(logger, "Se recibió respuesta de memoria %d", rta_memoria);

    return rta_memoria;
}


// funcion auxiliar genérica para accesos a memoria
uint32_t acceso_a_memoria_3(codigo_operacion header, uint32_t valor1)
{
    uint32_t rta_memoria;

    t_paquete *paquete = crear_paquete(header, sizeof(uint32_t) * 2);
    agregar_a_buffer_UINT32(paquete->payload, (uint32_t)valor1);
    enviar_paquete(conexion_memoria, paquete);
    destruir_paquete(paquete);

    uint8_t resp_memoria = recibir_header(conexion_memoria);

    if (resp_memoria != MEMORIA_OK)
    {
        log_info(logger, "Ha ocurrido un error durante el acceso a memoria. - ACCESO 3");
    }

    t_buffer *payload = recibir_payload(conexion_memoria);
    rta_memoria = buffer_take_UINT32(payload);

    log_info(logger, "Se recibió respuesta de memoria %d", rta_memoria);

    return rta_memoria;
}