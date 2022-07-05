#include "swap.h"

void crear_archivo_proceso(uint32_t PID) {
    char* path_carpeta_swap = memoria_config -> path_swap;
    char* nombre_archivo = string_from_format("%s/%d.swap",path_carpeta_swap,PID);
    
    if(access(path_carpeta_swap,F_OK) == -1 ) {
        mkdir(path_carpeta_swap,0700);
    }

    FILE * archivo = fopen(nombre_archivo,"w+");
    
    if(archivo == NULL) {
        log_info(logger,"No se pudo crear el archivo .swap del proceso: %d",PID);
    } else {
        log_info(logger,"Se creo el archivo .swap del proceso: %d",PID);
        fclose(archivo);
    }
}


void eliminar_archivo_proceso(uint32_t PID) {
    char* path_carpeta_swap = memoria_config -> path_swap;
    char* nombre_archivo = string_from_format("%s/%d.swap",path_carpeta_swap,PID);
    
    FILE * archivo = fopen(nombre_archivo,"r");
    
    if(archivo > 0) {
        if(remove(nombre_archivo) == 0) { // Eliminamos el archivo
            log_info(logger,"El archivo '%d.swap' fue eliminado", PID);
        } else {
            log_info(logger,"No se pudo eliminar el archivo '%d.swap'", PID);
        }
    } else {
        log_info(logger,"El archivo '%d.swap' no existe", PID);
    }
}


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

    log_info(logger, "Se inicializó archivito swap path:%s, tamanio:%d, fd:%d", 
            archivito_swap -> path, archivito_swap -> tamanio, archivito_swap -> fd);
    
    ftruncate(archivito_swap -> fd, tamanio);

    void* archivito_mapeado = mmap(NULL, archivito_swap->tamanio, PROT_READ|PROT_WRITE,MAP_SHARED, archivito_swap->fd, 0);
    memset(archivito_mapeado, 0, archivito_swap -> tamanio);
    msync(archivito_mapeado, archivito_swap -> tamanio, MS_SYNC);
    munmap(archivito_mapeado, archivito_swap -> tamanio);

    int cantidad_entradas_proceso = get_cantidad_entradas_proceso(PID);
    archivito_swap -> entradas = malloc(sizeof(t_page_data) * cantidad_entradas_proceso);
    
    for (uint32_t i=0;i<cantidad_entradas_proceso;i++) {
        archivito_swap -> entradas[i].dato = 0;
    }

    log_info(logger, "Se creó el archivo %s para el proceso ID:%d", archivito_swap -> path, PID);
    dictionary_put(diccionario_swap, int_a_string(PID), archivito_swap);

    return archivito_swap;
}


void destruir_archivo_swap(t_swap* archivito_swap) {
    close(archivito_swap -> fd);
    free(archivito_swap -> path);
    free(archivito_swap -> entradas);
    free(archivito_swap);
}


t_page_data* leer_pagina(t_swap* archivito_swap, int nro_pagina) {
    log_info(logger, "flaco q te pasa");
    void* archivito_mapeado = mmap(NULL, archivito_swap->tamanio, PROT_READ|PROT_WRITE, MAP_SHARED, archivito_swap->fd, 0);
    void* datos = malloc(sizeof(memoria_config -> tamanio_pagina));
    memcpy(datos, archivito_mapeado + (nro_pagina * (memoria_config -> tamanio_pagina)), memoria_config -> tamanio_pagina);
    munmap(archivito_mapeado, archivito_swap -> tamanio);
    return datos;
}


void escribir_pagina(t_swap* archivito_swap, int nro_pagina, t_page_data* page_data) {
    void* archivito_mapeado = mmap(NULL, archivito_swap->tamanio, PROT_READ|PROT_WRITE,MAP_SHARED, archivito_swap->fd, 0);
    memcpy(archivito_mapeado + nro_pagina * memoria_config -> tamanio_pagina, page_data, memoria_config -> tamanio_pagina);
    msync(archivito_mapeado, archivito_swap->tamanio, MS_SYNC);
    munmap(archivito_mapeado, archivito_swap->tamanio);
}


void swappear(uint32_t PID, t_entrada_segundo_nivel* entrada) {
    // Si el bit de modificado está en 1, leer el marco y almacenar el dato
    uint32_t dato;

    if (entrada -> bit_modificado) {
        // TODO: Leer frame
        entrada -> bit_modificado = 0;
        dato = leer_contenido_frame(entrada -> nro_frame);
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
    t_swap* swap_proceso = (t_swap*) dictionary_get(diccionario_swap, int_a_string(PID));
    escribir_pagina(swap_proceso, entrada -> nro_pagina, page_data);
}


void desswappear(uint32_t PID, t_entrada_segundo_nivel* entrada) {
    t_page_data* page_data = malloc(sizeof(t_page_data));

    // Obtener los datos de swap del diccionario
    t_swap* swap_proceso = (t_swap*) dictionary_get(diccionario_swap, int_a_string(PID));

    // Leer el contenido de la página en swap
    page_data = leer_pagina(swap_proceso, entrada -> nro_pagina);

    // Si el proceso tiene límite de marcos asignados, correr algoritmo de reemplazo
    if (list_size(get_entradas_en_memoria_proceso(PID)) >= memoria_config -> marcos_por_proceso) {
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

    entrada -> bit_presencia = 1;
    entrada -> bit_uso = 1;
    entrada -> bit_modificado = 0; // Esto ya debería estar en 0, peroweno porlasdudas lo pongo

    // Por último, escribir en el frame lo que ya estaba en la página (lo que leímos de swap)
    escribir_frame_n(entrada -> nro_frame, page_data -> dato);
}
