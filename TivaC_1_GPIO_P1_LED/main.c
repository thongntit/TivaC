/*
 * Bài hướng dẫn TivaC của CLB Điện- Điện Tử EClub PTITHCM
 * Viết bởi Nanolove95 góp ý vui lòng gửi về nanolove95@gmail.com Thanks!!!
 */

/*
 *  Include thư viện
 */
#include <stdbool.h>			//Thư viện định nghĩa kiểu biến Bool
#include <stdint.h>				//Thư viện định nghĩa kiểu biến Int
#include "inc/hw_memmap.h"		//Thư viện định nghĩa định nghĩa chức năng ngoại vi
#include "driverlib/gpio.h"		//Thư viện GPIO
#include "driverlib/sysctl.h"	//Thư viện SysCtl

/*
 * Định nghĩa Define
 */
#define LED_RED 	GPIO_PIN_1			// Define để thuận tiện hơn khi code
#define LED_BLUE	GPIO_PIN_2
#define LED_GREEN	GPIO_PIN_3

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
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //set chức năng I/O cho PORTF
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); 	//Thiết lập chân nào là OUTPUT
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE);			// 2 cách thiết lập là như nhau
//	GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	LED_RED	);  	//Thiết lập giá trị OUTPUT tại chân 1 2 3

	while(1)
	{
		GPIOPinWrite(GPIO_PORTF_BASE,LED_RED,LED_RED);
		SysCtlDelay(SysCtlClockGet()/3);
		GPIOPinWrite(GPIO_PORTF_BASE,LED_RED,0);

		GPIOPinWrite(GPIO_PORTF_BASE,LED_BLUE,LED_BLUE);
		SysCtlDelay(SysCtlClockGet()/3);
		GPIOPinWrite(GPIO_PORTF_BASE,LED_BLUE,0);

		GPIOPinWrite(GPIO_PORTF_BASE,LED_GREEN,LED_GREEN);
		SysCtlDelay(SysCtlClockGet()/3);
		GPIOPinWrite(GPIO_PORTF_BASE,LED_GREEN,0);
	}

	/*
	 * Nâng cao
	 */
//	unsigned int led=LED_RED;
//	while(1)
//	{
//		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	led	);
//		SysCtlDelay(SysCtlClockGet()/3);			//delay 1s
//		led<<=1;			// dịch biến led sang trái 1
//		if (led>LED_GREEN)
//		{
//			led=LED_RED;
//		}
//	}
}
