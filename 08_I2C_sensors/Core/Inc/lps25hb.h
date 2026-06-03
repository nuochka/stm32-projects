/*
 * lps25hb.h
 *
 *  Created on: Jun 1, 2026
 *      Author: anastasiiakovalenko
 */

#pragma once
#include "stm32l4xx.h"

HAL_StatusTypeDef lps25hb_init(void);

float lps25hb_read_temp(void);

float lps25hb_read_pressure(void);

void lps25hb_set_calib(uint16_t value);

