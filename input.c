#include "project.h"
#include "sense.h"
#include <linux/input.h>
#include <math.h>
#include <stdio.h>

static pi_joystick_t *joystick = NULL;
static pi_i2c_t *magnet = NULL;
static JoystickState *current_js = NULL;
static void joystick_callback(unsigned int code)
{
    if (current_js == NULL) {
        return;
    }

    switch (code) {
    case KEY_UP:
        current_js->up = 1;
        break;
    case KEY_DOWN:
        current_js->down = 1;
        break;
    case KEY_LEFT:
        current_js->left = 1;
        break;
    case KEY_RIGHT:
        current_js->right = 1;
        break;
    case KEY_ENTER:
        current_js->center = 1;
        break;
    default:
        break;
    }
}

void open_input(void)
{
    joystick = getJoystickDevice();
    if (joystick == NULL) {
        fprintf(stderr, "Could not open joystick device.\n");
    }
    magnet = geti2cDevice();
    if (magnet == NULL) {
        fprintf(stderr, "Could not open i2c magnetometer device.\n");
    } else {
        printf("i2c: %s\n", magnet->name);
        configureMag(magnet);
    }
}

void close_input(void)
{
    if (joystick != NULL) {
        freeJoystick(joystick);
        joystick = NULL;
    }
    magnet = NULL;
}
float get_compass_heading_degrees(void)
{
    coordinate_t data;
    float heading;

    if (magnet == NULL) {
        return 0.0f;
    }
    getMagData(magnet, &data);
    heading = atan2f(data.y, data.x) * 180.0f / (float)M_PI;

    if (heading < 0.0f) {
        heading += 360.0f;
    }

    return heading;
}

void read_joystick(JoystickState *js)
{
    if (js == NULL || joystick == NULL) {
        return;
    }

    js->up = 0;
    js->down = 0;
    js->left = 0;
    js->right = 0;
    js->center = 0;

    current_js = js;
    pollJoystick(joystick, joystick_callback, 50);
    current_js = NULL;
}

void apply_joystick_input(const JoystickState *js,
                          int *is_on,
                          int *brightness_level,
                          int *theme_index)
{
    if (js == NULL || is_on == NULL || brightness_level == NULL || theme_index == NULL) {
        return;
    }

    if (js->center) {
        if (*is_on) {
            *is_on = 0;
        } else {
            *is_on = 1;
        }
    }

    if (js->up) {
        if (*brightness_level < BRIGHTNESS_MAX) {
            *brightness_level = *brightness_level + 1;
        }
    }

    if (js->down) {
        if (*brightness_level > BRIGHTNESS_MIN) {
            *brightness_level = *brightness_level - 1;
        }
    }

    if (js->right) {
        *theme_index = *theme_index + 1;
        if (*theme_index >= NUM_COLOR_THEMES) {
            *theme_index = 0;
        }
    }

    if (js->left) {
        *theme_index = *theme_index - 1;
        if (*theme_index < 0) {
            *theme_index = NUM_COLOR_THEMES - 1;
        }
    }
}
