#include <arduino.h>
#include "odometry.h"
#include "pinMap.h"

float lCount = 0; //Pulses that Left Wheel has turned
float rCount = 0; //Pulses that Right Wheel has turned

float lError = 0;
float rError = 0;
float lDistance = 0;
float rDistance = 0;
float lMotor = 0;
float rMotor = 0;
float kp = 0.001;      //0.0004 all values when inc = 300
float kd = 0;           //0
float ki = 0.000;      //0.0005
float prevLError = 0;
float prevRError = 0;

float lineError = 0;

int enableMotorControl = 0;

void lCounter()
{
    static int8_t prevLpattern;
    int8_t lPattern = (!digitalRead(lInterruptPin)) | (!digitalRead(lEncoder)<<1);

    if(lPattern-prevLpattern==1) lCount+=100;
    if(lPattern-prevLpattern==3) lCount-=100;
    if(lPattern-prevLpattern==-1) lCount-=100;
    if(lPattern-prevLpattern==-3) lCount+=100;
    
    prevLpattern = lPattern;
}

void rCounter()
{
    static int8_t prevRpattern;
    int8_t rPattern = digitalRead(rInterruptPin) | (digitalRead(rEncoder)<<1);

    if(rPattern-prevRpattern==1) rCount+=100;
    if(rPattern-prevRpattern==3) rCount-=100;
    if(rPattern-prevRpattern==-1) rCount-=100;
    if(rPattern-prevRpattern==-3) rCount+=100;

    prevRpattern = rPattern;
}

void timerInterrupt()
{
    TCCR2A = 0b10;  //CTC mode
    TCCR2B = 0b100; //Timer 2 Prescalar of 64 (16M/64 = 250kHz)
    OCR2A = 250;    //Compare value for channel A (from above, value of 250 means 1kHz)
    TIMSK2 = 0b11;  //Interrupt on Channel A, and enable interrupts
}

ISR(TIMER2_COMPA_vect)
{                    //Interrupt Service Routine runs each interrupt
  if(enableMotorControl) motorControl();
}

void motor(float a, float b)
{
    if(a>0) digitalWrite(lMotorDir, 1);
    else digitalWrite(lMotorDir, 0);

    if(b>0) digitalWrite(rMotorDir, 0);
    else digitalWrite(rMotorDir, 1);

    analogWrite(lMotorPower, abs(a)*254);

    analogWrite(rMotorPower, abs(b)*254);

}

float debug = 1;
float diffVal = 0;

int index = 0;

float lHistory[10];
float rHistory[10];

void motorControl()
{
    float lSum = 0;
    float rSum = 0;

    lDistance += 300 - error;     //Fwd velocity: 70/100 pulse / interrupt 
    rDistance += 300 + error;
    
    prevLError = lError;
    prevRError = rError;

    lError = lDistance-lCount;
    rError = rDistance-rCount;

    lHistory[index] = lError;
    rHistory[index] = rError;

    if(index>9) index = 9;
    lSum = 0;
    rSum = 0;

    /*for(int c=0; c<10; ++c){
        lSum += lHistory[c];
        rSum += rHistory[c];
    }*/
    diffVal = (lSum)*kd;

    lMotor = 0.5 + ((lError * kp))+ (((lError*10)-lSum)*kd);// + (prevLError)*ki + ((lError-prevLError)*kd);
    rMotor = 0.5 + ((rError * kp)) + (((rError*10)-rSum)*kd);// + (prevRError)*ki + ((rError-prevRError)*kd);

    //lMotor = 0.4;
    //rMotor = 0.4;

    //lMotor += error;
    //rMotor -= error;

    if(lMotor > 1) lMotor = 1;
    if(lMotor < -1) lMotor = -1;

    if(rMotor > 1) rMotor = 1;
    if(rMotor < -1) rMotor = -1;

    motor(lMotor, rMotor);



}