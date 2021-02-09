#include <Arduino.h>
#include "pinMap.h"
#include "odometry.h"
#include "functions.h"

/* debug...
 * 1) Print function select value
 * 2) Print encoder values
 */

void debug(char dbgType)
{
    switch (dbgType)
    {
    case 1:                                 //Print function value and analogRead value
        Serial.print("functionSelect:");
        Serial.print(funcSelect(), DEC);
        Serial.print(", analogRead:");
        Serial.println(analogRead(functionSelect));
        break;
    case 2:                                 //Print encoder values
        Serial.print("lVal:");
        Serial.print(lCount);
        Serial.print(", rVal:");
        Serial.println(rCount);
        break;
    }
    delay(100);
}

char funcSelect() //Checked, works: 04/01/2021
{
    int funcInput = analogRead(functionSelect);
    char funcVal;
    if (funcInput > 839)
        funcVal = 99;
    else if (funcInput > 657)
        funcVal = 0;
    else if (funcInput > 638)
        funcVal = 1;
    else if (funcInput > 620)
        funcVal = 2;
    else if (funcInput > 603)
        funcVal = 3;
    else if (funcInput > 581)
        funcVal = 4;
    else if (funcInput > 559)
        funcVal = 5;
    else if (funcInput > 535)
        funcVal = 6;
    else if (funcInput > 495)
        funcVal = 7;
    else if (funcInput > 450)
        funcVal = 8;
    else if (funcInput > 413)
        funcVal = 9;
    else if (funcInput > 371)
        funcVal = 10;
    else if (funcInput > 313)
        funcVal = 11;
    else if (funcInput > 245)
        funcVal = 12;
    else if (funcInput > 173)
        funcVal = 13;
    else if (funcInput > 87)
        funcVal = 14;
    else
        funcVal = 15;
    return (funcVal); //Returns 99 if button is pressed
}