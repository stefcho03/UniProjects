#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/fpu.h"
#include "driverlib/timer.h"
#include <Digitaler_Filter.h>
// Praeprozessor-Makros
#define SAMPLERATE 44000
#define FILTERORDER 50
// Funktionen-Deklarationen
void adcIntHandler(void);
void setup(void);
// global variables
int32_t bufferSample[FILTERORDER]; //Ringbuffer
int32_t sampleIndex = 0;
float Result; //Energy after the filtering
int i;
void main(void) // nicht veraendern!! Bitte Code in adcIntHandler
{
setup();
while(1){}
}
void setup(void){// konfiguriert den Mikrocontroller
// konfiguriere System-Clock
SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
uint32_t period = SysCtlClockGet()/SAMPLERATE;
// aktiviere Peripherie
SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
// aktiviere Gleitkommazahlen-Modul
FPUEnable();
FPUStackingEnable();
FPULazyStackingEnable();
FPUFlushToZeroModeSet(FPU_FLUSH_TO_ZERO_EN);
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
void adcIntHandler(void){
//Reading the input
uint32_t adcInputValue;
ADCSequenceDataGet(ADC0_BASE,3,&adcInputValue);
//Squaring the input for better values
bufferSample[sampleIndex]=adcInputValue*adcInputValue;
if(sampleIndex==FILTERORDER-1){
Result = 0;
//filtering
for(i=0;i<BL;i++){
Result = Result + bufferSample[i]*B[i];
}
}
sampleIndex++;
if(sampleIndex==FILTERORDER){
sampleIndex=0;
}
//turn all of the LEDs off to ensure proper function when lowering the volume
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0,0x00);
//turn however many LEDs we need based on the ouput
if(Result>=1452000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_7,0xff);
}
if(Result>=1443000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_6,0xff);
}
if(Result>=1430000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_5,0xff);
}
if(Result>=1424000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_4,0xff);
}
if(Result>=1418000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_3,0xff);
}
if(Result>=1408000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_2,0xff);
}
if(Result>=1398000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1,0xff);
}
if(Result>= 1385000){
GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_0,0xff);
}
// am Ende von adcIntHandler, Interrupt-Flag loeschen
ADCIntClear(ADC0_BASE,3);
}