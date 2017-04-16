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
#include "driverlib/systick.h"    //Thư viện SYSTICk

/*
 * Định nghĩa Define
 */
#define LED_RED 	GPIO_PIN_1
#define LED_BLUE	GPIO_PIN_2
#define LED_GREEN	GPIO_PIN_3

/*
 * Khai báo biến
 */
extern void ToggleLed(void);
unsigned int led=0;
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
	 * Thiết lập Interrupt
	 */
	IntEnable(INT_TIMER0A);											//Enable Interrupt cho Timer 0A
	IntMasterEnable();												//Enable Interrupt Tông

	/*
	 * Thiết lập SysTick
	 */

	SysTickIntRegister(&ToggleLed);
	SysTickPeriodSet(10000000);
	SysTickIntEnable();
	SysTickEnable();

	/*
	 * Chú ý cần thiết lập khai báo chương trình ngắt ở file tm4c123gh6p_startup.ccs.c thì chương trình ngắt mới chạy được
	 * Khai báo chương trình ngắt ở dòng extern void _c_int00(void); trong file ccs.c
	 * Thay tên IntDefaultHandler bằng tên trương trình ngắt ở dòng  Timer 0 subtimer A
	 */

}
	void ToggleLed(void)
	{
		led=~led;
		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,led);
	}
