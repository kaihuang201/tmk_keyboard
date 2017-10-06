/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"

#ifndef DEBOUNCE
#   define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void clear_rows(void);
static void select_row(uint8_t row);

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

#define LED_ON()    do { palSetPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13) ;} while (0)
#define LED_OFF()   do { palClearPad(TEENSY_PIN13_IOPORT, TEENSY_PIN13); } while (0)
#define LED_TGL()   do { palTogglePad(TEENSY_PIN13_IOPORT, TEENSY_PIN13); } while (0)

// palSetPadMode(ioportid_t, uint8_t, iomode_t);
// palClearPad(ioportid_t, uint8_t);
// palReadPad(ioportid_t, uint8_t);

#define PORT_COL_0  TEENSY_PIN0_IOPORT
#define PORT_COL_1  TEENSY_PIN1_IOPORT
#define PORT_COL_2  TEENSY_PIN2_IOPORT
#define PORT_COL_3  TEENSY_PIN3_IOPORT
#define PORT_COL_4  TEENSY_PIN4_IOPORT
#define PORT_COL_5  TEENSY_PIN5_IOPORT
#define PORT_COL_6  TEENSY_PIN6_IOPORT
#define PORT_COL_7  TEENSY_PIN7_IOPORT
#define PORT_COL_8  TEENSY_PIN8_IOPORT
#define PORT_COL_9  TEENSY_PIN9_IOPORT
#define PORT_COL_10 TEENSY_PIN10_IOPORT
#define PORT_COL_11 TEENSY_PIN11_IOPORT
#define PORT_COL_12 TEENSY_PIN12_IOPORT
#define PORT_COL_13 TEENSY_PIN14_IOPORT
#define PORT_COL_14 TEENSY_PIN15_IOPORT

#define PIN_COL_0  TEENSY_PIN0
#define PIN_COL_1  TEENSY_PIN1
#define PIN_COL_2  TEENSY_PIN2
#define PIN_COL_3  TEENSY_PIN3
#define PIN_COL_4  TEENSY_PIN4
#define PIN_COL_5  TEENSY_PIN5
#define PIN_COL_6  TEENSY_PIN6
#define PIN_COL_7  TEENSY_PIN7
#define PIN_COL_8  TEENSY_PIN8
#define PIN_COL_9  TEENSY_PIN9
#define PIN_COL_10 TEENSY_PIN10
#define PIN_COL_11 TEENSY_PIN11
#define PIN_COL_12 TEENSY_PIN12
#define PIN_COL_13 TEENSY_PIN14
#define PIN_COL_14 TEENSY_PIN15


#define PORT_ROW_0 TEENSY_PIN21_IOPORT
#define PORT_ROW_1 TEENSY_PIN22_IOPORT
#define PORT_ROW_2 TEENSY_PIN23_IOPORT
#define PORT_ROW_3 TEENSY_PIN24_IOPORT
#define PORT_ROW_4 TEENSY_PIN25_IOPORT

#define PIN_ROW_0 TEENSY_PIN21
#define PIN_ROW_1 TEENSY_PIN22
#define PIN_ROW_2 TEENSY_PIN23
#define PIN_ROW_3 TEENSY_PIN24
#define PIN_ROW_4 TEENSY_PIN25

void matrix_init(void)
{
    // initialize row and col
    clear_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    //debug
    debug_matrix = true;
    LED_ON();
    wait_ms(500);
    LED_OFF();
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        clear_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            wait_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

/* Column pin configuration */
static void  init_cols(void)
{
    // internal pull-up
    palSetPadMode(PORT_COL_0,  PIN_COL_0,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_1,  PIN_COL_1,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_2,  PIN_COL_2,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_3,  PIN_COL_3,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_4,  PIN_COL_4,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_5,  PIN_COL_5,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_6,  PIN_COL_6,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_7,  PIN_COL_7,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_8,  PIN_COL_8,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_9,  PIN_COL_9,  PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_10, PIN_COL_10, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_11, PIN_COL_11, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_12, PIN_COL_12, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_13, PIN_COL_13, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(PORT_COL_14, PIN_COL_14, PAL_MODE_INPUT_PULLUP);
}

/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(void)
{
    return 
        ( (palReadPad(PORT_COL_0,  PIN_COL_0 )==PAL_HIGH) ? 0 : (1<<0 )) |
        ( (palReadPad(PORT_COL_1,  PIN_COL_1 )==PAL_HIGH) ? 0 : (1<<1 )) |
        ( (palReadPad(PORT_COL_2,  PIN_COL_2 )==PAL_HIGH) ? 0 : (1<<2 )) |
        ( (palReadPad(PORT_COL_3,  PIN_COL_3 )==PAL_HIGH) ? 0 : (1<<3 )) |
        ( (palReadPad(PORT_COL_4,  PIN_COL_4 )==PAL_HIGH) ? 0 : (1<<4 )) |
        ( (palReadPad(PORT_COL_5,  PIN_COL_5 )==PAL_HIGH) ? 0 : (1<<5 )) |
        ( (palReadPad(PORT_COL_6,  PIN_COL_6 )==PAL_HIGH) ? 0 : (1<<6 )) |
        ( (palReadPad(PORT_COL_7,  PIN_COL_7 )==PAL_HIGH) ? 0 : (1<<7 )) |
        ( (palReadPad(PORT_COL_8,  PIN_COL_8 )==PAL_HIGH) ? 0 : (1<<8 )) |
        ( (palReadPad(PORT_COL_9,  PIN_COL_9 )==PAL_HIGH) ? 0 : (1<<9 )) |
        ( (palReadPad(PORT_COL_10, PIN_COL_10)==PAL_HIGH) ? 0 : (1<<10)) |
        ( (palReadPad(PORT_COL_11, PIN_COL_11)==PAL_HIGH) ? 0 : (1<<11)) |
        ( (palReadPad(PORT_COL_12, PIN_COL_12)==PAL_HIGH) ? 0 : (1<<12)) |
        ( (palReadPad(PORT_COL_13, PIN_COL_13)==PAL_HIGH) ? 0 : (1<<13)) |
        ( (palReadPad(PORT_COL_14, PIN_COL_14)==PAL_HIGH) ? 0 : (1<<14));
}

/* Row pin configuration */
static void clear_rows(void)
{
    // hi-Z
    palSetPadMode(PORT_ROW_0, PIN_ROW_0, PAL_MODE_INPUT);
    palSetPadMode(PORT_ROW_1, PIN_ROW_1, PAL_MODE_INPUT);
    palSetPadMode(PORT_ROW_2, PIN_ROW_2, PAL_MODE_INPUT);
    palSetPadMode(PORT_ROW_3, PIN_ROW_3, PAL_MODE_INPUT);
    palSetPadMode(PORT_ROW_4, PIN_ROW_4, PAL_MODE_INPUT);
}

static void select_row(uint8_t i)
{
    // Output low to select
    switch(i) {
    case 0:
        palSetPadMode(PORT_ROW_0, PIN_ROW_0, PAL_MODE_OUTPUT_PUSHPULL);
        palClearPad(PORT_ROW_0, PIN_ROW_0);
        break;
    case 1:
        palSetPadMode(PORT_ROW_1, PIN_ROW_1, PAL_MODE_OUTPUT_PUSHPULL);
        palClearPad(PORT_ROW_1, PIN_ROW_1);
        break;
    case 2:
        palSetPadMode(PORT_ROW_2, PIN_ROW_2, PAL_MODE_OUTPUT_PUSHPULL);
        palClearPad(PORT_ROW_2, PIN_ROW_2);
        break;
    case 3:
        palSetPadMode(PORT_ROW_3, PIN_ROW_3, PAL_MODE_OUTPUT_PUSHPULL);
        palClearPad(PORT_ROW_3, PIN_ROW_3);
        break;
    case 4:
        palSetPadMode(PORT_ROW_4, PIN_ROW_4, PAL_MODE_OUTPUT_PUSHPULL);
        palClearPad(PORT_ROW_4, PIN_ROW_4);
        break;
    }
}
