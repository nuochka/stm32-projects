/*
 * mcp23s08.c
 *
 *  Created on: May 25, 2026
 *      Author: anastasiiakovalenko
 */
#include "mcp23s08.h"
#include "spi.h"


void mcp_reg_write(uint8_t reg, uint8_t value){
	uint8_t tx[3] = { 0x40, reg, value };
	HAL_GPIO_WritePin(IOEXP_CS_GPIO_Port, IOEXP_CS_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi2, tx, 3, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(IOEXP_CS_GPIO_Port, IOEXP_CS_Pin, GPIO_PIN_SET);

}

uint8_t mcp_reg_read(uint8_t reg){
	uint8_t txrx[2] = { 0x41, reg, 0xff};
	HAL_GPIO_WritePin(IOEXP_CS_GPIO_Port, IOEXP_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi2, txrx, txrx, 3, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(IOEXP_CS_GPIO_Port, IOEXP_CS_Pin, GPIO_PIN_SET);

	return txrx[2];
}

void MCP_GPIO_WritePin(uint8_t pin, MCP_PinState state){
	if (pin > 7) return;

	uint8_t current_olat = mcp_reg_read(MCP_OLAT);
	if (state == MCP_PIN_RESET){
		current_olat |= (1 << pin);
	} else {
		current_olat &= ~(1 << pin);
	}

	mcp_reg_write(MCP_OLAT, current_olat);
}

int8_t MCP_GPIO_ReadPin(uint8_t pin){
	if (pin > 7) return -1;

	uint8_t iodir = mcp_reg_read(MCP_IODIR);

	if(iodir & (1 << pin) == 0){
		return -1;
	}

	uint8_t current_gpio = mcp_reg_read(MCP_GPIO);
	if((current_gpio & (1 << pin)) != 0) {
		return 1;
	} else {
		return 0;
	}

}
