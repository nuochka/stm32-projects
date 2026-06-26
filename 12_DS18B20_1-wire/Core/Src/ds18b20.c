/*
 * ds18b20.c
 *
 *  Created on: Jun 23, 2026
 *      Author: anastasiiakovalenko
 */

#include <string.h>
#include "ds18b20.h"
#include "wire.h"

#define DS18B20_SCRATCHPAD_SIZE	9

/* DS18B20 Function Commands */
#define DS18B20_READ_ROM		0x33
#define DS18BS20_MATCH_ROM		0x55
#define DS18BS20_SKIP_ROM		0xCC

#define DS18B20_CONVERT_T		0x44
#define DS18B20_READ_SCRATCHPAD	0xBE

HAL_StatusTypeDef ds18b20_init(void){
	return wire_init();
}

HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code){
	int i;
	int crc;

	if(wire_reset() != HAL_OK){
		return HAL_ERROR;
	}

	wire_write(DS18B20_READ_ROM);

	/* Read 8 bytes of internal ROM data */
	for(i = 0; i < DS18B20_ROM_CODE_SIZE; i++){
		rom_code[i] = wire_read();
	}

	/* Perform integrity check on the first 7 bytes against the 8th CRC byte */
	crc = wire_crc(rom_code, DS18B20_ROM_CODE_SIZE - 1);
	if(rom_code[DS18B20_ROM_CODE_SIZE - 1] == crc){
		return HAL_OK;
	} else {
		return HAL_ERROR;
	}
}

/**
 * @brief Internal helper to select specific sensor and send operational command.
 */
static HAL_StatusTypeDef send_cmd(const uint8_t* rom_code, uint8_t cmd){
	int i;
	if(wire_reset() != HAL_OK){
		return HAL_ERROR;
	}

	if(!rom_code){
		/* Broadcast to all sensors on the bus */
		wire_write(DS18BS20_SKIP_ROM);
	} else {
		/* Address a single device via its unique 64-bit ID */
		wire_write(DS18BS20_MATCH_ROM);
		for(i = 0; i < DS18B20_ROM_CODE_SIZE; i++){
			wire_write(rom_code[i]);
		}
	}
	wire_write(cmd);
	return HAL_OK;
}

HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code){
	return send_cmd(rom_code, DS18B20_CONVERT_T);
}

/**
 * @brief Reads the raw 9-byte internal scratchpad storage from the DS18B20.
 */
static HAL_StatusTypeDef ds18b20_read_scratchpad(const uint8_t* rom_code, uint8_t* scratchpad){
	int i;
	uint8_t crc;

	if(send_cmd(rom_code, DS18B20_READ_SCRATCHPAD) != HAL_OK){
		return HAL_ERROR;
	}
	/* Dump scratchpad memory bytes */
	for(i = 0; i < DS18B20_SCRATCHPAD_SIZE; i++){
		scratchpad[i] = wire_read();
	}
	/* Match computed CRC with the 9th byte received from the sensor */
	crc = wire_crc(scratchpad, DS18B20_SCRATCHPAD_SIZE - 1);
	if(scratchpad[DS18B20_SCRATCHPAD_SIZE - 1] == crc){
		return HAL_OK;
	} else {
		return HAL_ERROR;
	}
}

float ds18b20_get_temp(const uint8_t* rom_code){
	uint8_t scratchpad[DS18B20_SCRATCHPAD_SIZE];
	int16_t temp;

	if(ds18b20_read_scratchpad(rom_code, scratchpad) != HAL_OK){
		/* 85.0C is the default hardware power-on value, indicating read error here */
		return 85.0f;
	}
	/* Extract temperature values from byte 0 and byte 1 */
	memcpy(&temp, &scratchpad[0], sizeof(temp));
	return temp / 16.0f;
}

