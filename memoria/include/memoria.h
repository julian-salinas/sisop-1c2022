/*
 * memoria.h
 *
 *  Created on: 19 abr. 2022
 *      Author: utnso
 */

#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils-memoria.h"

//includes de shared
#include "networking.h"
#include "serializacion.h"
#include "logging.h"
#include "config.h"

t_log* logger;
int conexion_cpu;
void enviar_config_a_cpu();

#endif /* MEMORIA_H_ */
