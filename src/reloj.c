/* Copyright 2022, Laboratorio de Microprocesadores
 * Facultad de Ciencias Exactas y Tecnología
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Implementations of time and alarm clock management
 **
 ** \addtogroup clock Clock
 ** \brief Time and alarm clock management
 ** @{ */

/* === Headers files inclusions =============================================================== */

#include "reloj.h"
#include <string.h>

/* === Macros definitions ====================================================================== */

//! Cantidad de elementos en el arreglo BCD con la hora actual
#define TIME_SIZE 6

//! Valor inicial de las variables del reloj
#define INITIAL_VALUE 0

//! Valor máximo de las unidades en un numero BCD
#define MAX_UNITS_VALUE 9

//! Valor máximo de las decenas en los segundos y minutos
#define MAX_TENS_VALUE 5

//! Indice de la unidad de segundos en el vector de hora
#define SECONDS_UNITS 5

//! Indice de la decena de segundos en el vector de hora
#define SECONDS_TENS 4

//! Indice de la unidad de segundos en el vector de hora
#define MINUTES_UNITS 3

/* === Private data type declarations ========================================================== */

struct clock_s {
    uint8_t time[TIME_SIZE];
    uint16_t ticks_count;
    uint16_t ticks_per_second;
    bool valid;
};

/* === Private variable declarations =========================================================== */

/* === Private function declarations =========================================================== */

/**
 * @brief Función para incrementar un digito BCD almacenado en un vector
 *
 * @param number Puntero al primer digito del numero BCD
 * @param index Indice del digito que se incrementa
 */
static void IncrementDigit(uint8_t number[], uint8_t index);

/* === Public variable definitions ============================================================= */

/* === Private variable definitions ============================================================ */

static struct clock_s instances;

/* === Private function implementation ========================================================= */

static void IncrementDigit(uint8_t number[], uint8_t index) {
    number[index]++;
    if (number[index] > MAX_UNITS_VALUE) {
        number[index--] = INITIAL_VALUE;
        number[index]++;
    }
}

/* === Public function implementation ========================================================= */

clock_t ClockCreate(uint16_t ticks_per_second) {
    clock_t clock = &instances;

    memset(clock, INITIAL_VALUE, sizeof(struct clock_s));
    clock->ticks_per_second = ticks_per_second;

    return clock;
}

bool ClockGetTime(clock_t reloj, uint8_t * result, uint8_t size) {
    memcpy(result, reloj->time, size);
    return reloj->valid;
}

void ClockSetupTime(clock_t clock, uint8_t const * const time, uint8_t size) {
    clock->valid = true;
    memcpy(clock->time, time, size);
}

void ClockNewTick(clock_t clock) {
    clock->ticks_count++;
    if (clock->ticks_count == clock->ticks_per_second) {
        clock->ticks_count = INITIAL_VALUE;
        IncrementDigit(clock->time, SECONDS_UNITS);
    }
    if (clock->time[SECONDS_TENS] > MAX_TENS_VALUE) {
        clock->time[SECONDS_TENS] = INITIAL_VALUE;
        IncrementDigit(clock->time, MINUTES_UNITS);
    }
}

/* === End of documentation ==================================================================== */

/** @} End of module definition for doxygen */
