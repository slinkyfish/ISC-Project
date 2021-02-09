#ifndef ISC
#define ISC

#define iscAddress 0x5



///////I2C Register Addresses////////
#define FIRMVER 0
#define CONTROL 1
#define INTERRUPTCTRL 2
#define INTERRUPTFLAGS 3
#define SENSSTATE 4
#define LEDCTRL 5
#define PULSEDUR 6
#define SENS0SETUP 7
#define SENS1SETUP 8
#define SENS2SETUP 9
#define SENS3SETUP 10
#define SENS4SETUP 11
#define SENS0VAL 13
#define SENS1VAL 15
#define SENS2VAL 17
#define SENS3VAL 19
#define SENS4VAL 21
#define SENS0THRSH 25
#define SENS1THRSH 27
#define SENS2THRSH 29
#define SENS3THRSH 31
#define SENS4THRSH 33
#define SCANTIME 37
#define BRDCONFIG 39
#define BRDINTCTRL 40
#define BRDINTFLAGS 41
#define LINEERROR 42

#define LEFTVAL SENS3VAL
#define MIDVAL SENS2VAL
#define RIGHTVAL SENS1VAL

#define LEFTSETUP SENS3SETUP
#define MIDSETUP SENS2SETUP
#define RIGHTSETUP SENS1SETUP

#define BASIC_LINE_SENSOR 1
#define BASIC_WALL_SENSOR 2
#define SP_LINE_SENSOR 3

extern int ISC_firmVer;
extern void iscInit();
extern void basicLineSensorSetup();
extern void basicWallSensorSetup();
extern void spLineSensorSetup();
extern void iscEnable(uint8_t boardType);
extern void iscSend8(uint8_t regAddr, uint8_t data);
extern void iscSend16(uint8_t regAddr, uint16_t data);
extern uint16_t iscGet(uint8_t regAddr, uint8_t numRegisters);
extern void getError();

extern int maxVal;
extern int minVal;
extern float myLineError;
extern int leftVal;
extern int midVal;
extern int rightVal;

#endif