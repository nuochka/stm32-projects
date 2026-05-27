/*
 * lcd.h
 *
 * Created on: May 25, 2026
 * Author: anastasiiakovalenko
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

/* TFT physical resolution dimensions */
#define LCD_WIDTH  160
#define LCD_HEIGHT 128

/* ST7735S hardware command register definitions */
#define ST7735S_SLPOUT			0x11
#define ST7735S_DISPOFF			0x28
#define ST7735S_DISPON			0x29
#define ST7735S_CASET			0x2a
#define ST7735S_RASET			0x2b
#define ST7735S_RAMWR			0x2c
#define ST7735S_MADCTL			0x36
#define ST7735S_COLMOD			0x3a
#define ST7735S_FRMCTR1			0xb1
#define ST7735S_FRMCTR2			0xb2
#define ST7735S_FRMCTR3			0xb3
#define ST7735S_INVCTR			0xb4
#define ST7735S_PWCTR1			0xc0
#define ST7735S_PWCTR2			0xc1
#define ST7735S_PWCTR3			0xc2
#define ST7735S_PWCTR4			0xc3
#define ST7735S_PWCTR5			0xc4
#define ST7735S_VMCTR1			0xc5
#define ST7735S_GAMCTRP1		0xe0
#define ST7735S_GAMCTRN1		0xe1

/* Standard RGB565 system color macros */
#define BLACK     0x0000
#define RED       0xf800
#define GREEN     0x07e0
#define BLUE      0x001f
#define YELLOW    0xffe0
#define MAGENTA   0xf81f
#define CYAN      0x07ff
#define WHITE     0xffff

/* Core graphics API functions */
void lcd_init(void);
void lcd_put_pixel(int x, int y, uint16_t color);
void lcd_fill_box(int x, int y, int width, int height, uint16_t color);
void lcd_draw_rgba_image(int x, int y, int width, int height, const uint16_t* image_data);
void lcd_copy(void);

/* DMA and hardware flow status functions */
void lcd_transfer_done(void);
bool lcd_is_busy(void);
