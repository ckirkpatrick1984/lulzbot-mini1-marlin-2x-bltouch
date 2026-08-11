/**********************
 * Lulzbot_Extras.cpp *
 **********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

#include "Marlin.h"
#include "module/planner.h"
#include "module/endstops.h"
#include "module/stepper_indirection.h"
#include "gcode/gcode.h"
#if ENABLED(LULZBOT_USE_TOUCH_UI)
    #include "lcd/extensible_ui/lib/ftdi_eve_lib/basic/ftdi_basic.h"
#endif
#include "Lulzbot_Extras.h"

/******************************** EMI MITIGATION *******************************/

#define LULZBOT_EMI_SHUTOFF(pin)             SET_OUTPUT(pin); WRITE(pin, LOW);

void LULZBOT_ON_STARTUP(void) {
    EnableProbePins::enable(false);

    #if defined(LULZBOT_USE_ARCHIM2)
        LULZBOT_EMI_SHUTOFF(GPIO_PB1_J20_5)
        LULZBOT_EMI_SHUTOFF(GPIO_PB0_J20_6)
        LULZBOT_EMI_SHUTOFF(GPIO_PB3_J20_7)
        LULZBOT_EMI_SHUTOFF(GPIO_PB2_J20_8)
        LULZBOT_EMI_SHUTOFF(GPIO_PB6_J20_9)
        LULZBOT_EMI_SHUTOFF(GPIO_PB5_J20_10)
        LULZBOT_EMI_SHUTOFF(GPIO_PB8_J20_11)
        LULZBOT_EMI_SHUTOFF(GPIO_PB4_J20_12)
        LULZBOT_EMI_SHUTOFF(GPIO_PB9_J20_13)
        LULZBOT_EMI_SHUTOFF(GPIO_PB7_J20_14)
        LULZBOT_EMI_SHUTOFF(GPIO_PB14_J20_17)
        LULZBOT_EMI_SHUTOFF(GPIO_PA18_J20_21)
        LULZBOT_EMI_SHUTOFF(GPIO_PA17_J20_22)
    #endif

    #if defined(LULZBOT_USE_Z_BELT)
        enable_Z();
        #if defined(LULZBOT_IS_TAZ)
            queue.inject_P(PSTR("G28 Z"));
        #endif
    #endif
}

void LULZBOT_ON_REFLASH() {
    #if ENABLED(LULZBOT_USE_TOUCH_UI)
        CLCD::set_brightness(0);
    #endif
}

/* Enable the probe pins only only when homing/probing,
 * as this helps reduce EMI by grounding the lines.
 *
 * On Mini:
 *   Z_MIN_PIN are the bed washers.
 *
 * On TAZ:
 *   Z_MIN_PIN corresponds to the Z-Home push button.
 *   Z_MIN_PROBE_PIN are the bed washers.
 */
/* Project change (not upstream LulzBot): with a BLTouch, the "disable"
 * half of this must NOT drive the pin. Z_MIN_PIN is the stock bed-washer
 * probe's passive contact - grounding it as an output when idle is a
 * sound EMI measure for a bare wire to a washer. It is wrong for a
 * BLTouch: that same pin is now the BLTouch's signal OUTPUT, and a
 * BLTouch v3 drives it push-pull, so LULZBOT_EMI_SHUTOFF puts the AVR's
 * output driver in direct contention with the BLTouch's whenever the
 * probe reports a trigger outside G28/G29/M119 - a dead short between
 * two active drivers, which risks both the probe and the MCU pin.
 *
 * For the BLTouch build the pin is therefore left as a pulled-up input
 * at all times. The EMI rationale doesn't transfer anyway: a BLTouch
 * signal line is an actively driven digital output, not a long passive
 * wire to a capacitive bed, so there is no floating line to quiet.
 */
#if defined(LULZBOT_USE_BLTOUCH)
    #define LULZBOT_SET_PIN_STATE(pin, enable) \
        SET_INPUT(pin); \
        WRITE(pin, HIGH);
#else
#define LULZBOT_SET_PIN_STATE(pin, enable) \
    if(enable) { \
        /* Set as inputs with pull-up resistor */ \
        SET_INPUT(pin); \
        WRITE(pin, HIGH); \
        delay(5); /* The bed acts as a capacitor and takes a while to charge up */ \
    } else { \
        LULZBOT_EMI_SHUTOFF(pin); \
    }
#endif

#if defined(LULZBOT_USE_AUTOLEVELING) && !defined(LULZBOT_Z_MIN_PROBE_PIN)

     void EnableProbePins::enable(const bool enable) {
         endstops.enable_z_probe(enable);
         LULZBOT_SET_PIN_STATE(Z_MIN_PIN, enable)
         LULZBOT_EXTRUDER_MOTOR_SHUTOFF_ON_PROBE(enable)
     }

#elif defined(LULZBOT_USE_AUTOLEVELING) && defined(LULZBOT_Z_MIN_PROBE_PIN)

     void EnableProbePins::enable(const bool enable) {
         endstops.enable_z_probe(enable);
         LULZBOT_SET_PIN_STATE(Z_MIN_PIN, enable)
         LULZBOT_SET_PIN_STATE(LULZBOT_Z_MIN_PROBE_PIN, enable)
     }

#else

     void EnableProbePins::enable(const bool) {
     }

#endif