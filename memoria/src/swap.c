#include "swap.h"

t_page_data* crear_page_data(int32_t nro_pagina, uint32_t dato) {
    t_page_data* tmp = malloc(sizeof(t_page_data));
    tmp -> nro_pagina = nro_pagina;
    tmp -> dato = dato;
    return tmp;
}


void destruir_page_data(t_page_data* page_data) {
    free(page_data);
}


t_swap* crear_archivo_swap(uint32_t PID, size_t tamanio) {
    t_swap* archivito_swap = malloc(sizeof(t_swap));
    archivito_swap -> path = string_from_format("%s/%d.swap", memoria_config->path_swap, PID);
    archivito_swap -> tamanio = tamanio;
    archivito_swap -> fd = open(archivito_swap -> path, O_RDWR|O_CREAT, S_IRWXU);

    ftruncate(archivito_swap -> fd, tamanio);

    void* archivito_mapeado = mmap(NULL, archivito_swap->tamanio, PROT_READ|PROT_WRITE,MAP_SHARED, archivito_swap->fd, 0);
    memset(archivito_mapeado, 0, archivito_swap -> tamanio);
    msync(archivito_mapeado, archivito_swap -> tamanio, MS_SYNC);
    munmap(archivito_mapeado, archivito_swap -> tamanio);

    int cantidad_entradas_proceso = get_cantidad_entradas_proceso(PID);
    // archivito_swap -> entradas = malloc(sizeof(t_page_data) * cantidad_entradas_proceso);
    archivito_swap -> entradas = calloc(cantidad_entradas_proceso, sizeof(t_page_data));
    
    for (uint32_t i=0;i<cantidad_entradas_proceso;i++) {
        archivito_swap -> entradas[i].dato = 0;
    }

    log_info(logger, "Se creó el archivo %s para el proceso ID:%d", archivito_swap -> path, PID);
    char* nro_pid = int_a_string(PID);
    dictionary_put(diccionario_swap, nro_pid, archivito_swap);
    free(nro_pid);

    return archivito_swap;
}


void destruir_archivo_swap(uint32_t PID) {
    char* nro_pid = int_a_string(PID);
    t_swap* archivito_swap = (t_swap*) dictionary_get(diccionario_swap, nro_pid);
    free(nro_pid);
    close(archivito_swap -> fd);
    unlink(archivito_swap -> path);
    free(archivito_swap -> path);
    free(archivito_swap -> entradas);
    free(archivito_swap);
}


t_page_data* leer_pagina(t_swap* archivito_swap, int nro_pagina) {
    sem_wait(mutex_swap);
    void* archivito_mapeado = mmap(NULL, archivito_swap->tamanio, PROT_READ|PROT_WRITE, MAP_SHARED, archivito_swap->fd, 0);
    void* datos = malloc(sizeof(memoria_config -> tamanio_pagina));
    // memcpy(datos, archivito_mapeado + nro_pagina * memoria_config -> tamanio_pagina, memoria_config -> tamanio_pagina);
    memcpy(datos, archivito_mapeado + nro_pagina * memoria_config -> tamanio_pagina, sizeof(t_page_data));
    munmap(archivito_mapeado, archivito_swap -> tamanio);
    sem_post(mutex_swap);
    return datos;
}


void escribir_pagina(t_swap* archivito_swap, int nro_pagina, t_page_data* page_data) {
    sem_wait(mutex_swap);
    void* archivito_mapeado = mmap(NULL, archivito_swap->tamanio, PROT_READ|PROT_WRITE,MAP_SHARED, archivito_swap -> fd, 0);
    // memcpy(archivito_mapeado + nro_pagina * memoria_config -> tamanio_pagina, page_data, memoria_config -> tamanio_pagina);
    memcpy(archivito_mapeado + nro_pagina * memoria_config -> tamanio_pagina, page_data, sizeof(t_page_data));
    msync(archivito_mapeado, archivito_swap->tamanio, MS_SYNC);
    munmap(archivito_mapeado, archivito_swap->tamanio);
    sem_post(mutex_swap);
}


void swappear(uint32_t PID, t_entrada_segundo_nivel* entrada) {
    

    // Si el bit de modificado está en 1, leer el marco y almacenar el dato
    uint32_t dato;

    log_warning(logger, "Swappeando la entrada %d", entrada -> nro_pagina);

    if (entrada -> bit_modificado) {
        // TODO: Leer frame
        entrada -> bit_modificado = 0;
        dato = leer_contenido_frame_n(entrada -> nro_frame);
    }
    else {
        dato = 0;
    }

    entrada -> bit_presencia = 0;
    entrada -> bit_uso = 0;
    entrada -> bit_modificado = 0;

    // Liberar el frame
    liberar_frame_n(entrada -> nro_frame);

    // Guardar datos en swap
    t_page_data* page_data = crear_page_data(entrada -> nro_pagina, dato);
    char* nro_pid = int_a_string(PID);
    t_swap* swap_proceso = (t_swap*) dictionary_get(diccionario_swap, nro_pid);
    free(nro_pid);
    escribir_pagina(swap_proceso, entrada -> nro_pagina, page_data);
    destruir_page_data(page_data);

    log_warning(logger, "Se swappeo la entrada %d", entrada -> nro_pagina);

    sem_wait(mutex_cantidad_accesos_swap);
        cantidad_accesos_swap++;
        log_info(logger, "Cantidad de accesos a SWAP: %d", cantidad_accesos_swap);
    sem_post(mutex_cantidad_accesos_swap);
}


void desswappear(uint32_t PID, t_entrada_segundo_nivel* entrada) {

    log_warning(logger, "Deswappeando la entrada %d", entrada -> nro_pagina);

    // Obtener los datos de swap del diccionario
    char* str_id = int_a_string(PID);
    t_swap* swap_proceso = (t_swap*) dictionary_get(diccionario_swap, str_id);
    free(str_id);
    
    // Leer el contenido de la página en swap
    t_page_data* page_data = leer_pagina(swap_proceso, entrada -> nro_pagina);

    t_list* entradas_en_memoria = get_entradas_en_memoria_proceso(PID);

    // Si el proceso tiene límite de marcos asignados, correr algoritmo de reemplazo
    if (list_size(entradas_en_memoria) >= memoria_config -> marcos_por_proceso) {
        t_entrada_segundo_nivel* entrada_a_reemplazar;

        if (algoritmo_reemplazo == CLOCK) {
            entrada_a_reemplazar = algoritmo_clock(PID);
        }
        else {
            entrada_a_reemplazar = algoritmo_clock_mejorado(PID);
        }

        // Mandar a swap a la entrada que acabamos de reemplazar
        swappear(PID, entrada_a_reemplazar);

        // Asignar el frame que se acaba de liberar a la entrada a reemplazar
        // NOTA: el bit de ocupado del frame que se manipula nunca se altera, siempre permanece ocupado
        entrada -> nro_frame = entrada_a_reemplazar -> nro_frame;
    }
    else {
        // NOTA: el bit de ocupado del frame que se manipula nunca se altera, siempre permanece ocupado
        entrada -> nro_frame = get_posicion_frame_libre();
    }
    list_destroy(entradas_en_memoria);

    entrada -> bit_presencia = 1;
    entrada -> bit_uso = 1;
    entrada -> bit_modificado = 0; // Esto ya debería estar en 0, peroweno porlasdudas lo pongo

    ocupar_frame_n(entrada -> nro_frame);

    // Por último, escribir en el frame lo que ya estaba en la página (lo que leímos de swap)
    escribir_frame_n(entrada -> nro_frame, page_data -> dato);
    destruir_page_data(page_data);

    log_warning(logger, "Se deswappeo la entrada %d en el marco %d", entrada -> nro_pagina, entrada -> nro_frame);
    sem_wait(mutex_cantidad_accesos_swap);
        cantidad_accesos_swap++;
        log_info(logger, "Cantidad de accesos a SWAP: %d", cantidad_accesos_swap);
    sem_post(mutex_cantidad_accesos_swap);
}
