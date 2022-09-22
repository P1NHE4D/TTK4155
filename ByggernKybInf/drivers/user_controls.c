#include "user_controls.h"
#include "adc.h"

pos_t read_joystick_position() {
    pos_t pos;
    // TODO: check if conversion works... think he mentioned sth. about the mcu not supporting floats
    pos.x = (uint8_t) (adc_read(JOYSTICK_X) / 255) * 100;
    pos.y = (uint8_t) (adc_read(JOYSTICK_Y) / 255) * 100;
    return pos;
}

direction_t read_joystick_direction() {
    pos_t pos = read_joystick_position();
    /*TODO: add switch statement*/
}

uint_8 read_slider(channel_t slider) {
   return (uint8_t) (adc_read(slider) / 255) * 100
}

uint_8 read_button(button_t button) {
    // TODO: check if correct
    switch (button) {
        case JOYSTICK:
            return (PINB & PINB0);
        case LEFT_BUTTON:
            return (PINB & PINB1);
        case RIGHT_BUTTON:
            return (PINB & PINB2);
    }
}