#ifndef isc_header
#define isc_header

#include "Arduino.h"

#define enb 1
#define lowRes 2
#define txEnb 4
#define invert 32
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
        int sensor(uint8_t sensor);
        void ledMode(uint8_t mode);
        void setup(uint8_t sensor, uint8_t bits);
        void txPulse(uint8_t length);
        uint8_t states();
};

#endif