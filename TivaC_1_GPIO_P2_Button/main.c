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
unsigned int btn1,btn2;

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
	 *  Unlock chức năng Input cho chân PF0
	 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	//HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = SW1;

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1 | SW2  );					//Thiết lập chân nào là INPUT
//	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 | GPIO_PIN_0 );	// cách khác
//	GPIODirModeSet(GPIO_PORTF_BASE, SW1 | SW2 , GPIO_DIR_MODE_IN);		// cách khác nữa
	/*
	 * Thiết lập kiểu chân cho chân PF0 và PF4 là Pull Up (có điện trở kéo lên)
	 */
	GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2 ,GPIO_STRENGTH_2MA ,GPIO_PIN_TYPE_STD_WPU);


	while(1)
	{
		btn1=GPIOPinRead(GPIO_PORTF_BASE,SW1); // đọc SW1
		btn2=GPIOPinRead(GPIO_PORTF_BASE,SW2); // đọc SW2
		if (btn1==0)														// nếu SW1 được nhấn thì sáng led màu đỏ
		{
			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN, 0);
			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, LED_RED);
		}
		if (btn2==0)														// nếu SW2 được nhấn thì sáng led màu lục
		{
			GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN, 0);
			GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, LED_GREEN);
		}
	}

/*
 * 		NÂNG CAO TÍ XÍU
 */
//	unsigned int key;
//	while(1)
//	{
//		key=GPIOPinRead(GPIO_PORTF_BASE,SW1|SW2);
//		if (key==SW2)
//		{
//			GPIOPinWrite(GPIO_PORTF_BASE,LED_RED, LED_RED);
//		}
//		if (key==SW1)
//		{
//			GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, LED_GREEN);
//		}
//	}

}
