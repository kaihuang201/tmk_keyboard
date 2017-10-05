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

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

// Initialize the LED output pin
// Credit @pastosai https://github.com/patosai/gh60/blob/master/src/led.c
inline led_init(void)
{
    DDRB |= (1<<PB6);

    TCCR1A |= ((1<<WGM10)        // 8 bit fast PWM
              | (1<<COM1A1)    // Clear OC1A on compare match, clear them at bottom
              | (1<<COM1B1));  // Clear OC1B on compare match, clear them at bottom
    TCCR1B |= ((1<<WGM12)        // 8 bit fast PWM
              | (1<<CS11));    // 1/8 prescale

    OCR1B = 0;
}
/* to turn off
DDRB &= ~(1<<PB6);
PORTB |= (1<<PB6);
*/

inline
void set_led_level(uint8_t lvl)
{
    OCR1B = lvl;
}

// LED states
static uint8_t light_upper = 200; // breathing upper limit
static uint8_t light_lower = 5; // breathing lower limit
static uint16_t light_cycle = 200; // update LED light level ever light_cycle times light_lvl_exec is called
static uint16_t light_cycle_base = 120; // base value of light cycle

static uint8_t light_cool_down = 0; // breathing cool down counter, used at lowest brightness
static bool light_increasing = true;
static uint8_t light_lvl = 0; // brightness

void light_lvl_add(uint8_t x)
{
    if (255 - light_lvl >= x)
        light_lvl += x;
    else
        light_lvl = 255;
}

void light_lvl_sub(uint8_t x)
{
    if (light_lvl >= x)
        light_lvl -= x;
    else
        light_lvl = 0;
}

void light_lvl_exec(void)
{
    static uint16_t t = 0;
    t = (t + 1) % light_cycle;
    if (t != 0) 
        return;

    if (light_cool_down) {
        light_cool_down--;
    } else {
        if (light_increasing)
        {
            light_lvl_add(1);
        } else {
            light_lvl_sub(1);
        }
    }

    if (light_lvl > light_upper) {
        light_increasing = false;
        light_lvl_sub(4);
    }

    if (light_lvl < light_lower && !light_cool_down) {
        light_increasing = true;
        light_cool_down = 24;
        light_lvl = light_lower;
    }

    set_led_level(light_lvl);

    light_cycle = 200 / light_lvl + light_cycle_base;

    dprintf("light_lvl = %d, light_cycle = %d\n", light_lvl, light_cycle);
}

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


void matrix_init(void)
{
    //debug_enable = true; // Uncomment to get debug messages
    //debug_matrix = true; // Print matrix state every time it changes
    //debug_mouse = true;

    // initialize row and col
    unselect_rows();
    init_cols();

    led_init();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(2);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {

                if ((matrix[i] ^ matrix_debouncing[i]) & matrix_debouncing[i])
                {
                    light_lvl_add(50); // Add light level if new key is pressed
                }

                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    light_lvl_exec(); // Update LED

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

/* Column pin configuration
 * col: 0
 * pin: B0
 */
static void  init_cols(void)
{
    // Input with pull-up(DDR:0, PORT:1)
    DDRF  &= ~(1<<0 | 1<<1);
    PORTF |=  (1<<0 | 1<<1);

    DDRE  &= ~(1<<6);
    PORTE |=  (1<<6);

    DDRD  &= ~(1<<4 | 1<<6 | 1<<7);
    PORTD |=  (1<<4 | 1<<6 | 1<<7);

    DDRC  &= ~(1<<7 | 1<<6);
    PORTC |=  (1<<7 | 1<<6);

    DDRB  &= ~(1<<0 | 1<<1 | 1<<3 | 1<<4 | 1<<5 | 1<<7);
    PORTB |=  (1<<0 | 1<<1 | 1<<3 | 1<<4 | 1<<5 | 1<<7);
}

static matrix_row_t read_cols(void)
{
    return
        (PINF&(1<<0) ? 0 : (1<< 0)) |
        (PINF&(1<<1) ? 0 : (1<< 1)) |
        (PINE&(1<<6) ? 0 : (1<< 2)) |
        (PINC&(1<<7) ? 0 : (1<< 3)) |
        (PINC&(1<<6) ? 0 : (1<< 4)) |
        (PINB&(1<<7) ? 0 : (1<< 5)) |
        (PIND&(1<<4) ? 0 : (1<< 6)) |
        (PINB&(1<<1) ? 0 : (1<< 7)) |
        (PINB&(1<<0) ? 0 : (1<< 8)) |
        (PINB&(1<<5) ? 0 : (1<< 9)) |
        (PINB&(1<<4) ? 0 : (1<<10)) |
        (PIND&(1<<7) ? 0 : (1<<11)) |
        (PIND&(1<<6) ? 0 : (1<<12)) |
        (PINB&(1<<3) ? 0 : (1<<13));
}

/* Row pin configuration
 * row: 0
 * pin: B1
 */
static void unselect_rows(void)
{
    // Hi-Z(DDR:0, PORT:0) to unselect
    DDRD  &= ~0b00101111;
    PORTD &= ~0b00101111;
}

static void select_row(uint8_t row)
{
    // Output low(DDR:1, PORT:0) to select
    switch(row) {
    case 0:
        DDRD  |= (1<<0);
        PORTD &= ~(1<<0);
        break;
    case 1:
        DDRD  |= (1<<1);
        PORTD &= ~(1<<1);
        break;
    case 2:
        DDRD  |= (1<<2);
        PORTD &= ~(1<<2);
        break;
    case 3:
        DDRD  |= (1<<3);
        PORTD &= ~(1<<3);
        break;
    case 4:
        DDRD  |= (1<<5);
        PORTD &= ~(1<<5);
        break;
    }
}
