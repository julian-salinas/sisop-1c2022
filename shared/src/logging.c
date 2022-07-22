#include "logging.h"


t_log* iniciar_logger(char* file, char* process_name) {
	t_log* nuevo_logger = log_create(file, process_name, true, LOG_LEVEL_TRACE);

	return nuevo_logger;
}