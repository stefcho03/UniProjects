/*
 * UART.cpp
 *
 *      Author: Robin Rau
 *      Email: robin.rau@student.kit.edu
 */


// #define USE_UART_LIBRARY

#ifndef USE_UART_LIBRARY

#include "UART.h"

UART::UART()
{
    // Empty Constructor
}

UART::~UART()
{
    // Empty Destructor
}
void UART::init(System *sys, uint32_t portBase, uint32_t tx, uint32_t rx){
    /*
         * Initializes a GPIO-pin object by enabling the corresponding peripheral
         * (the port which contains the pin), and configuring the pin as input or
         * output.
         *
         * sys:      Pointer to the current System instance. Needed for error
         *           handling.
         * portBase: base address of the UART port. Use the constants provided by
         *           the TivaWare API (inc/hw_memmap.h line 53-56, 73-76).
         * tx:       address of the UART pin, used to transmit data. Use the constants provided by
         *           the TivaWare API (Config.h line 150).
         * rx:       address of the UART pin, used to receive data. Use the constants provided by
         *           the TivaWare API (Config.h line 151).
         */

        // Save system, port and transmitter and receiver pins
        this->sys = sys;
        this->portBase = portBase;
        this->tx = tx;
        this->rx = rx;


        // The default current and pin type (also see Config.h line 150-152)
        tx = CFG_UART_TX;
        rx = CFG_UART_RX;
        portBase = CFG_UART_PORT;

        /*
         * Enable the peripheral (here: a GPIO port). This can be redone for other
         * pins in the same port, as only one bit is set in a register. Therefore
         * no need to check whether the bit is already set (simpler and faster
         * without unnecessary check).
         *
         * Note: In this class sometimes a mapping of values is needed. For example
         *       in this case the base address of the port must be mapped to the
         *       correct system control constant. This can be done in different
         *       ways. For educational purposes, this class uses three different
         *       methods: switch case, if else, array. More details should be given
         *       in the student's manual.
         */
//        switch (portBase)
//        {
//        case GPIO_PORTA_BASE:
//            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//            break;
//        case GPIO_PORTB_BASE:
//            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//            break;
//        case GPIO_PORTC_BASE:
//            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
//            break;
//        case GPIO_PORTD_BASE:
//            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
//            break;
//        case GPIO_PORTE_BASE:
//            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
//            break;
//        case GPIO_PORTF_BASE:
//            SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//            break;
//        default:
//            sys->error(GPIOWrongConfig, &portBase, &pin, &dir);
//        }
//
//        // Wait until peripheral is enabled "TivaWare(TM) Treiberbibliothek"
//        // page 502, "TivaC Launchpad Workshop" page 78)
//        sys->delayCycles(5);
//
//        // Configure pin as input or output.
//        GPIODirModeSet(portBase, pin, dir);
//
//        // Apply pull-up parameter. This also calls GPIOPadConfigSet needed by
//        // GPIODirModeSet (see "TivaWare(TM) Treiberbibliothek" page 253).
//        setPullup(pullup);

}
void UART::send(const char *transmit, uint8_t length){
        this->transmit = transmit;
        this->length = length;
        length = 11; // 1 start, 1 stop, 1 parity and 8 normal
        //UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
bool UART::receive(char *receive, uint8_t length){
        this->receive = receive;
        this->length = length;

}
#endif
