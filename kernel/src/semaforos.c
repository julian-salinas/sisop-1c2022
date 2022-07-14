#include "semaforos.h"

void ini_semaforos(void) {

	/* =============== Mutex para proteger variables =============== */ 
	
	mutex_pid = malloc(sizeof(sem_t));
	sem_init(mutex_pid, 0, 1);

	/* =============== Mutex para que no se molesten entre threads =============== */ 

	mutex_mediano_plazo = malloc(sizeof(sem_t));
	sem_init(mutex_mediano_plazo, 0, 1);

	mutex_suspension = malloc(sizeof(sem_t));
	sem_init(mutex_suspension, 0, 1);

	/* =============== Semáforo contador para multiprogramacion =============== */

	sem_multiprogramacion = malloc(sizeof(sem_t));
	sem_init(sem_multiprogramacion, 0, kernel_config -> grado_multiprogramacion);

	/* ======== Semáforos usados para llamar a c/planificador ======== */

	sem_largo_plazo = malloc(sizeof(sem_t));
	sem_init(sem_largo_plazo, 0, 0);

	/* ========= Semáforos para indicar eventos/condiciones - CONTADORES ========== */
	
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

	/* ============= Semáforos mutex sockets comunicación con otros módulos ============= */
	mutex_socket_memoria = malloc(sizeof(sem_t));
	sem_init(mutex_socket_memoria, 0, 1);

	mutex_socket_cpu_dispatch = malloc(sizeof(sem_t));
	sem_init(mutex_socket_cpu_dispatch, 0, 1);

	mutex_socket_cpu_interrupt = malloc(sizeof(sem_t));
	sem_init(mutex_socket_cpu_interrupt, 0, 1);

    sem_proceso_interrumpido = malloc(sizeof(sem_t));
	sem_init(sem_proceso_interrumpido, 0, 0);
}