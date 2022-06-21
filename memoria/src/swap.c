#include "swap.h"


void crear_archivo_proceso(uint32_t PID) {
    char* nombre_archivo = string_from_format("%d.swap",PID);
    
    FILE * archivo = fopen(nombre_archivo,"w+");
    
    if(archivo == NULL) {
        log_info(logger,"No se pudo crear el archivo .swap del proceso: %d",PID);
    } else {
        log_info(logger,"Se creo el archivo .swap del proceso: %d",PID);
    }

    fclose(archivo);
}


void eliminar_archivo_proceso(uint32_t PID) {
    char* nombre_archivo = string_from_format("%d.swap",PID);
    
    if(remove(nombre_archivo) == 0) { // Eliminamos el archivo
        log_info(logger,"El archivo '%s' fue eliminado", nombre_archivo);
    } else {
        log_info(logger,"No se pudo eliminar el archivo '%s'", nombre_archivo);
    }
}