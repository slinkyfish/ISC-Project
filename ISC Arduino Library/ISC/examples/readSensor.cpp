#include <ISC.h>
ISC Isc;

void setup(){
	Serial.begin(115200);		//Start communications over Serial Port
	Isc.reset(0x50);             //Establish communication and Reset ISC (Address 0x50)
  	Isc.setup(SENSOR0, enb+txEnb);   //Setup Left Sensor, enabled with Tx enabled
  	Isc.txPulse(10);                 //10µs Tx on time before sampling begins
  	Isc.begin(basicLineSensor);
}

void loop(){
	Serial.println(Isc.getVal(SENSOR0));
	delay(100);
}
