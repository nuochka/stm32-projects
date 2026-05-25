/*
 * mcp23s08.h
 *
 * Created on: May 25, 2026
 * Author: anastasiiakovalenko
 */

#pragma once

#include <stdint.h>

// Core Registers (Most frequently used)
#define MCP_IODIR       0x00    // I/O Direction: 1 = Pin is configured as an input, 0 = Output
#define MCP_GPPU        0x06    // GPIO Pull-Up Resistor: 1 = Enable internal 100kΩ pull-up resistor
#define MCP_GPIO        0x09    // General Purpose I/O: Reflects the logic level on the physical pins
#define MCP_OLAT        0x0A    // Output Latch: Modifies the output latches (controls output pins)

// Configuration and Interrupt Registers
#define MCP_IPOL        0x01    // Input Polarity: 1 = Inverts the polarity of the input pin data
#define MCP_GPINTEN     0x02    // Interrupt-on-Change Control: 1 = Enables interrupt-on-change for the pin
#define MCP_DEFVAL      0x03    // Default Value: The default value comparison register for interrupts
#define MCP_INTCON      0x04    // Interrupt Control: 1 = Compare against DEFVAL, 0 = Compare against previous pin state
#define MCP_IOCON       0x05    // I/O Expander Configuration: Controls device operating features (e.g., INT pin behavior)
#define MCP_INTF        0x07    // Interrupt Flag: 1 = Indicates which pin caused the interrupt
#define MCP_INTCAP      0x08    // Interrupt Capture: Captures the logic state of the pins at the moment the interrupt occurred


typedef enum {
	MCP_PIN_RESET = 0,
	MCP_PIN_SET = 1
} MCP_PinState;

/**
 * @brief  Reads a 1-byte value from a specific MCP23S08 register via SPI.
 * @param  reg: The address of the target register (e.g., MCP_GPIO).
 * @retval The 8-bit data read from the register.
 */
uint8_t mcp_reg_read(uint8_t reg);

/**
 * @brief  Writes a 1-byte value to a specific MCP23S08 register via SPI.
 * @param  reg: The address of the target register (e.g., MCP_IODIR).
 * @param  value: The 8-bit data byte to write into the register.
 * @retval None.
 */
void mcp_reg_write(uint8_t reg, uint8_t value);

/**
 * @brief  Sets the logic state (0 or 1) of a specific port expander pin.
 * @param  pin: Pin number to modify (0 to 7).
 * @param  state: Target logic state (MCP_PIN_RESET or MCP_PIN_SET).
 * @retval None
 */
void MCP_GPIO_WritePin(uint8_t pin, MCP_PinState state);

/**
 * @brief  Reads the physical logic state of a specific pin after verifying it is an input.
 * @param  pin: Pin number to read (0 to 7).
 * @retval 0 or 1 if the pin is an input, or -1 if the pin is configured as an output.
 */
int8_t MCP_GPIO_ReadPin(uint8_t pin);
