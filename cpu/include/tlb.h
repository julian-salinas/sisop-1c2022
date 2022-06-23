#ifndef TLB
#define TLB

#include "networking.h"
#include "serializacion.h"
#include "time.h" 

    /**
     * @DESC: Entrada de la TLB
     *  - pagina: contiene el n° de página
     *  - marco: contiene el n° de marco o frame
     *  - instante_carga: se almacena el time en segundos cuando fue creado para FIFO
     *  - instante_ultima_referencia: se almacena el time en segundos cuando fue usado para LRU
     */ 
    typedef struct {
        int pagina;
        int marco;
        time_t instante_carga;
        time_t instante_ultima_referencia; 
    } t_entrada_tlb;

    typedef t_list t_tlb;
    typedef t_list t_lista_entradas;

    void inicializar_tlb(t_tlb* tlb);
    
    bool primeraEntrada(t_entrada_tlb* entrada1, t_entrada_tlb* entrada2);

#endif /* TLB */