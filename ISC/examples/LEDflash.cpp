#include <ISC.h>
ISC Isc;

void setup(){
	Isc.reset(0x50);             //Establish communication and Reset ISC (Address 0x50)
  	Isc.begin(basicLineSensor);
	ledMode(SLOWBLINK);
}

void loop(){
}
