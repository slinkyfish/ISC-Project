#include <arduino.h>
#include "odometry.h"
#include "pinMap.h"

float lCount = 0; //Pulses that Left Wheel has turned
float rCount = 0; //Pulses that Right Wheel has turned

float lError = 0;
float rError = 0;
float Distance = 100;
float lMotor = 0;
float rMotor = 0;
float kp = 40;      //50
float prevLError = 0;
float prevRError = 0;

float rInc = 0;

int enableMotorControl = 0;

void lCounter()
{
    static int8_t prevLpattern;
    int8_t lPattern = (!digitalRead(lInterruptPin)) | (!digitalRead(lEncoder)<<1);

    if(lPattern-prevLpattern==1) lCount++;
    if(lPattern-prevLpattern==3) lCount--;
    if(lPattern-prevLpattern==-1) lCount--;
    if(lPattern-prevLpattern==-3) lCount++;
    
    prevLpattern = lPattern;
}

void rCounter()
{
    static int8_t prevRpattern;
    int8_t rPattern = digitalRead(rInterruptPin) | (digitalRead(rEncoder)<<1);

    if(rPattern-prevRpattern==1) rCount++;
    if(rPattern-prevRpattern==3) rCount--;
    if(rPattern-prevRpattern==-1) rCount--;
    if(rPattern-prevRpattern==-3) rCount++;

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

    if(b>0) digitalWrite(rMotorDir, 1);
    else digitalWrite(rMotorDir, 0);

    analogWrite(lMotorPower, abs(a)*254);

    analogWrite(rMotorPower, abs(b)*254);

}

void motorControl()
{
    Distance += 5;
    lError = (Distance-rInc-lCount);
    rError = (Distance+rInc-rCount);

    lMotor = (lError / kp);// + ((lError-prevLError)/kd);
    rMotor = (rError / kp);// + ((rError-prevRError)/kd);

    if(lMotor > 1) lMotor = 1;
    if(lMotor < -1) lMotor = -1;

    if(rMotor > 1) rMotor = 1;
    if(rMotor < -1) rMotor = -1;

    motor(lMotor, rMotor);

    prevLError = lError;
    prevRError = rError;

}