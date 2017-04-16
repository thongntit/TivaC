/*
 * Bài hướng dẫn TivaC của CLB Điện- Điện Tử EClub PTITHCM
 * Viết bởi Nanolove95 góp ý vui lòng gửi về nanolove95@gmail.com Thanks!!!
 */

/*
 *  Include thư viện
 */
#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

/*
 *  Define thư viện
 */
#define LED_RED 	GPIO_PIN_1
#define LED_BLUE	GPIO_PIN_2
#define LED_GREEN	GPIO_PIN_3

/*
 * Khai báo biến
 */
unsigned int DutyCycle=10;//Duty cycle of PWM

void PWMADJ(void);
unsigned int thoigian1xung;
int main(void) {
	/*
	 * Thiết lập tốc độ Clock cho hệ thống.
	 * SYSCTL_XTAL_16MHZ = sử dụng thạch anh ngoài 16MHZ
	 * SYSCTL_OSC_MAIN	 = sử dụng bộ Osc chính
	 * SYSCTL_USE_PLL	 = sử dụng bộ PLL (tốc độ 400MHz/2= 200MHz )
	 * SYSCTL_SYSDIV_5	 = độ chia bộ PLL (ở đây là 200/5 = 40MHz)
	 */
	SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_SYSDIV_5 ); // Thiết lập tốc độ cho MCU chạy ở 40MHz
	/*
	 * Thiết lập GPIO
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //	set chức năng I/O cho PORTF
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); 	//Thiết lập chân nào là OUTPUT
	/*
	 * Thiết lập Timer
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/100-1);
	TimerIntRegister(TIMER0_BASE,TIMER_A,&PWMADJ);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();
	TimerEnable(TIMER0_BASE, TIMER_A);

	/*
	 * Thiết lập PWM
	 */
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);					// Thiết lập Clock cho PWM
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);			// Thiết lập Enable module PWM 1
	GPIOPinConfigure(GPIO_PF2_M1PWM6);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	thoigian1xung=SysCtlClockGet()/10000;
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, thoigian1xung);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, thoigian1xung*DutyCycle/100);
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);

	/*
	 * Thiết lập Interrupt
	 */
	IntEnable(INT_TIMER0A);											//Enable Interrupt cho Timer 0A
	IntMasterEnable();												//Enable Interrupt Tông

}
void PWMADJ(void){
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	PWMGenDisable(PWM1_BASE, PWM_GEN_3);
	if(DutyCycle>=100)
	{
		DutyCycle=0;
	}
	else DutyCycle = DutyCycle+1;
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,thoigian1xung*DutyCycle/100);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
}
