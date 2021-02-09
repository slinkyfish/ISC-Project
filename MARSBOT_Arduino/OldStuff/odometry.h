#ifndef odometry
#define odometry

extern float lCount;
extern float rCount;

extern float rInc;

extern int enableMotorControl;

extern void lCounter();     //Left Wheel interrupt
extern void rCounter();     //Right Wheel interrupt

extern void timerInterrupt();   //1kHz timer interrupt

extern void motor(float a, float b);
extern void motorControl();
#endif