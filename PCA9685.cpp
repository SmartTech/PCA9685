/*************************************************** 
  This is a library for PCA9685 driver
 ***************************************************/

#include <PCA9685.h>

/**************************************************************************/
/*! 
    @brief  Instantiates a new PCA9685 PWM driver chip with the I2C address on the Wire interface. On Due we use Wire1 since its the interface on the 'default' I2C pins.
    @param  addr The 7-bit I2C address to locate this chip, default is 0x40
*/
/**************************************************************************/
PCA9685::PCA9685(uint8_t addr) :
	#if defined(ARDUINO_SAM_DUE)
	  wire(&Wire1),
	#else
	  wire(&Wire),
	#endif
	pca9685_addr(addr)
{
}

/**************************************************************************/
/*! 
    @brief  Instantiates a new PCA9685 PWM driver chip with the I2C address on a TwoWire interface
    @param  i2c  A pointer to a 'Wire' compatible object that we'll use to communicate with
    @param  addr The 7-bit I2C address to locate this chip, default is 0x40
*/
/**************************************************************************/
#ifdef _WIREBASE_H_
PCA9685::PCA9685(WireBase* _wire, uint8_t addr) :
	wire(_wire), pca9685_addr(addr)
{
}
#endif

/**************************************************************************/
/*! 
    @brief  Setups the I2C interface and hardware
*/
/**************************************************************************/
uint8_t PCA9685::begin(void) {
  wire->begin();
  reset();
  setPWMFreq(DEFAULT_PCA9685_FREQ); // set a default frequency
}


/**************************************************************************/
/*! 
    @brief  Sends a reset command to the PCA9685 chip over I2C
*/
/**************************************************************************/
uint8_t PCA9685::reset(void) {
  write(PCA9685_MODE1, 0x80);
  delay(10);
}

/**************************************************************************/
/*! 
    @brief  Sets the PWM frequency for the entire chip, up to ~1.6 KHz
    @param  freq Floating point frequency that we will attempt to match
*/
/**************************************************************************/
uint8_t PCA9685::setPWMFreq(float freq) {
  // Correct for overshoot in the frequency setting
  freq *= 0.9;
  // Calculate estimated pre-scale
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  // Calculate final pre-scale
  uint8_t prescale = floor(prescaleval + 0.5);
  // Read old mode1 & modify it
  uint8_t oldmode = read(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  write(PCA9685_MODE1, newmode);          // go to sleep
  write(PCA9685_PRESCALE, prescale);      // set the prescaler
  write(PCA9685_MODE1, oldmode);          // back to old mode1
  delay(5);                                // some delay
  write(PCA9685_MODE1, oldmode | 0xa0);   // This sets the MODE1 register to turn on auto increment.
}

/**************************************************************************/
/*! 
    @brief  Sets the PWM output of one of the PCA9685 pins
    @param  num One of the PWM output pins, from 0 to 15
    @param  on At what point in the 4096-part cycle to turn the PWM output ON
    @param  off At what point in the 4096-part cycle to turn the PWM output OFF
*/
/**************************************************************************/
uint8_t PCA9685::setPWM(uint8_t num, uint16_t on, uint16_t off) {
  wire->beginTransmission(pca9685_addr);
  wire->write(LED0_ON_L+4*num);
  wire->write(on);
  wire->write(on>>8);
  wire->write(off);
  wire->write(off>>8);
  wire->endTransmission();
  #ifdef USE_WIRE_DELAY
  delay(1);
  #endif
}

/**************************************************************************/
/*! 
    @brief  Helper to set pin PWM output. Sets pin without having to deal with on/off tick placement and properly handles a zero value as completely off and 4095 as completely on.  Optional invert parameter supports inverting the pulse for sinking to ground.
    @param  num One of the PWM output pins, from 0 to 15
    @param  val The number of ticks out of 4096 to be active, should be a value from 0 to 4095 inclusive.
    @param  invert If true, inverts the output, defaults to 'false'
*/
/**************************************************************************/
uint8_t PCA9685::setPin(uint8_t num, uint16_t val, bool invert)
{
  // Clamp value between 0 and 4095 inclusive.
  val = min(val, (uint16_t)4095);
  if (invert) {
    if (val == 0) {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    }
    else if (val == 4095) {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    }
    else {
      setPWM(num, 0, 4095-val);
    }
  }
  else {
    if (val == 4095) {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    }
    else if (val == 0) {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    }
    else {
      setPWM(num, 0, val);
    }
  }
}

/*******************************************************************************************/

uint8_t PCA9685::read(uint8_t addr) {
  // Write for set register address
  wire->beginTransmission(pca9685_addr);
  wire->write(addr);
  wire->endTransmission();
  #ifdef USE_WIRE_DELAY
  delay(1);
  #endif
  // Read from setting address register
  wire->requestFrom((uint8_t)pca9685_addr, (uint8_t)1);
  return wire->read();
}

uint8_t PCA9685::write(uint8_t addr, uint8_t value) {
  // Write data to register address
  wire->beginTransmission(pca9685_addr);
  wire->write(addr);
  wire->write(value);
  wire->endTransmission();
  #ifdef USE_WIRE_DELAY
  delay(1);
  #endif
}
