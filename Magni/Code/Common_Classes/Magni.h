/*
 * Magni.h
 *
 *  Created on: 20.11.2021
 *      Author: Robin
 */

#ifndef MAGNI_H_
#define MAGNI_H_

#include "System.h"
#include "PWM.h"
#include "GPIO.h"
#include "Config.h"
#include "ErrorCodes.h"
#include "Compare.h"
#include "ADC.h"
#include "Timer.h"
#include "Distance.h"
#include "UART.h"

#define COMBINED        0
#define PWMLEFT         1
#define PWMRIGHT        2
#define RPMLEFT         3
#define RPMRIGHT        4
#define CURRENTLEFT     5
#define CURRENTRIGHT    6
#define ERROR           7
#define SPEED           8
#define BATTERY         9
#define BATTERYCOMMA    10
#define SENSORDIR       11
#define DISTANCE        12


class Magni
{
public:
    Magni();
    virtual ~Magni();
    void init(System *sys, void (*rpmLeftISR)(void), void (*rpmRightISR)(void), void (*sensor1ISR)(void), void (*sensor2ISR)(void), void (*sensor3ISR)(void), void (*sensor4ISR)(void), void (*sensor5ISR)(void));
    void update(void (*sensor1ISR)(void), void (*sensor2ISR)(void), void (*sensor3ISR)(void), void (*sensor4ISR)(void), void (*sensor5ISR)(void));
    uint8_t bool2Byte(bool *values);
    void sensorISR1();
    void background();
    void getTransmitWords();
    void rpmISR1();
    void rpmISR2();
    Compare rpmLeft, rpmRight;
    Distance sensor1, sensor2, sensor3, sensor4, sensor5;
private:
    System *magniSys;
    UART communication;
    PWM motors;
    GPIO enable, dirLeft, dirRight, brakeLeft, brakeRight, faultLeft, faultRight, faultCLR;
    GPIO onboard,onboard2;
    ADC motorLeftCurrent, motorRightCurrent, battery;
    char receiveWords[3] = {0x00, 0x00, 0x00};
    bool standby = false;
    bool updateFlag = false;
    bool measureStarted = false;;
    bool enableStatus = false;
    bool status = false;
    char transmitWords[13] = "ABCDEFGHIJ";
    float dutyLeft, dutyRight, minimal_distance = 255, speed = 1.0f;
    uint8_t joyStickVert = 0, joyStickHori = 0, sensor_dir = 0, batteryCounter = 0, uartCounter = 0, timeOut = 0,distanceMid = 0;
    uint8_t lastDistances[5] = {255,255,255,255,255};
    uint8_t distanceCounter = 0;
    uint8_t sensor = 1;
};



#endif /* MAGNI_H_ */
