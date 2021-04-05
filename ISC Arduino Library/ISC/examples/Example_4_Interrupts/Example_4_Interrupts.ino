/*
   Example_4_Interrupts
   This example configures the SENSOR1 to provide an interrupt when it rises above a value of 500.
   Each time this happens, a counter is incremented
*/

#include <ISC.h>

ISC Isc;                                                  //Create instance of ISC board

int ISCinterrupt = A1;                                    //ISC interrupt is connected to pin A1 on Arduino

int count = 0;                                            //Initialise count variable which will increment on each interrupt

void setup() {
  pinMode(ISCinterrupt, INPUT);                           //Configure Interrupt from ISC to Arduino as an INPUT

  Serial.begin(115200);                                   //Start communications over Serial Port
  Isc.reset(0x50);                                        //Establish communication and Reset ISC (Address 0x50)

  Isc.configure(SENSOR1, enb + txEnb + risingInterrupt);  //Enable SENSOR1 with Tx ambient light removal and interrupt on rising edge
  Isc.setThreshold(SENSOR1, 500);                         //Set threshold of 500 for SENSOR1, when it rises above this, an interrupt occurs

  Isc.setInterrupts(SENSOR1INTERRUPT);                    //Enable interrupt on SENSOR1

  Isc.begin(basicLineSensor);                             //Ensure Board type matches that connected, otherwise 'flip' must be considered
}

void loop() {
  Serial.println(count);

  //Main code goes here - make sure that it loops relatively quickly//
  delay(50);

  if (digitalRead(ISCinterrupt)) handleInterrupts();     //interrupt has occurred
}

void handleInterrupts() {
  int sensorChanged = Isc.getInterrupts();                              //get Interrupt flags to see which sensor changed
  int sensorStates = Isc.states();                                      //get sensor states to see which direction the sensor changed

  if ((sensorChanged & SENSOR0INTERRUPT) && !(sensorStates & SENSOR0INTERRUPT)) { //Sensor 0 falling interrupt
    Serial.println("SENSOR0 Falling Interrupt");
  }
  if ((sensorChanged & SENSOR0INTERRUPT) && (sensorStates & SENSOR0INTERRUPT)) {  //Sensor 0 rising interrupt
    Serial.println("SENSOR0 Rising Interrupt");
  }
  if ((sensorChanged & SENSOR1INTERRUPT) && !(sensorStates & SENSOR1INTERRUPT)) { //Sensor 1 falling interrupt
    Serial.println("SENSOR1 Falling Interrupt");
  }
  if ((sensorChanged & SENSOR1INTERRUPT) && (sensorStates & SENSOR1INTERRUPT)) {  //Sensor 1 rising interrupt
    Serial.println("SENSOR1 Rising Interrupt");
    count++;
  }
  if ((sensorChanged & SENSOR2INTERRUPT) && !(sensorStates & SENSOR2INTERRUPT)) { //Sensor 2 falling interrupt
    Serial.println("SENSOR2 Falling Interrupt");
  }
  if ((sensorChanged & SENSOR2INTERRUPT) && (sensorStates & SENSOR2INTERRUPT)) {  //Sensor 2 rising interrupt
    Serial.println("SENSOR2 Rising Interrupt");
  }
  if ((sensorChanged & SENSOR3INTERRUPT) && !(sensorStates & SENSOR3INTERRUPT)) { //Sensor 3 falling interrupt
    Serial.println("SENSOR3 Falling Interrupt");
  }
  if ((sensorChanged & SENSOR3INTERRUPT) && (sensorStates & SENSOR3INTERRUPT)) {  //Sensor 3 rising interrupt
    Serial.println("SENSOR3 Rising Interrupt");
  }
  if ((sensorChanged & SENSOR4INTERRUPT) && !(sensorStates & SENSOR4INTERRUPT)) { //Sensor 4 falling interrupt
    Serial.println("SENSOR4 Falling Interrupt");
  }
  if ((sensorChanged & SENSOR4INTERRUPT) && (sensorStates & SENSOR4INTERRUPT)) {  //Sensor 4 rising interrupt
    Serial.println("SENSOR4 Rising Interrupt");
  }
  Isc.clearInterrupts(sensorChanged);                                              //Clear interrupt flags as they have been handled
}
