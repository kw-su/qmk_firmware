// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "keymap_japanese.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───────┐┌───┬───┐
     * │Esc│ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ = │ Backsp││Ins│PgU│
     * ├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─────┤├───┼───┤
     * │ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ [ │ ] │     ││Del│PgD│
     * ├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┐ Ent│└───┴───┘
     * │ Caps │ A │ S │ D │ F │ G │ H │ J │ K │ L │ ; │ ' │ # │    │
     * ├────┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┴────┤┌───┐
     * │Shft│ \ │ Z │ X │ C │ V │ B │ N │ M │ , │ . │ / │ Shift    ││ ↑ │
     * ├────┼───┴┬──┴─┬─┴───┴───┴───┴───┴───┴──┬┴───┼───┴┬────┬─┬──┴┼───┼───┐
     * │Ctrl│GUI │Alt │                        │ Alt│ GUI│Ctrl│ │ ← │ ↓ │ → │
     * └────┴────┴────┴────────────────────────┴────┴────┴────┘ └───┴───┴───┘
     */
    [0] = LAYOUT_67_iso(
           KC_ESC  ,JP_1    ,JP_2    ,JP_3    ,JP_4    ,JP_5    ,JP_6    ,JP_7    ,JP_8    ,JP_9    ,JP_0    ,JP_MINS ,JP_CIRC ,JP_YEN  ,KC_BSPC ,
             KC_TAB ,JP_Q    ,JP_W    ,JP_E    ,JP_R    ,JP_T    ,JP_Y    ,JP_U    ,JP_I    ,JP_O    ,JP_P    ,JP_AT   ,JP_LBRC    ,KC_ENT       ,
             KC_CAPS ,JP_A    ,JP_S    ,JP_D    ,JP_F    ,JP_G    ,JP_H    ,JP_J    ,JP_K    ,JP_L    ,JP_SCLN ,JP_COLN ,JP_RBRC,
              KC_LSFT ,JP_Z    ,JP_X    ,JP_C    ,JP_V    ,JP_B    ,JP_N    ,JP_M    ,JP_COMM ,JP_DOT  ,JP_SLSH ,JP_BSLS ,         KC_RSFT       ,
        KC_LCTL,MO(1)   ,KC_LCMD ,KC_LOPT ,JP_MHEN ,        KC_SPC            ,JP_HENK ,KC_ROPT,MO(1)   ,KC_RCMD ,MO(2)  ,         KC_RCTL
    ),
    [1] = LAYOUT_67_iso(
           JP_ZKHK ,KC_F1   ,KC_F2   ,KC_F3   ,KC_F4   ,KC_F5   ,KC_F6   ,KC_F7   ,KC_F8   ,KC_F9   ,KC_F10  ,KC_F11  ,	KC_F12  ,_______,KC_DEL ,
             _______,_______,KC_VOLD ,KC_UP   ,KC_VOLU ,KC_MUTE ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______   ,        _______    ,
             _______ ,_______,KC_LEFT ,KC_DOWN ,KC_RGHT ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______  ,
              _______ ,_______,KC_BRID ,_______ ,KC_BRIU ,_______ ,_______ ,_______ ,_______ ,_______ ,_______ ,_______  ,             _______       ,
        _______,KC_PSCR,_______ ,_______ ,_______ ,         _______          ,_______ ,_______ ,MO(1)   ,_______ ,DF(2)   ,         _______
    ),
    [2] = LAYOUT_67_iso(
           KC_ESC  ,KC_NO   ,MI_Cs3  ,MI_Ds3  ,KC_NO   ,MI_Fs3  ,MI_Gs3  ,MI_As3  ,KC_NO   ,MI_Cs4 ,MI_Ds4  ,KC_NO  ,MI_Fs4 ,KC_NO  ,KC_BSPC ,
             KC_NO  ,MI_C3   ,MI_D3   ,MI_E3   ,MI_F3   ,MI_G3   ,MI_A3   ,MI_B3   ,MI_C4   ,MI_D4  ,MI_E4   ,MI_F4   ,MI_G4    ,KC_ENT       ,
             KC_NO   ,KC_NO   ,MI_Fs1  ,MI_Gs1  ,MI_As1  ,KC_NO   ,MI_Cs2  ,MI_Ds2  ,KC_NO   ,MI_Fs2 ,MI_Gs2 ,MI_As2 ,KC_NO,
              MI_OCTD ,MI_F1   ,MI_G1   ,MI_A1   ,MI_B1   ,MI_C2   ,MI_D2   ,MI_E2   ,MI_F2   ,MI_G2  ,MI_A2   ,MI_B2   ,         MI_OCTU     ,
        KC_LCTL,MO(0)  ,_______ ,KC_LOPT ,MI_TRSD ,        KC_SPC            ,MI_TRSU ,KC_ROPT ,MO(1)  ,_______ ,DF(0)  ,         KC_RCTL
    )
};