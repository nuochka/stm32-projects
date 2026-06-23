/*
 * wire.h
 *
 * Created on: Jun 23, 2026
 * Author: anastasiiakovalenko
 */

#pragma once

#include "stm32l4xx.h"

/**
 * @brief Initializes the 1-Wire hardware timer peripheral.
 * @retval HAL_StatusTypeDef HAL_OK if successful.
 */
HAL_StatusTypeDef wire_init(void);

/**
 * @brief Generates a Reset pulse and checks for a Presence pulse from the slave.
 * @retval HAL_StatusTypeDef HAL_OK if presence pulse detected, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef wire_reset(void);

/**
 * @brief Reads a single byte from the 1-Wire bus.
 * @retval uint8_t Received byte.
 */
uint8_t wire_read(void);

/**
 * @brief Writes a single byte to the 1-Wire bus.
 * @param byte Byte to be transmitted.
 */
void wire_write(uint8_t byte);

/**
 * @brief Calculates the 8-bit CRC.
 * @param data Pointer to the input data buffer.
 * @param len Length of the data buffer.
 * @retval uint8_t Calculated CRC value.
 */
uint8_t wire_crc(const uint8_t* data, int len);
