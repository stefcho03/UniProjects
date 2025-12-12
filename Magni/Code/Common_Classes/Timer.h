/*
 * Timer.h
 *
 *    Author: Max Zuidberg
 *     Email: m.zuidberg@icloud.com
 */

#ifndef TIMER_H_
#define TIMER_H_


/*
 * stdbool.h:               Boolean definitions for the C99 standard
 * stdint.h:                Variable definitions for the C99 standard
 * inc/hw_types.h:          Macros for hardware access, both direct and via the
 *                          bit-band region.
 * inc/hw_memmap.h:         Macros defining the memory map of the Tiva C Series
 *                          device. This includes defines such as peripheral
 *                          base address locations such as GPIO_PORTF_BASE.
 * inc/hw_ints.h:           Macros defining the interrupt assignments
 * driverlib/sysctl.h:      Defines and macros for the System Control API of
 *                          DriverLib. This includes API functions such as
 *                          SysCtlClockSet.
 * driverlib/interrupt.h:   Defines and macros for NVIC Controller (Interrupt)
 *                          API of driverLib. This includes API functions such
 *                          as IntEnable and IntPrioritySet.
 * driverlib/timer.h:       Defines and macros for Timer API of driverLib.
 *                          This includes API functions such as TimerConfigure.
 * System.h:                Access to current CPU clock and other functions.
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "System.h"
#include "Config.h"


class Timer
{
public:
    Timer();
    virtual ~Timer();
    void init(System* sys, uint32_t base, void (*ISR)(void), uint32_t freq = 0);
    void start();
    void stop();
    void clearInterruptFlag();
    void setPeriodUS(uint32_t periodUS);
    void setFreq(uint32_t frequency);
    uint32_t getFreq();
    uint32_t getPeriodUS();

private:
    uint32_t base;
    uint32_t loadValue, prescale, periodUS = 0, freq = 0;
    uint_fast8_t number;
    System* sys;
    const uint32_t SYSCTL_MAPPING[6] = {SYSCTL_PERIPH_TIMER0,
                                        SYSCTL_PERIPH_TIMER1,
                                        SYSCTL_PERIPH_TIMER2,
                                        SYSCTL_PERIPH_TIMER3,
                                        SYSCTL_PERIPH_TIMER4,
                                        SYSCTL_PERIPH_TIMER5};
};

#endif /* TIMER_H_ */
