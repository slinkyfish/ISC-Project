/*
 * Example_1_LeftLED
 * This example configures the ISC and controls th Left LED directly.
 * By consulting the Library Documentation, as an extension, alter the code: 
 *  - to control the right LED directly
 *  - to blink slow for 5 seconds, and then turn the Left LED on (HINT: see example 1 for blink)
 */

#include <ISC.h>

ISC Isc;                      //Create instance of ISC board

void setup() {
  Isc.reset(0x50);             //Establish communication and Reset ISC (Address 0x50)
  Isc.begin(basicLineSensor);  //Board type is required but does not affect lights
  Isc.leftLED(HIGH);           //Turn Left LED on
}

void loop() {
}
