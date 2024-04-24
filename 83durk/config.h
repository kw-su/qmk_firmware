// Copyright 2023 kwsn (@yamiduru)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define MATRIX_ROWS 5*2
#define MATRIX_COLS 7

#ifndef MIDI_ADVANCED
   #define MIDI_ADVANCED
#endif

#define F_SCL 400000L
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBU





















/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
