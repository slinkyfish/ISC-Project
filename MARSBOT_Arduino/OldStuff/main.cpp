#include <Arduino.h>
#include <Wire.h>           //Used for I2C Interface
#include "pinMap.h"
#include "odometry.h"
#include "functions.h"
#include "ISC.h"

const int boardType = SP_LINE_SENSOR;

void calibrate(){                   //Measure and store the Middle sensor minimum and maximum values
  midVal = iscGet(MIDVAL, 2);

  if (midVal > maxVal)
    maxVal = midVal;

  if (midVal < minVal)
    minVal = midVal;
}

void setup()
{
  setupPinModes();            //Configure I/O for each pin

  if(funcSelect()&1)
    digitalWrite(mainLED, 1);    //LED ON = Motors will be enabled - bit 1 on DIP Switch!!

  attachInterrupt(digitalPinToInterrupt(lInterruptPin), lCounter, CHANGE); //Start monitoring Left Wheel encoder count
  attachInterrupt(digitalPinToInterrupt(rInterruptPin), rCounter, CHANGE); //Start monitoring Right Wheel encoder count

  timerInterrupt();           //Configure 1kHz Timer interrupt
  interrupts();               //Start periodic interrupt

  Serial.begin(9600);         //Start Communications with PC Terminal

  iscInit();                  //Start comms with ISC and initialise

  Serial.print("Connected to ISC, Firmware Version: ");
  Serial.println(ISC_firmVer);

  spLineSensorSetup();        //Set ISC registers for S. Pithouse Line Sensor Board

  while (funcSelect() != 99); //Button must be pressed to enable ISC

  iscEnable(boardType);    //Enable with chosen Sensor board attached

  while (funcSelect() == 99); //Button must be released to start main code

  iscSend8(LEDCTRL, 4);       //blink fast to show calibration mode

  while (funcSelect() != 99){   //calibrate until button is pushed
    calibrate();
  }

  while (funcSelect() == 99); //Button must be released to start main code

  iscSend8(LEDCTRL, 3);     //board control
  iscSend8(BRDCONFIG, 1);   //Side Markers

  delay(1000);
  
  if(funcSelect()==1)
     enableMotorControl = 1;     //Enable motorControl Functionto be called in timer interrupt
  digitalWrite(mainLED, enableMotorControl);
}

int midPoint = 999;
int leftThrsh = 350;
int rightThrsh = 350;

int error = 0;
int black = 0;

void loop()
{
  //Serial.println(lCount);
  
  getError();   //update myLineError value

  rInc += (myLineError)/120;
  //iscSend8(LEDCTRL, 5);
  //delay(1000);

  /*for(float a=-1; a<1; a+=0.1){
    motor(a, a);
    delay(200);
  }*/
  //Serial.print(myLineError);
  //Serial.print(", ");
  //Serial.print(rInc);
  //Serial.print(", ");
  //Serial.print(lCount);
  //Serial.print(", ");
  //Serial.println(rCount);

  delay(1);



  //Serial.println(iscGet(SCANTIME, 2));
  /*delay(5);
  Serial.print(myLineError);
  Serial.print(", ");
  Serial.print(maxVal);
  Serial.print(", ");
  Serial.print(iscGet(LEFTVAL, 2));
  Serial.print(", ");
   Serial.print(iscGet(MIDVAL, 2));
  Serial.print(", ");
  Serial.print(iscGet(RIGHTVAL, 2));
  Serial.print(", ");
   Serial.println(black);
   */
  //Serial.print(", ");

  //myLineError = 720 - iscGet(MIDVAL, 2);
  //if(iscGet(LEFTVAL, 2)>iscGet(RIGHTVAL, 2)) myLineError *= -1;

//Serial.println(myLineError);
/*
  //////HANDLE ISC INTERRUPT/////

  if (digitalRead(iscInterrupt))              //Interrupt received from ISC
  {
    int intFlags = iscGet(INTERRUPTFLAGS, 1); //Store Interrupt Flags
    int intAck = 0;                           //Flags to acknowledge at the end of this routine
    int brdAck = 0;                           //Board-specific flags to acknowledge at the end of this routine

    /////////////////Sensor State Interrupt occurred/////////////////

    if (intFlags & 0b11111)
    {
      int sensState = iscGet(SENSSTATE, 1); //Store sensor flags

      if (intFlags & 1 << 1) //Sensor 1 has changed
      {
        if (sensState & 1 << 1) //Sensor 1 Rising edge
        {
          Serial.println("Sensor 1 RISING Edge");
        }
        else //Sensor 1 Falling edge
        {
          Serial.println("Sensor 1 FALLING Edge");
        }
        intAck |= 1 << 1; //Sensor 1 Acknowledge
      }
      if (intFlags & 1 << 3) //Sensor 3 has changed
      {
        if (sensState & 1 << 3) //Sensor 3 Rising Edge
        {
          Serial.println("Sensor 3 RISING Edge");
        }
        else //Sensor 3 Falling Edge
        {
          Serial.println("Sensor 3 FALLING Edge");
        }
        intAck |= 1 << 3; //Sensor 3 Acknowledge
      }
    }

    /////////////////Board-specific Interrupt occurred/////////////////

    if (intFlags & 1 << 7)
    {
      int brdFlags = iscGet(BRDINTFLAGS, 1); //Store board-specific flags
      if (brdFlags & 1)                       //LOSTLINE Interrupt occurred
      {
        Serial.println("Lostline");
        brdAck |= 1;
      }
      if (brdFlags & 1 << 2) //CROSSOVER Interrupt occurred
      {
        Serial.println("Crossover");
        brdAck |= 1 << 2;
      }
      iscSend8(BRDINTFLAGS, brdAck); //Acknowledge board-specific interrupts
    }
    iscSend8(INTERRUPTFLAGS, intAck); //Acknowledge interrupts
  }

  Serial.print(iscGet(SENS1VAL, 2));
  Serial.print(" ");
  Serial.print(iscGet(SENS2VAL, 2));
  Serial.print(" ");
  Serial.print(iscGet(SENS3VAL, 2));
  Serial.print(" ");
  Serial.println(iscGet(LINEERROR, 2));
  delay(10);
  */
}