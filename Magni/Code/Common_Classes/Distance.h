/*
 * Distance.h
 *
 *      Author: Robin Rau
 *      Email: robin.rau@student.kit.edu
 */

/*
 * System.h:        Access to the System class
 * GPIO.h:          Access the GPIO Port/Pins
 */

#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "System.h"
#include "GPIO.h"

class Distance
{
public:
    Distance();
    virtual ~Distance();
    void init(System *sys, uint32_t echoPort, uint32_t echoPin, uint32_t triggerPort, uint32_t triggerPin, void (*ISR)(void));
    void trigger();
    void edgeDetect();
    void calcDistance();
    float getDistance();
    bool isMeasuring();
private:
    /*
     * The following array is needed to let the compiler know that the
     * precompiled class library needs space for its private variables.
     * Or in shorter terms: simply ignore it.
     */
    uint32_t spaceForLib[60];
};




#endif /* DISTANCE_H_ */
