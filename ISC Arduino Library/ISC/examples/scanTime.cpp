#include <ISC.h>
ISC Isc;

void setup(){
	Serial.begin(115200);		//Start communications over Serial Port
	Isc.reset(0x50);             //Establish communication and Reset ISC (Address 0x50)
  	Isc.begin(basicLineSensor);
}

void loop(){
	Serial.println(Isc.scanTime(SENSOR0));
	delay(100);
}
