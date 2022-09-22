
typedef struct pos_t {
	int8_t x, y;
} pos_t;
enum direction_t { UP, RIGHT, DOWN, LEFT, NEUTRAL };
enum button_t {JOYSTICK, LEFT_BUTTON, RIGHT_BUTTON};


pos_t read_joystick_position(void);
direction_t read_joystick_direction(void);
uint_8 read_slider(channel_t slider);