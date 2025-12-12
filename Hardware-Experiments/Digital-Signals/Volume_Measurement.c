/*
* Aufgabe_11.c
*
* Created on: 4.05.2023 ã.
* Author: HP
*/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
// Praeprozessor-Makros
#define BUFFER_SIZE 1000
#define SAMPLERATE 44000
// Funktionen-Deklarationen
void adcIntHandler(void);
void setup(void);
// globale Variablen
uint32_t Values[BUFFER_SIZE]; //Ringbuffer
uint32_t Squared; //variable that stores the squared input
int i = 0;
void main(void){ // nicht veraendern!! Bitte Code in adcIntHandler 
setup();
while(1){}
}
void setup(void){// konfiguriert den MiKrocontroller
// konfiguriere System-Clock
SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
uint32_t period = SysCtlClockGet()/SAMPLERATE;
// aktiviere Peripherie
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
// konfiguriere GPIO
GPIOPinTypeADC(GPIO_PORTE_BASE,GPIO_PIN_2);
GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
// konfiguriere Timer
TimerConfigure(TIMER0_BASE,TIMER_CFG_PERIODIC);
TimerLoadSet(TIMER0_BASE, TIMER_A, period - 1);
TimerControlTrigger(TIMER0_BASE,TIMER_A,true);
TimerEnable(TIMER0_BASE,TIMER_A);
// konfiguriere ADC
ADCClockConfigSet(ADC0_BASE,ADC_CLOCK_RATE_FULL,1);
ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_TIMER, 0);
ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END);
ADCSequenceEnable(ADC0_BASE, 3);
ADCIntClear(ADC0_BASE,3);
ADCIntRegister(ADC0_BASE,3,adcIntHandler);
ADCIntEnable(ADC0_BASE,3);
}
void adcIntHandler (void){
uint32_t adcInputValue;
ADCSequenceDataGet(ADC0_BASE,3,&adcInputValue);
int j;
uint32_t Energy = 0;
Squared = adcInputValue*adcInputValue;
Values[i] = Squared; //Storing the squared values in the ringbuffer
i++;
//reset the counter if the array is full
if (i==BUFFER_SIZE){
i = 0;
};
//calculating the whole energy
for (j=0;j<BUFFER_SIZE;j++){
Energy = (Energy + Values[j]);
}
Energy = Energy/1000;
//turn all of the LEDs off to ensure proper function when lowering the 
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0,0x00);
//turn however many LEDs we need based on the ouput
if(Energy>=800000){ // Volume: 88-100
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_7,0xff);
}
if(Energy>=400000){ // Volume: 72-87
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,0xff);
}
if(Energy>=200000){ // Volume: 64-71
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0xff);
}
if(Energy>=150000){ // Volume: 52-70
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0xff);
}
if(Energy>=80500){ // Volume: 43-52
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,0xff);
}
if(Energy>= 20000){ // Volume: 33-42
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0xff);
}
if(Energy>= 5000){ // Volume: 22-33
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,0xff);
}
if(Energy>= 500){ // Volume: 12-22
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0,0xff);
}
// am Ende von adcIntHandler, Interrupt-Flag loeschen
ADCIntClear(ADC0_BASE,3);
};