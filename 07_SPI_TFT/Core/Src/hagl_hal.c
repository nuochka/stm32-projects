/*
 * hagl_hal.c
 *
 *  Created on: May 27, 2026
 *      Author: anastasiiakovalenko
 */


#include "hagl_hal.h"
#include "lcd.h"

static void put_pixel(void *self, int16_t x, int16_t y, hagl_color_t color) {
	if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT)) {
		lcd_put_pixel(x, y, color);
	}
}

size_t hagl_hal_init(hagl_backend_t *backend) {
    backend->width = DISPLAY_WIDTH;
    backend->height = DISPLAY_HEIGHT;
    backend->put_pixel = put_pixel;

    return 1;
}
