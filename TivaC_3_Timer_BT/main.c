/*
 * Bài hướng dẫn TivaC của CLB Điện- Điện Tử EClub PTITHCM
 * Viết bởi Nanolove95 góp ý vui lòng gửi về nanolove95@gmail.com Thanks!!!
 */

/*
 *  Include thư viện
 */
#include <stdbool.h>			//Thư viện định nghĩa kiểu biến Bool
#include <stdint.h>				//Thư viện định nghĩa kiểu biến Int
#include "inc/hw_memmap.h"		//Thư viện định nghĩa định nghĩa ngoại vi
#include "driverlib/gpio.h"		//Thư viện GPIO
#include "driverlib/sysctl.h"	//Thư viện SysCtl
#include "driverlib/interrupt.h"//Thư viện Interrupt
#include "inc/hw_ints.h"		//Thư viện GPIO
#include "driverlib/timer.h"    //
/*
 *  2 Thư viện đặc biệt dùng để unlock chức năng Input cho SW2
 */
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"

/*
 * Định nghĩa Define
 */
#define LED_RED 	GPIO_PIN_1
#define LED_BLUE	GPIO_PIN_2
#define LED_GREEN	GPIO_PIN_3
#define SW1			GPIO_PIN_4
#define SW2			GPIO_PIN_0
/*
 * Khai báo biến
 */
extern void ToggleLed(void);
extern void PushButton(void);
unsigned int led=0,dem;
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
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE);
	/*
	 * Unlock SW2 PF0
	 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	//HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = SW1;

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,  GPIO_PIN_4 | GPIO_PIN_0);				//Thiết lập chân nào là INPUT
	GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2 , GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);


	GPIOIntTypeSet(GPIO_PORTF_BASE,SW1 | SW2, GPIO_FALLING_EDGE); 	//Thiết lập Interrupt khi có sự kiện ngắt cạnh xuống
	GPIOIntRegister(GPIO_PORTF_BASE,&PushButton);					//Khai báo chương trình ngắt sẽ xảy ra khi có sự kiện Falling Edge
	GPIOIntEnable(GPIO_PORTF_BASE, SW1 | SW2);						//Enable Interrupt cho SW1 và SW2
	IntEnable(INT_GPIOF);											//Enable Interrupt cho PORTF
	/*
	 * Thiết lập Interrupt
	 */
	IntEnable(INT_TIMER0A);											//Enable Interrupt cho Timer 0A
	IntMasterEnable();												//Enable Interrupt Tông

	/*
	 * Thiết lập Timer
	 */
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_BOTH, SysCtlClockGet()*5 - 1);
	TimerIntRegister(TIMER0_BASE,TIMER_BOTH,&ToggleLed);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_BOTH);
	/*
	 * Chú ý cần thiết lập khai báo chương trình ngắt ở file tm4c123gh6p_startup.ccs.c thì chương trình ngắt mới chạy được
	 * Khai báo chương trình ngắt ở dòng extern void _c_int00(void); trong file bccs.c
	 * Thay tên IntDefaultHandler bằng tên trương trình ngắt ở dòng  Timer 0 subtimer A
	 */

}
	void ToggleLed(void)
	{
		TimerDisable(TIMER0_BASE,TIMER_BOTH);
		TimerLoadSet(TIMER0_BASE, TIMER_BOTH, SysCtlClockGet()*3 - 1);
		TimerEnable(TIMER0_BASE, TIMER_BOTH);
//		TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
//		if(dem==2){
//			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,LED_RED);
//		}
//		if(dem==3){
//			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,LED_BLUE);
//		}
//		if(dem==4){
//			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,LED_GREEN);
//		}

	}

	void PushButton(void){
		GPIOIntClear(GPIO_PORTF_BASE, SW1);
		dem++;
	}
