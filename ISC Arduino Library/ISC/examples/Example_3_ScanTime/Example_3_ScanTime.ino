/*
   Example_3_ScanTime
   This example configures the ISC and reads the time taken for the last sensing cycle to occur.
*/

#include <ISC.h>

ISC Isc;                      //Create instance of ISC board

void setup() {
  Serial.begin(115200);        //Start communications over Serial Port
  Isc.reset(0x50);             //Establish communication and Reset ISC (Address 0x50)
  
  Isc.begin(basicLineSensor);  //Ensure Board type matches that connected, otherwise 'flip' must be considered
}

void loop() {
  Serial.print("Last Sensing Cycle took ");
  Serial.print(Isc.scanTime());
  Serial.println("us");
  delay(100);
}
