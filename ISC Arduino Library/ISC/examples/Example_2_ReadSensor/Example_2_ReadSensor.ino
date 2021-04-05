/*
   Example_2_ReadSensor
   This example configures the ISC and a sensor and reads the value every 100ms.
   By consulting the Library Documentation, as an extension, alter the code:
    - to read the raw value (HINT: configuration does not require txEnb)
    - to read two sensors and print to the screen
    - to turn the corresponding LED on for the sensor that is higher
*/

#include <ISC.h>

ISC Isc;                      //Create instance of ISC board

void setup() {
  Serial.begin(115200);        //Start communications over Serial Port
  Isc.reset(0x50);             //Establish communication and Reset ISC (Address 0x50)
  
  Isc.configure(SENSOR1, enb + txEnb);
  Isc.txPulse(10);                      //10µs Tx on time before sampling begins

  Isc.begin(basicLineSensor);  //Ensure Board type matches that connected, otherwise 'flip' must be considered
}

void loop() {
  Serial.println(Isc.getVal(SENSOR1));
  delay(100);
}
