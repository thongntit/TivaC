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

/*
 * Định nghĩa Define
 */
#define LED_RED 	GPIO_PIN_1
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
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE);
	GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	LED_RED	);  	//Thiết lập giá trị OUTPUT tại chân 1 2 3

	/*
	 * Nâng cao
	 */
	unsigned int led,dem;
	while(1)
	{
		led=dem*2;
		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	led	);
		SysCtlDelay(SysCtlClockGet()/3);			//delay 1s
		dem++;
		if(dem==8)
		{
			dem=0;
		}
	}



//	unsigned int i;
//	while(1)
//	{
//		for(i=0;i<=8;i++)
//		{
//			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	i*2	);
//			SysCtlDelay(SysCtlClockGet()/3);			//delay 1s
//		}
//	}
}
