/*
 * lcd.c
 *
 * Created on: May 25, 2026
 * Author: anastasiiakovalenko
 */

#include "lcd.h"
#include "spi.h"

/* Display hardware panel offsets */
#define LCD_OFFSET_X 1
#define LCD_OFFSET_Y 2

/* Mask to distinguish commands (bit 8 set) from data in the init table */
#define CMD(x) ((x) | 0x100)

/* ST7735S driver configuration registers and gamma settings */
static const uint16_t init_table[] = {
  CMD(ST7735S_FRMCTR1), 0x01, 0x2c, 0x2d,
  CMD(ST7735S_FRMCTR2), 0x01, 0x2c, 0x2d,
  CMD(ST7735S_FRMCTR3), 0x01, 0x2c, 0x2d, 0x01, 0x2c, 0x2d,
  CMD(ST7735S_INVCTR), 0x07,
  CMD(ST7735S_PWCTR1), 0xa2, 0x02, 0x84,
  CMD(ST7735S_PWCTR2), 0xc5,
  CMD(ST7735S_PWCTR3), 0x0a, 0x00,
  CMD(ST7735S_PWCTR4), 0x8a, 0x2a,
  CMD(ST7735S_PWCTR5), 0x8a, 0xee,
  CMD(ST7735S_VMCTR1), 0x0e,
  CMD(ST7735S_GAMCTRP1), 0x0f, 0x1a, 0x0f, 0x18, 0x2f, 0x28, 0x20, 0x22,
                         0x1f, 0x1b, 0x23, 0x37, 0x00, 0x07, 0x02, 0x10,
  CMD(ST7735S_GAMCTRN1), 0x0f, 0x1b, 0x0f, 0x17, 0x33, 0x2c, 0x29, 0x2e,
                         0x30, 0x30, 0x39, 0x3f, 0x00, 0x07, 0x03, 0x10,
  CMD(0xf0), 0x01,
  CMD(0xf6), 0x00,
  CMD(ST7735S_COLMOD), 0x05,
  CMD(ST7735S_MADCTL), 0xa0,
};

/* Local frame buffer storing pixels for the whole screen (160x128) */
static uint16_t frame_buffer[LCD_WIDTH * LCD_HEIGHT];

/* Transmit a single command byte over SPI (DC pin LOW) */
static void lcd_cmd(uint8_t cmd){
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

/* Transmit a single data byte over SPI (DC pin HIGH) */
static void lcd_data(uint8_t data){
	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

/* Parse 9-bit value to send either command or data */
static void lcd_send(uint16_t value){
	if(value & 0x100) {
		lcd_cmd(value);
	} else {
		lcd_data(value);
	}
}

/* Send 16-bit word split into two 8-bit SPI transactions (MSB first) */
static void lcd_data16(uint16_t value){
	lcd_data(value >> 8);
	lcd_data(value);
}

/* Set rendering boundaries (active window) on the display */
static void lcd_set_window(int x, int y, int width, int height){
	lcd_cmd(ST7735S_CASET); /* Column Address Set */
	lcd_data16(LCD_OFFSET_X + x);
	lcd_data16(LCD_OFFSET_X + x + width - 1);

	lcd_cmd(ST7735S_RASET); /* Row Address Set */
	lcd_data16(LCD_OFFSET_Y + y);
	lcd_data16(LCD_OFFSET_Y + y + height - 1);
}

/* Hardware reset and display initialization routine */
void lcd_init(void){
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(100);

	/* Flash initialization registers configuration */
	for(int i = 0; i < sizeof(init_table) / sizeof(uint16_t); i++){
		lcd_send(init_table[i]);
	}

	HAL_Delay(200);
	lcd_cmd(ST7735S_SLPOUT);  /* Wake up display from sleep mode */
	HAL_Delay(120);
	lcd_cmd(ST7735S_DISPON);  /* Turn display main video output ON */
}

/* Direct block fill bypassing the frame buffer (useful for quick clearing) */
void lcd_fill_box(int x, int y, int width, int height, uint16_t color){
	lcd_set_window(x, y, width, height);
	lcd_cmd(ST7735S_RAMWR);
	for(int i = 0; i < width * height; i++){
		lcd_data16(color);
	}
}

/* Write a single pixel color directly into the local RAM buffer */
void lcd_put_pixel(int x, int y, uint16_t color){
	frame_buffer[x + y * LCD_WIDTH] = color;
}

/* Draw raw external 16-bit RGB image array directly to screen window */
void lcd_draw_rgba_image(int x, int y, int width, int height, const uint16_t* image_data) {
	lcd_set_window(x, y, width, height);
	lcd_cmd(ST7735S_RAMWR);

	int total_pixels = width * height;
	for(int i = 0; i < total_pixels; i++) {
		uint16_t pixel = image_data[i];
		lcd_data(pixel >> 8);
		lcd_data(pixel & 0xFF);
	}
}

/* Start pushing the local frame buffer to display via DMA */
void lcd_copy(void) {
	lcd_set_window(0, 0, LCD_WIDTH, LCD_HEIGHT);
	lcd_cmd(ST7735S_RAMWR);

	HAL_GPIO_WritePin(LCD_DC_GPIO_Port, LCD_DC_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

	/* Trigger non-blocking hardware DMA transfer */
	HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)frame_buffer, sizeof(frame_buffer));
}

/* Post-DMA ISR callback routine: releases the CS pin when transfer finishes */
void lcd_transfer_done(void){
	HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
}

/* Check if the SPI hardware peripheral is currently transmitting */
bool lcd_is_busy(void){
	if(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY) {
		return true;
	} else {
		return false;
	}
}
