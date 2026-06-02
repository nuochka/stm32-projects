/*
 * lps24hb.c
 *
 * Created on: Jun 1, 2026
 * Author: anastasiiakovalenko
 */

#include "lps25hb.h"
#include "i2c.h"

#define LPS25HB_ADDR        0xBA

#define LPS25HB_WHO_AM_I 		0x0F
#define LPS25HB_CTRL_REG1 		0x10
#define LPS25HB_CTRL_REG2 		0x11
#define LPS25HB_CTRL_REG3 		0x12
#define LPS25HB_CTRL_REG4 		0x13
#define LPS25HB_PRESS_OUT_XL 	0x28
#define LPS25HB_PRESS_OUT_L 	0x29
#define LPS25HB_PRESS_OUT_H 	0x2A
#define LPS25HB_TEMP_OUT_L 		0x2B
#define LPS25HB_TEMP_OUT_H 		0x2C

#define TIMEOUT                 100

static uint8_t lps_read_reg(uint8_t reg)
{
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1, LPS25HB_ADDR, reg, 1, &value, sizeof(value), TIMEOUT);

    return value;
}

static void lps_write_reg(uint8_t reg, uint8_t value)
{
    HAL_I2C_Mem_Write(&hi2c1, LPS25HB_ADDR, reg, 1, &value, sizeof(value), TIMEOUT);
}

HAL_StatusTypeDef lps25hb_init(void)
{
    if (lps_read_reg(LPS25HB_WHO_AM_I) != 0xB4)
         return HAL_ERROR;

    lps_write_reg(LPS25HB_CTRL_REG1,  0x20);
    return HAL_OK;
}

float lps25hb_read_temp(void)
{
	union {
		uint16_t raw;
	    struct {
			uint8_t low;
	    	uint8_t high;
	    };
	} temp = {0};

     if (HAL_I2C_Mem_Read(&hi2c1, LPS25HB_ADDR, LPS25HB_TEMP_OUT_L, 1, (uint8_t*)&temp.low, 1, TIMEOUT) != HAL_OK)
         Error_Handler();
     if (HAL_I2C_Mem_Read(&hi2c1, LPS25HB_ADDR, LPS25HB_TEMP_OUT_H, 1, (uint8_t*)&temp.high, 1, TIMEOUT) != HAL_OK)
              Error_Handler();

     return temp.raw / 100.0f;
}

float lps25hb_read_pressure(void)
{
     union {
    	 uint32_t raw;
    	 struct {
    		 uint8_t low;
    		 uint8_t mid;
    		 uint8_t high;
    		 uint8_t _reserved;
    	 };
     } pressure = {0};

     if (HAL_I2C_Mem_Read(&hi2c1, LPS25HB_ADDR, LPS25HB_PRESS_OUT_XL, 1, (uint8_t*)&pressure.low, 1, TIMEOUT) != HAL_OK)
         Error_Handler();
     if (HAL_I2C_Mem_Read(&hi2c1, LPS25HB_ADDR, LPS25HB_PRESS_OUT_L, 1, (uint8_t*)&pressure.mid, 1, TIMEOUT) != HAL_OK)
         Error_Handler();
     if (HAL_I2C_Mem_Read(&hi2c1, LPS25HB_ADDR, LPS25HB_PRESS_OUT_H, 1, (uint8_t*)&pressure.high, 1, TIMEOUT) != HAL_OK)
         Error_Handler();

     return pressure.raw / 4096.0f;
}

