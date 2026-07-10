#pragma once

#ifdef VIA_ENABLE
/* VIA configuration. */
#    define DYNAMIC_KEYMAP_LAYER_COUNT 7
#endif // VIA_ENABLE

#ifndef __arm__
/* Disable unused features. */
#    define NO_ACTION_ONESHOT
#endif // __arm__

/* Charybdis-specific features. */

#ifdef POINTING_DEVICE_ENABLE
// Automatically enable the pointer layer when moving the trackball.  See also:
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS`
// - `CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD`
// #define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    define CHARYBDIS_DRAGSCROLL_REVERSE_Y
#endif // POINTING_DEVICE_ENABLE

// Override matrix pins for custom 3x6 pinout
#undef MATRIX_ROW_PINS
#undef MATRIX_COL_PINS
#undef DIODE_DIRECTION

#define MATRIX_COL_PINS { GP8, GP27, GP28, GP21, GP6, GP7 }
#define MATRIX_ROW_PINS { GP29, GP26, GP5, GP4 }
#define DIODE_DIRECTION COL2ROW
