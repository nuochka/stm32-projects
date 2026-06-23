/*
 * ds18b20.h
 *
 * Created on: Jun 23, 2026
 * Author: anastasiiakovalenko
 */

#pragma once

#include "stm32l4xx.h"

#define DS18B20_ROM_CODE_SIZE	8

/**
 * @brief Standard initialization for DS18B20 driver.
 */
HAL_StatusTypeDef ds18b20_init(void);

/**
 * @brief Fetches the unique 64-bit ROM code address from a single sensor.
 * @param rom_code Pointer to 8-byte buffer to store the address.
 */
HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code);

/**
 * @brief Instructs a specific sensor (or all sensors) to begin temperature conversion.
 * @param rom_code Pointer to 8-byte ROM address, or NULL to address all devices (Skip ROM).
 */
HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code);

/**
 * @brief Reads scratchpad from the sensor and processes the raw temperature.
 * @param rom_code Pointer to 8-byte ROM address, or NULL for Skip ROM.
 * @retval Calculated temperature in degrees Celsius, or 85.0f on error.
 */
float ds18b20_get_temp(const uint8_t* rom_code);
