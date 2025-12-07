#include "project.h"
#include "sense.h"
#include <math.h>
#include <stdio.h>

static pi_framebuffer_t *fb = NULL;
static void set_pixel(int x, int y, uint16_t color)
{
    sense_fb_bitmap_t *bm;

    if (fb == NULL) {
        return;
    }

    bm = fb->bitmap;
    if (bm == NULL) {
        return;
    }

    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return;
    }

    bm->pixel[x][y] = color;
}
static void fill_screen(uint16_t color)
{
    int x, y;

    if (fb == NULL) {
        return;
    }

    for (y = 0; y < 8; y++) {
        for (x = 0; x < 8; x++) {
            set_pixel(x, y, color);
        }
    }
}

void open_display(void)
{
    if (fb == NULL) {
        fb = getFrameBuffer();
        if (fb == NULL) {
            fprintf(stderr, "ERROR: getFrameBuffer() failed\n");
        } else {
            clearFrameBuffer(fb, 0x0000);
        }
    }
}

void close_display(void)
{
    if (fb != NULL) {
        clearFrameBuffer(fb, 0x0000);
        freeFrameBuffer(fb);
        fb = NULL;
    }
}

void clear_display(void)
{
    if (fb != NULL) {
        clearFrameBuffer(fb, 0x0000);
    }
}
typedef struct {
    int x;
    int y;
} Pixel;

static const Pixel ARROW_UP_PIXELS[] = {
    {3, 1}, {3, 2}, {3, 3}, {3, 4},
    {2, 2}, {4, 2},
    {1, 3}, {5, 3}
};
static const int ARROW_UP_COUNT = (int)(sizeof(ARROW_UP_PIXELS) / sizeof(Pixel));

static const Pixel ARROW_UP_RIGHT_PIXELS[] = {
    {4, 1}, {5, 2}, {6, 3}, {5, 4},
    {3, 2}, {4, 3}
};
static const int ARROW_UP_RIGHT_COUNT = (int)(sizeof(ARROW_UP_RIGHT_PIXELS) / sizeof(Pixel));

static const Pixel ARROW_RIGHT_PIXELS[] = {
    {4, 3}, {5, 3}, {6, 3},
    {5, 2}, {5, 4},
    {4, 2}, {4, 4}
};
static const int ARROW_RIGHT_COUNT = (int)(sizeof(ARROW_RIGHT_PIXELS) / sizeof(Pixel));

static const Pixel ARROW_DOWN_RIGHT_PIXELS[] = {
    {4, 4}, {5, 5}, {6, 6}, {5, 3},
    {3, 4}, {4, 5}
};
static const int ARROW_DOWN_RIGHT_COUNT = (int)(sizeof(ARROW_DOWN_RIGHT_PIXELS) / sizeof(Pixel));

static const Pixel ARROW_DOWN_PIXELS[] = {
    {3, 3}, {3, 4}, {3, 5}, {3, 6},
    {2, 4}, {4, 4},
    {1, 5}, {5, 5}
};
static const int ARROW_DOWN_COUNT = (int)(sizeof(ARROW_DOWN_PIXELS) / sizeof(Pixel));

static const Pixel ARROW_DOWN_LEFT_PIXELS[] = {
    {2, 4}, {1, 5}, {0, 6}, {2, 3},
    {3, 4}, {1, 3}
};
static const int ARROW_DOWN_LEFT_COUNT = (int)(sizeof(ARROW_DOWN_LEFT_PIXELS) / sizeof(Pixel));

static const Pixel ARROW_LEFT_PIXELS[] = {
    {1, 3}, {2, 3}, {3, 3},
    {2, 2}, {2, 4},
    {1, 2}, {1, 4}
};
static const int ARROW_LEFT_COUNT = (int)(sizeof(ARROW_LEFT_PIXELS) / sizeof(Pixel));

static const Pixel ARROW_UP_LEFT_PIXELS[] = {
    {2, 2}, {1, 1}, {0, 0}, {2, 3},
    {3, 2}, {1, 3}
};
static const int ARROW_UP_LEFT_COUNT = (int)(sizeof(ARROW_UP_LEFT_PIXELS) / sizeof(Pixel));

typedef enum {
    DIR_UP = 0,
    DIR_UP_RIGHT,
    DIR_RIGHT,
    DIR_DOWN_RIGHT,
    DIR_DOWN,
    DIR_DOWN_LEFT,
    DIR_LEFT,
    DIR_UP_LEFT
} Direction;
static const Pixel *get_arrow_pixels(Direction dir, int *count_out)
{
    switch (dir) {
    case DIR_UP:
        *count_out = ARROW_UP_COUNT;
        return ARROW_UP_PIXELS;
    case DIR_UP_RIGHT:
        *count_out = ARROW_UP_RIGHT_COUNT;
        return ARROW_UP_RIGHT_PIXELS;
    case DIR_RIGHT:
        *count_out = ARROW_RIGHT_COUNT;
        return ARROW_RIGHT_PIXELS;
    case DIR_DOWN_RIGHT:
        *count_out = ARROW_DOWN_RIGHT_COUNT;
        return ARROW_DOWN_RIGHT_PIXELS;
    case DIR_DOWN:
        *count_out = ARROW_DOWN_COUNT;
        return ARROW_DOWN_PIXELS;
    case DIR_DOWN_LEFT:
        *count_out = ARROW_DOWN_LEFT_COUNT;
        return ARROW_DOWN_LEFT_PIXELS;
    case DIR_LEFT:
        *count_out = ARROW_LEFT_COUNT;
        return ARROW_LEFT_PIXELS;
    case DIR_UP_LEFT:
        *count_out = ARROW_UP_LEFT_COUNT;
        return ARROW_UP_LEFT_PIXELS;
    default:
        *count_out = 0;
        return NULL;
    }
}
static Direction angle_to_direction(float relative_angle)
{
    if (relative_angle > -22.5f && relative_angle <= 22.5f) {
        return DIR_UP;
    } else if (relative_angle > 22.5f && relative_angle <= 67.5f) {
        return DIR_UP_RIGHT;
    } else if (relative_angle > 67.5f && relative_angle <= 112.5f) {
        return DIR_RIGHT;
    } else if (relative_angle > 112.5f && relative_angle <= 157.5f) {
        return DIR_DOWN_RIGHT;
    } else if (relative_angle > 157.5f || relative_angle <= -157.5f) {
        return DIR_DOWN;
    } else if (relative_angle > -157.5f && relative_angle <= -112.5f) {
        return DIR_DOWN_LEFT;
    } else if (relative_angle > -112.5f && relative_angle <= -67.5f) {
        return DIR_LEFT;
    } else {
        return DIR_UP_LEFT;
    }
}

static uint16_t make_color(int r5, int g6, int b5)
{
    if (r5 < 0) r5 = 0;
    if (r5 > 31) r5 = 31;
    if (g6 < 0) g6 = 0;
    if (g6 > 63) g6 = 63;
    if (b5 < 0) b5 = 0;
    if (b5 > 31) b5 = 31;

    return (uint16_t)((r5 << 11) | (g6 << 5) | b5);
}
static void get_base_theme_components(int theme_index,
                                      int *arrow_r5, int *arrow_g6, int *arrow_b5,
                                      int *bg_r5, int *bg_g6, int *bg_b5)
{
    *bg_r5 = 0;
    *bg_g6 = 0;
    *bg_b5 = 0;

    switch (theme_index) {
    case 0:
        *arrow_r5 = 0;
        *arrow_g6 = 63;
        *arrow_b5 = 0;
        break;
    case 1:
        *arrow_r5 = 31;
        *arrow_g6 = 63;
        *arrow_b5 = 31;
        break;
    case 2:
        *arrow_r5 = 0;
        *arrow_g6 = 0;
        *arrow_b5 = 0;
        *bg_r5 = 24;
        *bg_g6 = 48;
        *bg_b5 = 24;
        break;
    case 3:
        *arrow_r5 = 31;
        *arrow_g6 = 0;
        *arrow_b5 = 0;
        break;
    default:
        *arrow_r5 = 31;
        *arrow_g6 = 63;
        *arrow_b5 = 31;
        break;
    }
}
static void scale_brightness(int brightness_level,
                             int base_r5, int base_g6, int base_b5,
                             int *out_r5, int *out_g6, int *out_b5)
{
    if (brightness_level < BRIGHTNESS_MIN) {
        brightness_level = BRIGHTNESS_MIN;
    }
    if (brightness_level > BRIGHTNESS_MAX) {
        brightness_level = BRIGHTNESS_MAX;
    }
    *out_r5 = (base_r5 * brightness_level) / BRIGHTNESS_MAX;
    *out_g6 = (base_g6 * brightness_level) / BRIGHTNESS_MAX;
    *out_b5 = (base_b5 * brightness_level) / BRIGHTNESS_MAX;
}
void display_qibla_arrow(float heading_degrees,
                         int theme_index,
                         int brightness_level)
{
    float qibla = QIBLA_BEARING_DEGREES;
    float relative_angle;
    Direction dir;
    const Pixel *pixels;
    int count;
    int i;

    int base_arrow_r5, base_arrow_g6, base_arrow_b5;
    int base_bg_r5, base_bg_g6, base_bg_b5;
    int arrow_r5, arrow_g6, arrow_b5;
    int bg_r5, bg_g6, bg_b5;

    uint16_t arrow_color;
    uint16_t bg_color;

    while (heading_degrees < 0.0f) {
        heading_degrees += 360.0f;
    }
    while (heading_degrees >= 360.0f) {
        heading_degrees -= 360.0f;
    }

    while (qibla < 0.0f) {
        qibla += 360.0f;
    }
    while (qibla >= 360.0f) {
        qibla -= 360.0f;
    }
    relative_angle = qibla - heading_degrees;

    while (relative_angle > 180.0f) {
        relative_angle -= 360.0f;
    }
    while (relative_angle <= -180.0f) {
        relative_angle += 360.0f;
    }

    dir = angle_to_direction(relative_angle);

    get_base_theme_components(theme_index,
                              &base_arrow_r5, &base_arrow_g6, &base_arrow_b5,
                              &base_bg_r5, &base_bg_g6, &base_bg_b5);

    scale_brightness(brightness_level,
                     base_arrow_r5, base_arrow_g6, base_arrow_b5,
                     &arrow_r5, &arrow_g6, &arrow_b5);
    scale_brightness(brightness_level,
                     base_bg_r5, base_bg_g6, base_bg_b5,
                     &bg_r5, &bg_g6, &bg_b5);

    arrow_color = make_color(arrow_r5, arrow_g6, arrow_b5);
    bg_color = make_color(bg_r5, bg_g6, bg_b5);
    fill_screen(bg_color);
    pixels = get_arrow_pixels(dir, &count);
    if (pixels == NULL) {
        return;
    }
    for (i = 0; i < count; i++) {
        int x = pixels[i].x;
        int y = pixels[i].y;

        if (theme_index == 2) {
            set_pixel(x, y, make_color(0, 0, 0));
        } else {
            set_pixel(x, y, arrow_color);
        }
    }
}
