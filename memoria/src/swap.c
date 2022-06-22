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