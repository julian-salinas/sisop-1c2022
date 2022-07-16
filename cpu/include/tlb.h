#ifndef TLB
#define TLB

#include "networking.h"
#include "serializacion.h"
#include <time.h>    
    /**
     * @DESC: Entrada de la TLB
     *  - pagina: contiene el n° de página
     *  - marco: contiene el n° de marco o frame
     *  - instante_carga: se almacena el time en segundos cuando fue creado para FIFO
     *  - instante_ultima_referencia: se almacena el time en segundos cuando fue usado para LRU
     */ 

    typedef enum {
        FIFO,
        LRU
    } algoritmo_planificacion;

    algoritmo_planificacion algoritmo_elegido;

    typedef struct {
        int32_t pagina;
        uint32_t marco;
        time_t instante_carga;
        time_t instante_ultima_referencia; 
    } t_entrada_tlb;

    typedef t_list t_lista_entradas;
    typedef t_list t_tlb;

    t_lista_entradas* entradas_tlb;

    void inicializar_tlb(void);
    void generar_entradas_tlb(void);
    void liberar_tlb(void);

    bool algoritmo_LRU(t_entrada_tlb* entrada1, t_entrada_tlb* entrada2);
    bool algoritmo_FIFO(t_entrada_tlb* entrada1, t_entrada_tlb* entrada2);

    void ordenar_tlb(t_lista_entradas* lista_entradas);

    void elegir_algoritmo(char* algoritmo);

    int32_t buscar_entrada_tlb(t_lista_entradas* lista_entradas, uint32_t numero_pagina);

    void agregar_entrada_tlb(t_lista_entradas* lista_entradas, uint32_t numero_pagina, uint32_t numero_marco);

    void limpiar_tlb2(void);

#endif /* TLB */