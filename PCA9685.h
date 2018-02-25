/*************************************************** 
  This is a library for PCA9865 PWM driver
 ***************************************************/

#ifndef _PCA9685_H
#define _PCA9685_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

#define PCA9685_SUBADR1      0x02
#define PCA9685_SUBADR2      0x03
#define PCA9685_SUBADR3      0x04

#define PCA9685_MODE1        0x00
#define PCA9685_PRESCALE     0xFE

#define LED0_ON_L            0x06
#define LED0_ON_H            0x07
#define LED0_OFF_L           0x08
#define LED0_OFF_H           0x09

#define ALLLED_ON_L          0xFA
#define ALLLED_ON_H          0xFB
#define ALLLED_OFF_L         0xFC
#define ALLLED_OFF_H         0xFD

#define DEFAULT_PCA9685_ADDR 0x40
#define DEFAULT_PCA9685_FREQ  500

/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with PCA9685 PWM chip
*/
/**************************************************************************/
class PCA9685 {

  private: //--------------------------------------------------------
  
    #define USE_WIRE_DELAY
 
	#ifdef _WIREBASE_H_
	WireBase* wire;
	#else
	TwoWire*  wire;
	#endif
	
	uint8_t pca9685_addr = DEFAULT_PCA9685_ADDR;

	uint8_t read(uint8_t addr);
	uint8_t write(uint8_t addr, uint8_t value);
  
  
  public: //--------------------------------------------------------
 
	PCA9685(uint8_t addr = DEFAULT_PCA9685_ADDR);
	#ifdef _WIREBASE_H_
	PCA9685(WireBase* _wire, uint8_t addr = DEFAULT_PCA9685_ADDR);
	#endif
	
	uint8_t begin(void);
	uint8_t reset(void);
	uint8_t setPWMFreq(float freq);
	uint8_t setPWM(uint8_t num, uint16_t on, uint16_t off);
	uint8_t setPin(uint8_t num, uint16_t val, bool invert=false);
  
};

#endif // _PCA9685_H
