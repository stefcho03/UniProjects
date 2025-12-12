/*
 * Steering.h
 *
 *  Created on: 27.11.2021
 *      Author: Robin
 */

/*
 * System.h:    Access to the System Class
 * ADC.h:       Access to the ADC Module
 */

#ifndef STEERING_H_
#define STEERING_H_

#include "System.h"
#include "ADC.h"

class Steering
{
public:
    Steering();
    virtual ~Steering();
    void init(System *sys, uint32_t baseX, uint32_t baseY, uint32_t sampleSeqX, uint32_t sampleSeqY, uint32_t analogInX, uint32_t analogInY);
    void calcValue();
    float getRightSpeed();
    float getLeftSpeed();
private:

};



#endif /* STEERING_H_ */
