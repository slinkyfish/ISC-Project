#include "Arduino.h"
#include "isc.h"
#include "Wire.h"


void ISC::reset(uint8_t address){
    _address = address;
    Wire.setClock(400000);          ////400kHz mode////

    Wire.begin();                   ////Join I2C bus as a Master device////

    int configured = 0;
    while (!configured)             ////Establish communication with ISC////
    {
        //ISC_firmVer = iscGet(FIRMVER, 1);
        if (ISC::read(0, 1)){            //Non-zero value returned?
            configured = 1;                 //Communications established: Successfully read register
        }else{
            delay(1);                      //Pause before retrying register read
        }
    }

    ISC::write(1, 2, 1);           ////Reset ISC////

    configured = 0;
    while (!configured)             //Ensure ISC is Reset
    {
        if (!(ISC::read(1, 1) & 128)){   //ENABLED off?
            configured = 1;                 //ISC is reset: ENABLED bit is LOW
        }else{
            delay(1);                      //Pause before retrying register read
        }
    }
}

int ISC::read(uint8_t regAddr, uint16_t numVals)
{
    uint16_t tempVal = 0;
    Wire.beginTransmission(_address);
    Wire.write(regAddr);                    //First location to read from
    Wire.endTransmission();
    Wire.requestFrom(_address, numVals);  //Select number of bytes to get from the device (1 or 2 bytes)
    tempVal = Wire.read();                  //Read first byte
    if (numVals == 2)
    {
        tempVal = tempVal << 8;
        tempVal += Wire.read();             //Read and combine second byte
    }
    return tempVal;
}

void ISC::write(uint8_t regAddr, uint16_t data, uint8_t numVals){
    Wire.beginTransmission(_address);
    Wire.write(regAddr);
    if(numVals==1){
      Wire.write(data);
    }else{
       Wire.write(data >> 8);
    Wire.write(data & 0xff);
    }
    Wire.endTransmission();
}

int ISC::scanTime(){
    return(ISC::read(37, 2));
}

uint16_t ISC::getVal(uint8_t sensor){
    return(ISC::read(13+2*(sensor), 2));
}

void ISC::setThreshold(uint8_t sensor, uint16_t threshVal){
    ISC::write(25+2*(sensor), threshVal, 2);
}

void ISC::ledMode(uint8_t mode){
    ISC::write(5, mode, 1);
}

void ISC::configure(uint8_t sensor, uint8_t bits){
    ISC::write(7+(sensor), bits, 1);
}

void ISC::txPulse(uint8_t length){
    ISC::write(6, length, 1);
}

uint8_t ISC::states(){
     return(ISC::read(4, 1));
}

void ISC::begin(uint8_t boardType){
    ISC::write(1, 1 | (boardType<<2), 1);
}

void ISC::leftLED(uint8_t state){
     uint8_t ledState = ISC::read(5, 1) & 0b1<<6;	//preserve rightLED state
	 ledState |= state<<7;							//combine with new left LED state
	 ISC::write(5, ledState, 1);					//send to ISC
}

void ISC::rightLED(uint8_t state){
     uint8_t ledState = ISC::read(5, 1) & 0b1<<7;	//preserve leftLED state
	 ledState |= state<<6;							//combine with new left LED state
	 ISC::write(5, ledState, 1);					//send to ISC
}

void ISC::setInterrupts(uint8_t interrupts){
     ISC::write(2, interrupts, 1);					//send to ISC
}

uint8_t ISC::getInterrupts(){
     return(ISC::read(3, 1));
}

void ISC::clearInterrupts(uint8_t interrupts){
	ISC::write(3, interrupts, 1);					//send to ISC
}
