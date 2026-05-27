/*
 * hagl_hal.h
 *
 *  Created on: May 27, 2026
 *      Author: anastasiiakovalenko
 */

#ifndef INC_HAGL_HAL_H_
#define INC_HAGL_HAL_H_

#include <hagl_hal_color.h>
#include "lcd.h"
#include "hagl/bitmap.h"
#include "hagl/backend.h"

#define DISPLAY_WIDTH 	(LCD_WIDTH)
#define DISPLAY_HEIGHT 	(LCD_HEIGHT)
#define DISPLAY_DEPTH 	16

size_t hagl_hal_init(hagl_backend_t *backend);
typedef uint16_t color_t;
#define hagl_hal_put_pixel lcd_put_pixel


#endif /* INC_HAGL_HAL_H_ */
