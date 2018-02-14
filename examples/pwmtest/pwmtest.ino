/*************************************************** 
  This is a library for PCA9865 PWM driver
 ***************************************************/

#include <PCA9685.h>

PCA9685 pwm;
//PCA9685 pwm(0x40);
//PCA9685 pwm(&Wire, 0x40);

void setup() {
	
  Serial.begin(115200);
  
  Serial.println("16 channel PWM test!");

  pwm.begin();
  pwm.setPWMFreq(1600);  // This is the maximum PWM frequency

}

void loop() {
	
  // Drive each PWM in a 'wave'
  for (uint16_t i=0; i<4096; i += 8) {
    for (uint8_t pwmnum=0; pwmnum < 16; pwmnum++) {
      pwm.setPWM(pwmnum, 0, (i + (4096/16)*pwmnum) % 4096 );
    }
#ifdef ESP8266
    yield();  // take a breather, required for ESP8266
#endif
  }
  
}
