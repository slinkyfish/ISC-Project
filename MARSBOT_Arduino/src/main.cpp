#include <Arduino.h>
#include <isc.h>
#include "pinMap.h"
#include "odometry.h"
#include "functions.h"

ISC Isc;

const uint8_t cornerSensor = 4;   //Give Sensor inputs meaningful names
const uint8_t lSensor = 3;
const uint8_t mSensor = 2;
const uint8_t rSensor = 1;
const uint8_t ssSensor = 0;

float midVal = 0;
float minVal = 0;
float maxVal = 0;
float leftVal = 0;
float rightVal = 0;
float minDiff = 1023;

void calibrate(){                   //Measure and store the Middle sensor minimum and maximum values

  leftVal = Isc.sensor(lSensor);
  rightVal = Isc.sensor(rSensor);

  float difference = abs(leftVal - rightVal);

  if (difference < minDiff && (leftVal+rightVal)/2 > 400){
    minDiff = difference;
    midVal = (leftVal+rightVal)/2;
  }
  
}

void calibrate1(){                   //Measure and store the Middle sensor minimum and maximum values
  midVal = Isc.sensor(mSensor);

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

  Isc.reset(0x5);                          //Establish communication and Reset ISC (Address 0x05)

  Isc.setup(ssSensor, enb+txEnb);          //Setup SS Sensor, enabled with Tx enabled
  Isc.setThreshold(ssSensor, 300);         //Set Start / Stop sensor threshold to 300

  Isc.setup(lSensor, enb+txEnb);          //Setup Left Sensor, enabled with Tx enabled
  Isc.setup(mSensor, enb+txEnb);          //Setup Middle Sensor, enabled with Tx enabled
  Isc.setup(rSensor, enb+txEnb);          //Setup Right Sensor, enabled with Tx enabled

  Isc.setup(cornerSensor, enb+txEnb);     //Setup Corner Sensor, enabled with Tx enabled
  Isc.setThreshold(cornerSensor, 300);    //Set Corner sensor threshold to 300

  Isc.txPulse(10);                         //10us Tx on time before sampling begins

  delay(2000);



  while (funcSelect() != 99); //Button must be pressed to enable ISC

  Isc.begin(spLineSensor);
  
  if(analogRead(7)<400){
    Serial.println(analogRead(7));
    Isc.ledMode(SLOWBLINK);       //blink slow to show low battery mode
    while(1);
  }
    
  while (funcSelect() == 99); //Button must be released to start main code

  Isc.ledMode(FASTBLINK);       //blink fast to show calibration mode

  while (funcSelect() != 99){   //calibrate until button is pushed
    calibrate();
  }

  while (funcSelect() == 99); //Button must be released to start main code

  Isc.ledMode(LEDBRDCTRL);                 //Start LED indication
  Isc.write(39, 1, 1);

  delay(1000);
  
  if(funcSelect()==1)
     enableMotorControl = 1;     //Enable motorControl Function to be called in timer interrupt
  digitalWrite(mainLED, enableMotorControl);
}

int midPoint = 999;
int leftThrsh = 350;
int rightThrsh = 350;

float error = 0;
int black = 0;
float myError = 0;
float array[200];

void loop1(){
  //Serial.print(lCount);
  //Serial.print(", ");
  //Serial.println(rCount);
  //delay(100);
  //motor(0.5, 0.5);
  /*for(int a=0; a<200; ++a){
    array[a] = lMotor;
    delay(1);
  }
  while(1){
    for (int a = 0; a < 200; ++a)
    {
      Serial.println(array[a]);
      delay(1);
    }
    delay(3000);
  }
  */
}

float prevLineError = 0;
int a = 0;

void loop()
{
  if(Isc.sensor(lSensor)>270 && Isc.sensor(rSensor)>270){   //300 is level which sensors can see a bit of line (not full black)
  if(Isc.sensor(lSensor)< Isc.sensor(rSensor)){
    myError = Isc.sensor(lSensor);
  }else{
    myError = (2*midVal)-Isc.sensor(rSensor);    //500 is intersection value of l and r sensors
  }
}else{
  if(myError<midVal){
    //if(myError>0) myError = myError-1;    //increase error
  }else{
    //if(myError<1023) myError = myError+1; //increase error
  }
}

  array[a] = lineError;
  a++;
  if(a>9) a = 0;
  float mySum = 0;
  for(int b=0; b<10; ++b){
    mySum += array[b];
  }

  prevLineError = lineError;

  lineError = (myError-midVal);
  
  error = ((lineError*0.28) + ((lineError-prevLineError)*0) + prevLineError*0);

  Serial.println(error);
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