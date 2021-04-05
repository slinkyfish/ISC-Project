#ifndef isc_header
#define isc_header

#include "Arduino.h"

#define enb 1
#define lowRes 2
#define txEnb 4
#define flip 32
#define fallingInterrupt 64
#define risingInterrupt 128

#define basicLineSensor 1
#define basicWallSensor 2
#define spLineSensor 3

#define MASTERCTRL 0
#define FREQOUT 1
#define LEDOFF 2
#define LEDBRDCTRL 3
#define FASTBLINK 4
#define SLOWBLINK 5

#define SENSOR0 0
#define SENSOR1 1
#define SENSOR2 2
#define SENSOR3 3
#define SENSOR4 4

#define SENSOR0INTERRUPT 1
#define SENSOR1INTERRUPT 2
#define SENSOR2INTERRUPT 4
#define SENSOR3INTERRUPT 8
#define SENSOR4INTERRUPT 16

class ISC
{
    private:
        int _address;
    public:
        int read(uint8_t regAddr, uint16_t numVals);
        void write(uint8_t regAddr, uint16_t data, uint8_t numVals);
        void reset(uint8_t address);
        void begin(uint8_t boardType);
        int scanTime();
        void setThreshold(uint8_t sens, uint16_t threshVal);
        uint16_t getVal(uint8_t sensor);
        void ledMode(uint8_t mode);
		void leftLED(uint8_t state);
		void rightLED(uint8_t state);
        void configure(uint8_t sensor, uint8_t bits);
        void txPulse(uint8_t length);
        uint8_t states();
		void setInterrupts(uint8_t interrupts);
		uint8_t getInterrupts();
		void clearInterrupts(uint8_t interrupts);
};

#endif