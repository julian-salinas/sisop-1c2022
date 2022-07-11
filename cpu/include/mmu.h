#ifndef MMU
#define MMU
  
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>

// includes de las commons
#include<commons/string.h>
#include<commons/config.h>

// includes del módulo
#include "procesar_conexion.h"
#include "cpu_config.h"
#include "ciclo_instruccion.h"

// includes de shared
#include "networking.h"
#include "serializacion.h"
#include "config.h"
#include "proceso.h"
#include "utils.h"

#include "tlb.h"
#include "cpu.h"
#include "ciclo_instruccion.h"

uint32_t acceso_a_memoria(codigo_operacion header, uint32_t valor1, uint32_t valor2);

#endif /* MMU */