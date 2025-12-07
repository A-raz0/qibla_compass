#include "project.h"
#include <math.h>
#include <unistd.h>

static float normalize_angle_0_360(float angle)
{
    while (angle < 0.0f) {
        angle += 360.0f;
    }
    while (angle >= 360.0f) {
        angle -= 360.0f;
    }
    return angle;
}

int main(void)
{
    int is_on = 1;
    int brightness_level = BRIGHTNESS_DEFAULT;
    int theme_index = 0;
    JoystickState js;
    open_input();
    open_display();

    while (1) {
        float heading = get_compass_heading_degrees();
        heading = normalize_angle_0_360(heading);

        read_joystick(&js);

        apply_joystick_input(&js, &is_on, &brightness_level, &theme_index);

        if (is_on) {
            display_qibla_arrow(heading, theme_index, brightness_level);
        } else {
            clear_display();
        }
        usleep(100000);
    }
    close_display();
    close_input();

    return 0;
}
