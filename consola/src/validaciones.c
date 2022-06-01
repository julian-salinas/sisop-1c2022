#include "validaciones.h"

void free_consola(int socket_servidor, t_consola_config* consola_config) {
    log_info(logger_consola, "Modulo consola finalizado");
    liberar_socket_cliente(socket_servidor);
    log_destroy(logger_consola);
    destruir_consola_config(consola_config);
}

int validar_argumentos(int argc) {
    if (argc < 3) {
		log_error(logger_consola, "No se recibieron los parámetros correctos. Formato: {./consola} {tamañoProceso} {pathArchivo}"); 
        return 0;
    }
    
    return 1;
}

int validar_apertura_archivo(FILE* file, char* path) {
    if (file == NULL) {
        log_error(logger_consola, "No se pudo abrir el archivo de pseudocódigo. Verificar el path del mismo: %s", path);
        return 0;
    }

    return 1;
}