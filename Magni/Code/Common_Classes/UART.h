/*
 * UART.h
 *
 *  Created on: Sep 25, 2022
 *      Author: Robin
 */

#ifndef UART_H_
#define UART_H_

#include "System.h"

#define RX 3
#define TX 4

class UART
{
public:
    UART();
    virtual ~UART();
    void init(System *sys, uint32_t portBase, uint32_t tx, uint32_t rx);
    void send(const char *transmit, uint8_t length);
    bool receive(char *receive, uint8_t length);

private:
    /*
     * The following array is needed to let the compiler know that the
     * precompiled class library needs space for its private variables.
     * Or in shorter terms: simply ignore it.
     */
    System *sys;
    uint32_t portBase, tx, rx, length;
    char transmit, receive;
    uint32_t spaceForLib[40];
};
#endif /* UART_H_ */
