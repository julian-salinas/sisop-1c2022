#include "semaforos.h"

void ini_semaforos(void) {

	mutex_pid = malloc(sizeof(sem_t));
	sem_init(mutex_pid, 0, 1);

	/* =============== Mutex para que no joda el mediano plazo =============== */ 
	mutex_mediano_plazo = malloc(sizeof(sem_t));
	sem_init(mutex_mediano_plazo, 0, 1);

	/* =============== Semáforo contador para multiprogramacion =============== */
	sem_multiprogramacion = malloc(sizeof(sem_t));
	sem_init(sem_multiprogramacion, 0, kernel_config -> grado_multiprogramacion);

	/* ======== Semáforos usados para llamar a c/planificador ======== */
	sem_corto_plazo = malloc(sizeof(sem_t));
	sem_init(sem_corto_plazo, 0, 0);

	sem_mediano_plazo = malloc(sizeof(sem_t));
	sem_init(sem_mediano_plazo, 0, 0);

	sem_largo_plazo = malloc(sizeof(sem_t));
	sem_init(sem_largo_plazo, 0, 0);

	sem_io = malloc(sizeof(sem_t));
	sem_init(sem_io, 0, 0);

	/* =========== Mutex variables que contienen procesos ============ */
	mutex_proceso_corriendo = malloc(sizeof(sem_t));
	sem_init(mutex_proceso_corriendo, 0, 1);

	mutex_proceso_buscado = malloc(sizeof(sem_t));
	sem_init(mutex_proceso_buscado, 0, 1);

	/* ========= Semáforos para indicar eventos/condiciones ========== */
	sem_nuevo_proceso = malloc(sizeof(sem_t));
	sem_init(sem_nuevo_proceso, 0, 0);

	sem_procesos_esperando = malloc(sizeof(sem_t));
	sem_init(sem_procesos_esperando, 0, 0);

	sem_procesos_en_ready = malloc(sizeof(sem_t));
	sem_init(sem_procesos_en_ready, 0, 0);

	sem_cpu_disponible = malloc(sizeof(sem_t));
	sem_init(sem_cpu_disponible, 0, 1);

	sem_procesos_bloqueados = malloc(sizeof(sem_t));
	sem_init(sem_procesos_bloqueados, 0, 0);
}