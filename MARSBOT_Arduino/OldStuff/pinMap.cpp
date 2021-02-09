#include <Arduino.h>
#include "pinMap.h"

//NOTE PWM will not function on Pin11 (Timer 2 is used internally for interrupt)

int lInterruptPin = 2;
int lEncoder = 4;

int rInterruptPin = 3;
int rEncoder = 5;

int iscInterrupt = A2;

int lMotorPower = 9;
int lMotorDir = 7;

int rMotorPower = 10;
int rMotorDir = 8;

int mainLED = 13;

int functionSelect = A6;

void setupPinModes()
{
    pinMode(lMotorDir, OUTPUT);
    pinMode(lMotorPower, OUTPUT);
    pinMode(rMotorDir, OUTPUT);
    pinMode(rMotorPower, OUTPUT);

    pinMode(lInterruptPin, INPUT);
    pinMode(lEncoder, INPUT);
    pinMode(rInterruptPin, INPUT);
    pinMode(rEncoder, INPUT);

    pinMode(functionSelect, INPUT);

    pinMode(mainLED, OUTPUT);
}