/*
 * Remote.cpp
 *
 *  Created on: 23.02.2022
 *      Author: Robin
 */

#include "Remote.h"

Remote::Remote()
{

}

Remote::~Remote()
{

}

void Remote::init(System *sys)
{
    this->remoteSys = sys;

    // Initialize UART Communication
    communication.init(sys,CFG_UART_PORT, CFG_UART_TX, CFG_UART_RX);
    // Initialize the ESP Enable Pin
    esp.init(sys, CFG_ESP_E_PORT, CFG_ESP_E_PIN, CFG_ESP_E_DIR, CFG_ESP_E_PULLUP);

    // Initialize Switch
    sw1.init(sys, CFG_BTN1_PORT, CFG_BTN1_PIN, CFG_BTN1_DIR, CFG_BTN1_PULLUP);
    sw2.init(sys, CFG_BTN2_PORT, CFG_BTN2_PIN, CFG_BTN2_DIR, CFG_BTN2_PULLUP);
    sw3.init(sys, CFG_BTN3_PORT, CFG_BTN3_PIN, CFG_BTN3_DIR, CFG_BTN3_PULLUP);
    sw4.init(sys, CFG_BTN4_PORT, CFG_BTN4_PIN, CFG_BTN4_DIR, CFG_BTN4_PULLUP);

    wheel.init(sys, CFG_JOY_XAX_MOD, CFG_JOY_YAX_MOD, CFG_JOY_XAX_SSQ, CFG_JOY_YAX_SSQ, CFG_JOY_XAX_ANI,
               CFG_JOY_YAX_ANI);

    // Initialize Display
    display.init(sys);

    onboard.init(sys, GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_OUT, false);

    // Initialize MPU
    //sensor.init(sys, CFG_SENSOR_I2C_MODULE, CFG_SENSOR_ADRESSBIT, CFG_SENSOR_WHEEL_AXIS, CFG_SENSOR_HOR_AXIS);
    display.clearDisplay();
    this->menuNumber = 1;
}

void Remote::updateDisplay()
{
    if (this->received[7] == 1)
    {
        if (!errorThrow)
        {
            errorThrow = true;
            display.clearDisplay();
        }
        display.cursorPosition(6, 2);
        display.printText("ERROR 401");
        display.cursorPosition(4,3);
        display.printText("Battery Low!");
    }
    else if(this->received[7] == 2)
    {
        if (!errorThrow)
        {
            errorThrow = true;
            display.clearDisplay();
        }
        display.cursorPosition(6, 1);
        display.printText("ERROR 404");
        display.cursorPosition(1, 3);
        display.printText("Connection not found");
    }
    else
    {
        errorThrow = false;
        if (menuNumber == 1)
        {
            //display.clearDisplay();
            display.cursorPosition(6, 1);
            display.printText("Menuauswahl");
            display.cursorPosition(1, 2);
            display.printText(" Fahrmodus");
            display.cursorPosition(1, 3);
            display.printText(" DebugMenu");
            display.cursorPosition(1, 4);
            display.printText(" UART Menu");
            display.cursorPosition(1, this->cursorPos);
        }
        if (menuNumber == 2)
        {
            //display.clearDisplay();
            display.cursorPosition(7, 1);
            display.printText("Drive Mode");
            display.cursorPosition(1, 2);
            display.printText("Speed:");
            display.cursorPosition(8, 2);
            display.clearSegment(3);
            display.cursorPosition(8, 2);
            display.printNumber(this->received[8]);
            display.cursorPosition(14, 2);
            display.printText("EE:");
            display.cursorPosition(18, 2);
            display.clearSegment(1);
            display.cursorPosition(18, 2);
            display.printNumber(this->engineEnable);
            display.cursorPosition(1, 3);
            display.printText("Distance:");
            display.cursorPosition(11, 3);
            display.clearSegment(3);
            display.cursorPosition(11, 3);
            display.printNumber(this->received[12]);
            display.cursorPosition(15, 3);
            display.printText('m');
            display.cursorPosition(1, 4);
            display.printText("Battery:");
            display.cursorPosition(10, 4);
            display.clearSegment(5);
            display.cursorPosition(10, 4);
            display.printFloat(this->received[9] + 0.01f * this->received[10]);
            display.cursorPosition(15, 4);
            display.printText('V');
        }
        else if (menuNumber == 3)
        {
            display.clearDisplay();
            display.cursorPosition(7, 1);
            display.printText("Debug Mode");
            display.cursorPosition(1, 2);
            display.printText("PWM: ");
            display.printNumber(this->pwm);
        }
        else if (menuNumber == 4)
        {
            display.clearDisplay();
            /*
             uint8_t column = 1,row = 2;
             for(int i=0;i<12;i++)
             {
             display.cursorPosition(column, row);
             display.printNumber(this->received[i]);
             column += 5;
             if(column > 16)
             {
             column = 1;
             row += 1;
             }
             }
             */
            display.clearDisplay();
            display.cursorPosition(7, 1);
            display.printText("UART Menu");
            display.cursorPosition(1, 2);
            display.printNumber(this->received[0]);
            display.cursorPosition(6, 2);
            display.printNumber((int8_t) this->received[1]);
            display.cursorPosition(11, 2);
            display.printNumber((int8_t) this->received[2]);
            display.cursorPosition(16, 2);
            display.printNumber((uint8_t) this->received[3]);
            display.cursorPosition(1, 3);
            display.printNumber((uint8_t) this->received[4]);
            display.cursorPosition(6, 3);
            display.printNumber((int8_t) this->received[5]);
            display.cursorPosition(11, 3);
            display.printNumber((int8_t) this->received[6]);
            display.cursorPosition(16, 3);
            display.printNumber((uint8_t) this->received[7]);
            display.cursorPosition(1, 4);
            display.printNumber((uint8_t) this->received[8]);
            display.cursorPosition(5, 4);
            display.printFloat(this->received[9] + 0.01f * this->received[10]);
            display.cursorPosition(11, 4);
            display.printNumber((uint8_t) this->received[11]);
            display.cursorPosition(16, 4);
            display.printNumber((uint8_t) this->received[12]);
        }
    }

}
void Remote::update()
{
    // Just for current Joystick Mounting
    setTransmitValues();
    this->communication.send(this->transmit,3);
    bool status = this->communication.receive(this->received,13);
    onboard.write(status);
    if (menuNumber == 1)
    {
        if (!sw1.read())
        {
            this->remoteSys->delayUS(50000);
            while (!sw1.read());
            this->cursorPos--;
            if (this->cursorPos < 2)
                this->cursorPos = 2;
            display.cursorPosition(1, this->cursorPos);
            this->remoteSys->delayUS(50000);
        }
        if (!sw3.read())
        {
            this->remoteSys->delayUS(50000);
            while (!sw3.read());
            display.clearDisplay();
            this->menuNumber = this->cursorPos;
            this->remoteSys->delayUS(50000);
        }
        if (!sw4.read())
        {
            this->remoteSys->delayUS(50000);
            while (!sw4.read());
            this->cursorPos++;
            if (this->cursorPos > 4)
                this->cursorPos = 4;
            display.cursorPosition(1, this->cursorPos);
            this->remoteSys->delayUS(50000);
        }

    }
    if(menuNumber == 2)
    {
        /*
         * If the selected menu is the Driving Menu, pressing switch 3 changes the state of engine enable
         */
        if(!sw3.read())
        {
            this->remoteSys->delayUS(50000);
            while(!sw3.read());
            this->engineEnable = !this->engineEnable;
            this->remoteSys->delayUS(50000);
        }
    }
    if (menuNumber == 3)
    {
        /*
         * If the selected menu is the Debug Menu, pressing switch 1 increases the pwm value to be set
         * Pressing switch 4 decreases the pwm value
         */
        if (!sw1.read())
        {
            this->remoteSys->delayUS(50000);
            while (!sw1.read())
                ;
            this->pwm += 10;
            if (this->pwm > 100)
                this->pwm = 100;
            this->remoteSys->delayUS(50000);
        }
        if (!sw4.read())
        {
            this->remoteSys->delayUS(50000);
            while (!sw4.read())
                ;
            this->pwm -= 10;
            if (this->pwm < -100)
                this->pwm = -100;
            this->remoteSys->delayUS(50000);
        }
    }
    // Press Switch 2 to get back to main menu
    if(!sw2.read())
    {
        this->remoteSys->delayUS(50000);
        while(!sw2.read());
        this->menuNumber = 1;
        display.clearDisplay();
        this->remoteSys->delayUS(50000);
    }

}
void Remote::setTransmitValues()
{
    this->transmit[0] |= (this->engineEnable << 7);
    wheel.calcValue();
    this->remoteSys->setDebugVal("LeftDuty", this->wheel.getLeftSpeed() * 100);
    this->remoteSys->setDebugVal("RightDuty", this->wheel.getRightSpeed() * 100);
    if(wheel.getLeftSpeed() >= 0.0f)
    {
        this->transmit[0] |= (1 << 5);
        this->transmit[1] = wheel.getLeftSpeed() * 100;
    }
    else
    {
        this->transmit[0] &= 0b11011111;
        this->transmit[1] = -wheel.getLeftSpeed() * 100;
    }

    if(wheel.getRightSpeed() >= 0.0f)
    {
        this->transmit[0] |= (1 << 6);
        this->transmit[2] = wheel.getRightSpeed() * 100;
    }
    else
    {
        this->transmit[0] &= 0b10111111;
        this->transmit[2] = -wheel.getRightSpeed() * 100;
    }
    if(this->received[7] == 1)
    {
        this->transmit[0] = (this->engineEnable << 7);
        this->transmit[1] = 0;
        this->transmit[2] = 0;
    }
}
