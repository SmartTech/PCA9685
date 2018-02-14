/*************************************************** 
  This is a library for PCA9865 PWM driver
 ***************************************************/

#include <PCA9685.h>

PCA9685 pwm;
//PCA9685 pwm(0x40);
//PCA9685 pwm(&Wire, 0x40);

void setup() {
  Serial.begin(115200);
  Serial.println("GPIO test!");

  pwm.begin();
  pwm.setPWMFreq(50);
}

void loop() {
	
  // Drive each pin in a 'wave'
  for (uint8_t pin=0; pin<16; pin++) {
	  
    pwm.setPWM(pin, 4096, 0);       // turns pin fully on
    delay(100);
    pwm.setPWM(pin, 0, 4096);       // turns pin fully off
	
  }
  
}
