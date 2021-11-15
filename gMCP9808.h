/*!
 * Author: Graham Spaulding
 * Date: Nov 13, 2021
 * Desc: I2C Driver for Microchip's MCP9808 I2C Temp sensor
 * 
 * The manufacturer ID for the MCP9808 is 0x0054 (pg 27)
 * The device ID for the MCP9808 is 0x04 (pg 28)
 */

#ifndef GMCP9808_H
#define GMCP9808_H

#if ARDUINO >= 100  // if arduino IDE is greater the v1.0
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h> // arduino i2c library

#define MCP9808_I2CADDR_DEFAULT 0x18 // default i2c address
// Config register
#define MCP9808_REG_CONFIG 0x01              // Config register (pg 18)
#define MCP9808_REG_CONFIG_SHUTDOWN 0x0100   // bit 8 (pg 18)
#define MCP9808_REG_CONFIG_CRITLOCKED 0x0080 // bit 7 (pg 19)
#define MCP9808_REG_CONFIG_WINLOCKED 0x0040  // bit 6 (pg 19)
#define MCP9808_REG_CONFIG_INTCLR 0x0020     // bit 5 (pg 19)
#define MCP9808_REG_CONFIG_ALERTSTAT 0x0010  // bit 4 (pg 19)
#define MCP9808_REG_CONFIG_ALERTCNT 0x0008   // bit 3 (pg 19)
#define MCP9808_REG_CONFIG_ALERTSEL 0x0004   // bit 2 (pg 19)
#define MCP9808_REG_CONFIG_ALERTPOL 0x0002   // bit 1 (pg 19)
#define MCP9808_REG_CONFIG_ALERTMODE 0x0001  // bit 0 (pg 19)

#define MCP9808_REG_MFC_ID 0x06       // manufacture id reg (pg 27)
#define MCP9808_REG_DEVICE_ID 0x07    // device id reg (pg 28)
#define MCP9808_REG_AMBIENT_TEMP 0x05 // ambient temp reg (pg 24) 
 
/*!
 *    @brief  Class that stores state and functions for the MCP9808.
 */
class gMCP9808 {
public:
	gMCP9808(); // constructor declaration
	bool start(uint8_t addr);
	bool init();

	// get ambient temperature methods
	// float get_temp_f();
	float get_temp_c();

	// read/write methods for 8 and 16 bit registers
	void write8(uint8_t reg, uint8_t val);
	uint8_t read8(uint8_t reg);
	void write16(uint8_t reg, uint16_t val);
	uint16_t read16(uint8_t reg);

private:
	TwoWire *_wire;
	uint8_t _i2caddr;
};

#endif // GMCP9808_H