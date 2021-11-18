/*!
 * 
 */

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
#include "TinyWireM.h"
#define Wire TinyWireM
#else
#include <Wire.h>
#endif

#include "gMCP9808.h"
#include <stdexcept>

gMCP9808::gMCP9808() {}

/*!
 *    @brief  Setups the HW
 *    @param  addr
 *    @return True if initialization was successful, otherwise false.
 */
bool gMCP9808::start(uint8_t addr) {
	_i2caddr = addr;
	_wire = &Wire;
	return init();
}

/*!
 *    @brief  init function
 *    @return True if initialization was successful, otherwise false.
 */
bool gMCP9808::init() {
	_wire->begin();

	if (read16(MCP9808_REG_MFC_ID) != 0x0054)
    	return false;
 	if (read16(MCP9808_REG_DEVICE_ID) != 0x0400)
    	return false;

    write16(MCP9808_REG_CONFIG, 0x0);
    return true;
}

/*!
 *    @brief 
 */
float gMCP9808::get_temp_c() {
	float ambient_temp = NAN;
	uint16_t ambient_temp_reg = read16(MCP9808_REG_AMBIENT_TEMP);

	if (ambient_temp_reg != 0xFFFF) {
		ambient_temp_reg &= 0x0FFF;
		float upper_byte = (float) (ambient_temp_reg >> 8) * 16;
		float lower_byte = (float) (ambient_temp_reg & 0x00FF) * 0.0625;
		if (ambient_temp_reg & 0x1000) {
			ambient_temp = (float) 256 - (upper_byte + lower_byte);
		} else {
			ambient_temp = upper_byte + lower_byte;
		}
	}

	return ambient_temp;
}

/*!
 *    @brief Gets the current resolution
 *    @return Current resolution.
 * 			  	00 -> 0.5 deg C
 * 				01 -> 0.25 deg C
 * 				10 -> 0.125 deg C
 * 				11 -> 0.0625 deg C
 */
uint8_t gMCP9808::get_resolution() {
	return read8(MCP9808_REG_RESOLUTION) & 3;
}

/*!
 *    @brief Sets the current resolution
 *    @param resolution
 * 			  	00 -> 0.5 deg C
 * 				01 -> 0.25 deg C
 * 				10 -> 0.125 deg C
 * 				11 -> 0.0625 deg C
 */
void gMCP9808::set_resolution(uint8_t resolution) {
	write8(MCP9808_REG_RESOLUTION, resolution & 3);
}

/*!
 *    @brief low level 8-bit write procedure
 */
void gMCP9808::write8(uint8_t reg, uint8_t value) {
  _wire->beginTransmission(_i2caddr);
  _wire->write((uint8_t)reg);
  _wire->write(value);
  _wire->endTransmission();
}

/*!
 *    @brief low level 8-bit read procedure
 */
uint8_t gMCP9808::read8(uint8_t reg) {
	uint8_t val = 0xFF;
	uint8_t state;

	_wire->beginTransmission(_i2caddr); // device to write to
	_wire->write((uint8_t) reg);        // register to read from
	state = _wire->endTransmission();   // success if state = 0

	if (state == 0) {
		_wire->requestFrom((uint8_t) _i2caddr, (uint8_t) 1); // request 1 byte
		val = _wire->read();
	}

	return val;
}

/*!
 *    @brief low level 16-bit write procedure
 */
void gMCP9808::write16(uint8_t reg, uint16_t val) {
	_wire->beginTransmission(_i2caddr);
	_wire->write((uint8_t) reg);
	_wire->write(val >> 8);
	_wire->write(val & 0xFF);
	_wire->endTransmission();
}

/*!
 *    @brief low level 16-bit read procedure
 */
uint16_t gMCP9808::read16(uint8_t reg) {
 	uint16_t val = 0xFFFF;
	uint8_t state;

	_wire->beginTransmission(_i2caddr);
	_wire->write((uint8_t)reg);
	state = _wire->endTransmission();

	if (state == 0) {
	_wire->requestFrom((uint8_t)_i2caddr, (uint8_t)2);
	val = _wire->read();
	val <<= 8;
	val |= _wire->read();
	}

	return val;
}