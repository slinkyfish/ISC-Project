/*
 * Example_0_Blink
 * This example configures the ISC and blinks the LEDs slowly.
 * By consulting the Library Documentation, as an extension, alter the code: 
 *  - to blink fast
 *  - to blink slow for 5 seconds, and then fast for 5 seconds repeatedly
 */

#include <ISC.h>

ISC Isc;                      //Create instance of ISC board

void setup() {
  Isc.reset(0x50);             //Establish communication and Reset ISC (Address 0x50)
  Isc.begin(basicLineSensor);  //Board type is required but does not affect lights
  Isc.ledMode(SLOWBLINK);      //Start lights blinking slowly
}

void loop() {                   //Lights carry on blinking slowly whilst your code runs in this loop...
}
