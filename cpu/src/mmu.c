#include "mmu.h"

//MMU FALTA USO DE TLB
void mmu(int direccion_logica, uint32_t marco, uint32_t desplazamiento){
uint32_t nro_tabla_segundo_nivel;
uint32_t entrada_tabla_2do_nivel;

uint32_t numero_pagina= (uint32_t) floor(direccion_logica/tamanio_pagina);
uint32_t entrada_tabla_1er_nivel = (uint32_t) floor(numero_pagina/paginas_por_tabla);

//CHEQUEO EN TLB 
if((nro_tabla_segundo_nivel=buscar_entrada_tlb(tlb, numero_pagina))==0){

    //SI NO ENCUENTRA
    //primer acceso a memoria
    nro_tabla_segundo_nivel = acceso_a_memoria(PRIMER_ACCESO_MEMORIA, numero_pagina, entrada_tabla_1er_nivel);
    agregar_entrada_tlb(tlb, numero_pagina, nro_tabla_segundo_nivel);
}else{
    //nos ahorramos una entrada
    //segundo acceso a memoria
    entrada_tabla_2do_nivel = (uint32_t) numero_pagina % paginas_por_tabla;

    marco = acceso_a_memoria(SEGUNDO_ACCESO_MEMORIA, nro_tabla_segundo_nivel, entrada_tabla_2do_nivel);
    desplazamiento = (direccion_logica - (numero_pagina * tamanio_pagina));
    }
}

// funcion auxiliar genérica para accesos a memoria
uint32_t acceso_a_memoria(codigo_operacion header, uint32_t valor1, uint32_t valor2){
uint32_t rta_memoria;

t_paquete* paquete = crear_paquete(header, sizeof(uint32_t)*2);
agregar_a_buffer_UINT32(paquete -> payload, (uint32_t) valor1);
agregar_a_buffer_UINT32(paquete -> payload, (uint32_t) valor2);
enviar_paquete(conexion_memoria, paquete);
destruir_paquete(paquete);

uint8_t resp_memoria = recibir_header(conexion_memoria);

if (resp_memoria != header) {
    log_info(logger, "Ha ocurrido un error durante el acceso a memoria.");
}

t_buffer* payload = recibir_payload(conexion_memoria);
rta_memoria = buffer_take_UINT32(payload);

return rta_memoria;
}