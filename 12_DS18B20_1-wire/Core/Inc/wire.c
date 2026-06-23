/*
 * wire.c
 *
 *  Created on: Jun 23, 2026
 *      Author: anastasiiakovalenko
 */
#include "wire.h"
#include "gpio.h"
#include "tim.h"

HAL_StatusTypeDef wire_init(void){
	HAL_TIM_Base_Start(&htim6);
	return HAL_OK;
}

static void delay_us(uint32_t us){
	volatile uint32_t count = us * 10;
	while(count--){}
}

HAL_StatusTypeDef wire_reset(void){
	int rc;

	/* Disable interrupts to keep precise 1-Wire timings */
	__disable_irq();

	/* Pull the bus low for at least 480 microseconds */
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
	delay_us(480);

	/* Release the bus and wait for the pull-up resistor to pull it high */
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
	delay_us(70);

	/* Sample the bus to check for a presence pulse from the sensor */
	rc = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin);
	delay_us(410);

	/* Re-enable interrupts */
	__enable_irq();

	/* Sensor pulls the bus low (0) if present */
	if(rc == 0){
		return HAL_OK;
	} else {
		return HAL_ERROR;
	}
}

/**
 * @brief Writes a single bit to the 1-Wire bus using critical time slots.
 * @param value Bit value to write (0 or 1).
 */
static void write_bit(int value){
	__disable_irq();
	if(value) {
		/* Write 1 slot: pull low for <15us, then release */
		HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
		delay_us(6);
		HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
		delay_us(64);
	} else {
		/* Write 0 slot: pull low for at least 60us */
		HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
		delay_us(60);
		HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
		delay_us(10);
	}
	__enable_irq();
}

/**
 * @brief Reads a single bit from the 1-Wire bus.
 * @retval Sampled bit value (0 or 1).
 */
static int read_bit(void){
	int rc;
	__disable_irq();

	/* Initiate read time slot by pulling bus low briefly */
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_RESET);
	delay_us(6);
	HAL_GPIO_WritePin(DS_GPIO_Port, DS_Pin, GPIO_PIN_SET);
	delay_us(9);

	/* Sample the bit state from the sensor */
	rc = HAL_GPIO_ReadPin(DS_GPIO_Port, DS_Pin);
	delay_us(55);
	__enable_irq();
	return rc;
}

void wire_write(uint8_t byte){
	int i;
	/* Transmit data starting from the LSB */
	for(i = 0; i < 8; i++){
		write_bit(byte & 0x01);
		byte >>= 1;
	}
}

uint8_t wire_read(void){
	uint8_t value = 0;
	int i;
	/* Receive data starting from the LSB */
	for(i = 0; i < 8; i++){
		value >>= 1;
		if(read_bit())
			value |= 0x80;
	}
	return value;
}

/**
 * @brief Standard CRC-8 bitwise computation step.
 */
static uint8_t byte_crc(uint8_t crc, uint8_t byte){
	int i;
	for(i = 0; i < 8; i++){
		uint8_t b = crc ^ byte;
		crc >>= 1;
		if(b & 0x01)
			crc ^= 0x8c;
		byte >>= 1;
	}
	return crc;
}

uint8_t wire_crc(const uint8_t* data, int len){
	int i;
	uint8_t crc = 0;
	for(i = 0; i < len; i++){
		crc = byte_crc(crc, data[i]);
	}
	return crc;
}

