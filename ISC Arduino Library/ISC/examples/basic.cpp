#include <Arduino.h>
#include <ISC.h>

ISC Isc;                          //Create instance of ISC board

const uint8_t cornerSensor = 0;   //Give Sensor inputs meaningful names
const uint8_t lSensor = 0;
const uint8_t mSensor = 0;
const uint8_t rSensor = 0;
const uint8_t ssSensor = 0;

void setup()
{
  Serial.begin(9600);

  Isc.reset(0x5);                          //Establish communication and Reset ISC (Address 0x05)

  Isc.setup(ssSensor, enb+txEnb);          //Setup SS Sensor, enabled with Tx enabled
  Isc.setThreshold(ssSensor, 300);         //Set Start / Stop sensor threshold to 300
  Isc.txPulse(10);                         //10us Tx on time before sampling begins
  Isc.ledMode(FASTBLINK);                  //Start LEDs blinking quickly
  
  Isc.begin(spLineSensor);                             //Start main Sampling Loop
}

void loop()
{
  for(int a=0; a<100; ++a){
    //Serial.println(Isc.sensor(ssSensor));
    Serial.print(Isc.states());            //Bits on when above threshold
    Serial.print(", ");
    Serial.print(Isc.sensor(ssSensor));    //Print Start / Stop sensor value
    Serial.print(", ");
    Serial.println(Isc.scanTime()); //Display scanTime register
    delay(100);
  }
}