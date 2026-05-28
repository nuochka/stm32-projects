/*
 * eeprom.h
 *
 * Created on: May 28, 2026
 * Author: anastasiiakovalenko
 */

#pragma once

#include "stm32l4xx_hal.h"

/**
 * @brief Blocks execution until the physical EEPROM internal write cycle completes.
 */
void eeprom_wait(void);

/**
 * @brief Reads a block of data from a specific EEPROM memory address.
 * @param addr: Internal memory address inside the EEPROM.
 * @param data: Pointer to the buffer where received data will be stored.
 * @param size: Number of bytes to read.
 * @retval HAL Status
 */
HAL_StatusTypeDef eeprom_read(uint32_t addr, void* data, uint32_t size);

/**
 * @brief Writes a block of data to a specific EEPROM memory address.
 * @param addr: Internal memory address inside the EEPROM.
 * @param data: Pointer to the data buffer to be transmitted.
 * @param size: Number of bytes to write.
 * @retval HAL Status
 */
HAL_StatusTypeDef eeprom_write(uint32_t addr, void* data, uint32_t size);
