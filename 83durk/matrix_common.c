#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"
#include "matrix.h"
#include "debounce.h"
#include "atomic_util.h"


#ifdef SPLIT_KEYBOARD
#    include "split_common/split_util.h"
#    include "split_common/transactions.h"
#    include <string.h>

#    define ROWS_PER_HAND (MATRIX_ROWS / 2)
#else
#    define ROWS_PER_HAND (MATRIX_ROWS)
#endif

#ifndef MATRIX_IO_DELAY
#    define MATRIX_IO_DELAY 30
#endif

#define DUP_ROWS (ROWS_PER_HAND / 2)

/* matrix state(1:on, 0:off) */
matrix_row_t raw_matrix[MATRIX_ROWS];
matrix_row_t matrix[MATRIX_ROWS];

pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

#ifdef SPLIT_KEYBOARD
// row offsets for each hand
uint8_t thisHand, thatHand;
#endif

#ifdef MATRIX_MASKED
extern const matrix_row_t matrix_mask[];
#endif

pin_t row_pins[DUP_ROWS] = MATRIX_ROW_PINS;
pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

// user-defined overridable functions

__attribute__((weak)) void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__((weak)) void matrix_scan_kb(void) {
    matrix_scan_user();
}

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

// helper functions

inline uint8_t matrix_rows(void) {
    return MATRIX_ROWS;
}

inline uint8_t matrix_cols(void) {
    return MATRIX_COLS;
}

inline bool matrix_is_on(uint8_t row, uint8_t col) {
    return (matrix[row] & ((matrix_row_t)1 << col));
}

inline matrix_row_t matrix_get_row(uint8_t row) {
    // Matrix mask lets you disable switches in the returned matrix data. For example, if you have a
    // switch blocker installed and the switch is always pressed.
#ifdef MATRIX_MASKED
    return matrix[row] & matrix_mask[row];
#else
    return matrix[row];
#endif
}

#if (MATRIX_COLS <= 8)
#    define print_matrix_header() print("\nr/c 01234567\n")
#    define print_matrix_row(row) print_bin_reverse8(matrix_get_row(row))
#elif (MATRIX_COLS <= 16)
#    define print_matrix_header() print("\nr/c 0123456789ABCDEF\n")
#    define print_matrix_row(row) print_bin_reverse16(matrix_get_row(row))
#elif (MATRIX_COLS <= 32)
#    define print_matrix_header() print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
#    define print_matrix_row(row) print_bin_reverse32(matrix_get_row(row))
#endif

void matrix_print(void) {
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        print_hex8(row);
        print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

#ifdef SPLIT_KEYBOARD
bool matrix_post_scan(void) {
    bool changed = false;
    if (is_keyboard_master()) {
        static bool  last_connected              = false;
        matrix_row_t slave_matrix[ROWS_PER_HAND] = {0};
        if (transport_master_if_connected(matrix + thisHand, slave_matrix)) {
            changed = memcmp(matrix + thatHand, slave_matrix, sizeof(slave_matrix)) != 0;

            last_connected = true;
        } else if (last_connected) {
            // reset other half when disconnected
            memset(slave_matrix, 0, sizeof(slave_matrix));
            changed = true;

            last_connected = false;
        }

        if (changed) memcpy(matrix + thatHand, slave_matrix, sizeof(slave_matrix));

        matrix_scan_kb();
    } else {
        transport_slave(matrix + thatHand, matrix + thisHand);

        matrix_slave_scan_kb();
    }

    return changed;
}
#endif

/* `matrix_io_delay ()` exists for backwards compatibility. From now on, use matrix_output_unselect_delay(). */
__attribute__((weak)) void matrix_io_delay(void) {
    wait_us(MATRIX_IO_DELAY);
}
__attribute__((weak)) void matrix_output_select_delay(void) {
    waitInputPinDelay();
}
__attribute__((weak)) void matrix_output_unselect_delay(uint8_t line, bool key_pressed) {
    matrix_io_delay();
}

//col2row
static bool select_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
        setPinOutput_writeLow(pin);
        return true;
    }
    return false;
}
static void unselect_row(uint8_t row) {
    pin_t pin = row_pins[row];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        setPinOutput_writeHigh(pin);
#            else
        setPinInputHigh_atomic(pin);
#            endif
    }
}
static void unselect_rows(void) {
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        unselect_row(x);
    }
}
__attribute__((weak)) void matrix_init_pins(void) {
    unselect_rows();
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(col_pins[x]);
        }
    }
}
__attribute__((weak)) void matrix_read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row) {
    // Start with a clear matrix row
    matrix_row_t current_row_value = 0;

    if (!select_row(current_row)) { // Select row
        return;                     // skip NO_PIN row
    }
    matrix_output_select_delay();

    // For each col...
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    for (uint8_t col_index = 0; col_index < MATRIX_COLS; col_index++, row_shifter <<= 1) {
        uint8_t pin_state = readMatrixPin(col_pins[col_index]);

        // Populate the matrix row with the state of the col pin
        current_row_value |= pin_state ? 0 : row_shifter;
    }

    // Unselect row
    unselect_row(current_row);
    matrix_output_unselect_delay(current_row, current_row_value != 0); // wait for all Col signals to go HIGH

    // Update the matrix
    current_matrix[current_row] = current_row_value;
}
//row2col
static bool select_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
        setPinOutput_writeLow(pin);
        return true;
    }
    return false;
}
static void unselect_col(uint8_t col) {
    pin_t pin = col_pins[col];
    if (pin != NO_PIN) {
#            ifdef MATRIX_UNSELECT_DRIVE_HIGH
        setPinOutput_writeHigh(pin);
#            else
        setPinInputHigh_atomic(pin);
#            endif
    }
}
static void unselect_cols(void) {
    for (uint8_t x = 0; x < MATRIX_COLS; x++) {
        unselect_col(x);
    }
}
__attribute__((weak)) void matrix_init_pins(void) {
    unselect_cols();
    for (uint8_t x = 0; x < ROWS_PER_HAND; x++) {
        if (row_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(row_pins[x]);
        }
    }
}
__attribute__((weak)) void matrix_read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter) {
    bool key_pressed = false;

    // Select col
    if (!select_col(current_col)) { // select col
        return;                     // skip NO_PIN col
    }
    matrix_output_select_delay();

    // For each row...
    for (uint8_t row_index = DUP_ROWS; row_index < ROWS_PER_HAND; row_index++) {
        // Check row pin state
        if (readMatrixPin(row_pins[row_index]) == 0) {
            // Pin LO, set col bit
            current_matrix[row_index] |= row_shifter;
            key_pressed = true;
        } else {
            // Pin HI, clear col bit
            current_matrix[row_index] &= ~row_shifter;
        }
    }

    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed); // wait for all Row signals to go HIGH
}


// CUSTOM MATRIX 'LITE'
__attribute__((weak)) void matrix_init_custom(void) {
    for (unit_t i = 0; i < MATRIX_COLS; i++){
        if ( col_pins[i] != NO_PIN){
            ATOMIC_BLOCK_FORCEON{
                setPinInputHigh(col_pins[i]);
                setPinOutput(col_pins[i]);
            }
        }
    }
    for (unit_t i = 0; i < DUP_ROWS; i++){
        if ( row_pins[i] != NO_PIN){
            ATOMIC_BLOCK_FORCEON{
                setPinInputHigh(row_pins[i+DUP_ROWS])
                setpinOutput(row_pins[i])
            }
        }
    }
}
__attribute__((weak)) bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;
    matrix_row_t tmp_matrix[MATRIX_ROWS] = {0};
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    {
        for (unit8_t current_row = 0; current_row < DUP_ROWS; current_row++){
            matrix_read_cols_on_row(tmp_matrix, current_row);
        }
        for (unit8_t current_col = 0; current_col < MATRIX_COLS; current_col++  row_shifter <<= 1){
            matrix_read_rows_on_col(tmp_matrix, current_col, row_shifter);
        }
            matrix_has_changed = memcmp(current_matrix, tmp_matrix, sizeof(tmp_matrix)) != 0;
        if (matrix_has_changed) memcpy(current_matrix, tmp_matrix, sizeof(tmp_matrix));
    }
    return matrix_has_changed;
}
