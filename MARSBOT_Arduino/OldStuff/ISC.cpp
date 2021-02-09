#include <arduino.h>
#include <Wire.h>
#include "pinMap.h"
#include "ISC.h"

int ISC_firmVer = 0;

void iscInit()
{
    Wire.setClock(400000);          ////400kHz mode////

    Wire.begin();                   ////Join I2C bus as a Master device////
    
    int configured = 0;
    while (!configured)             ////Establish communication with ISC////
    {
        ISC_firmVer = iscGet(FIRMVER, 1);
        if (ISC_firmVer){            //Non-zero value returned?
            configured = 1;                 //Communications established: Successfully read register
        }else{
            delay(1);                      //Pause before retrying register read
        }
    }

    iscSend8(CONTROL, 2);           ////Reset ISC////

    configured = 0;
    while (!configured)             //Ensure ISC is Reset
    {
        if (!(iscGet(CONTROL, 1) & 128)){   //ENABLED off?
            configured = 1;                 //ISC is reset: ENABLED bit is LOW
        }else{
            delay(1);                      //Pause before retrying register read
        }
    }
}

// int leftSideVal = 0;
// int leftVal = 0;
// int midVal = 0;
// int rightVal = 0;
// int rightSideVal = 0;


void basicLineSensorSetup(){

    iscSend8(CONTROL, (1)<<2);          //SP Line Sensor board connected
    iscSend8(PULSEDUR, 10);             //Pulse Duration:  10us

    iscSend8(SENS0SETUP, 128 | 5);      //Corner Marker Sensor:         Enabled with Tx and rising interrupt
    iscSend8(SENS1SETUP, 5);            //Left Line Sensor:             Enabled with Tx
    iscSend8(SENS2SETUP, 5);            //Right Line Sensor:            Enabled with Tx
    iscSend8(SENS3SETUP, 128 | 5);      //Start/Stop Marker Sensor:     Enabled with Tx and rising interrupt

    iscSend16(SENS0THRSH, 500);         //Corner Marker Threshold
    iscSend16(SENS3THRSH, 500);         //Start/Stop Marker Threshold

    iscSend8(INTERRUPTCTRL, (1<<7) | 0b1001);         //Interrupt for board and markers
}

void basicWallSensorSetup(){

    iscSend8(CONTROL, (2)<<2);          //SP Line Sensor board connected
    iscSend8(PULSEDUR, 10);             //Pulse Duration:  10us

    iscSend8(SENS2SETUP, 5);            //Left Wall Sensor:             Enabled with Tx
    iscSend8(SENS1SETUP, 5);            //Front Wall Sensor:            Enabled with Tx
    iscSend8(SENS0SETUP, 5);            //Right Wall Sensor:            Enabled with Tx

    iscSend16(SENS2THRSH, 500);         //Left Wall Threshold
    iscSend16(SENS1THRSH, 500);         //Front Wall Threshold
    iscSend16(SENS0THRSH, 500);         //Right Wall Threshold

    iscSend8(INTERRUPTCTRL, (1<<7));    //Interrupt for board
}

void spLineSensorSetup(){
    iscSend8(LEDCTRL, 3);
    iscSend8(PULSEDUR, 50);             //Pulse Duration:  10us

    iscSend8(SENS4SETUP, 128 | 5);      //Corner Marker Sensor:         Enabled with Tx and rising interrupt
    iscSend8(LEFTSETUP, 5);            //Left Line Sensor:             Enabled with Tx
    iscSend8(MIDSETUP, 5);            //Middle Line Sensor:           Enabled with Tx
    iscSend8(RIGHTSETUP, 5);            //Right Line Sensor:            Enabled with Tx
    iscSend8(SENS0SETUP, 128 | 5);      //Start/Stop Marker Sensor:     Enabled with Tx and rising interrupt

    iscSend16(SENS4THRSH, 500);         //Corner Marker Threshold
    iscSend16(SENS0THRSH, 500);         //Start/Stop Marker Threshold

    iscSend16(SENS3THRSH, 475);         //Corner Marker Threshold
    iscSend16(SENS2THRSH, 400);         //Start/Stop Marker Threshold
    iscSend16(SENS1THRSH, 475);         //Start/Stop Marker Threshold

    iscSend8(BRDCONFIG, 1);

    iscSend8(INTERRUPTCTRL, (1<<7) | 0b10001);         //Interrupt for board and markers
}

void iscEnable(uint8_t boardType){
    iscSend8(CONTROL, 1 | (boardType<<2)); //ENABLE with specific Sensor Board
}
void iscSend8(uint8_t regAddr, uint8_t data)
{
    Wire.beginTransmission(iscAddress);
    Wire.write(regAddr);
    Wire.write(data);
    Wire.endTransmission();
}

void iscSend16(uint8_t regAddr, uint16_t data)
{
    Wire.beginTransmission(iscAddress);
    Wire.write(regAddr);
    Wire.write(data >> 8);
    Wire.write(data & 0xff);
    Wire.endTransmission();
}

uint16_t iscGet(uint8_t regAddr, uint8_t numRegisters)
{
    uint16_t tempVal = 0;

    Wire.beginTransmission(iscAddress);          //Start Queue 
    Wire.write(regAddr);                         //Add First location to read from to queue
    Wire.endTransmission();                      //Send queued byte over I2C

    Wire.requestFrom(iscAddress, numRegisters);  //Request specific number of bytes to get from the device (1 or 2 bytes)

    tempVal = Wire.read();                       //Read first byte

    if (numRegisters == 2)
        tempVal = (tempVal << 8) + Wire.read();   //Read and combine second byte

    return tempVal;
}


int maxVal = 0;
int minVal = 999;
float myLineError = 0;
int leftVal = 0;
int midVal = 0;
int rightVal = 0;


void getError(){
  int leftVal = iscGet(LEFTVAL, 2);
  int midVal = iscGet(MIDVAL, 2);
  int rightVal = iscGet(RIGHTVAL, 2);


  if (leftVal < rightVal)
  {
    if(midVal>minVal+40) myLineError = midVal - maxVal;
  }
  else
  {
    if(midVal>minVal+40) myLineError = maxVal - midVal;
  }

if(myLineError<0){
  //iscSend8(LEDCTRL, 1 << 6);
} else{
  //iscSend8(LEDCTRL, 1 << 7);
}
}
