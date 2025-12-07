#ifndef PROJECT_H
#define PROJECT_H
#include <stdint.h>
#define QIBLA_BEARING_DEGREES 32.0f // aprox degrees od qibla from me
#define BRIGHTNESS_MIN 1
#define BRIGHTNESS_MAX 5
#define BRIGHTNESS_DEFAULT 3
#define NUM_COLOR_THEMES 4

typedef struct {
    int up;
    int down;
    int left;
    int right;
    int center;
} JoystickState;
void open_input(void);

void close_input(void);

float get_compass_heading_degrees(void);

void read_joystick(JoystickState *js);

void apply_joystick_input(const JoystickState *js,
                          int *is_on,
                          int *brightness_level,
                          int *theme_index);

void open_display(void);
void close_display(void);
void clear_display(void);

void display_qibla_arrow(float heading_degrees,
                         int theme_index,
                         int brightness_level);

#endif
