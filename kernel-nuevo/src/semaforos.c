#include "semaforos.h"

void ini_semaforos(void) {

	mutex_pid = malloc(sizeof(sem_t));
	sem_init(mutex_pid, 0, 1);

	mutex_mediano_plazo = malloc(sizeof(sem_t));
	sem_init(mutex_mediano_plazo, 0, 1);

	sem_multiprogramacion = malloc(sizeof(sem_t));
	sem_init(sem_multiprogramacion, 0, kernel_config -> grado_multiprogramacion);

	sem_mediano_plazo = malloc(sizeof(sem_t));
	sem_init(sem_mediano_plazo, 0, 0);

	sem_corto_plazo = malloc(sizeof(sem_t));
	sem_init(sem_corto_plazo, 0, 0);

	sem_mediano_plazo = malloc(sizeof(sem_t));
	sem_init(sem_mediano_plazo, 0, 0);

	sem_largo_plazo = malloc(sizeof(sem_t));
	sem_init(sem_largo_plazo, 0, 0);
}