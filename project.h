#ifndef PROJECT_H
#define PROJECT_H
#include <stdint.h>
#define QIBLA_BEARING_DEG 58.0f // aprox degrees od qibla from me
#define BIGHTNESS_MIN 1
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
