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
unsigned int btn1,btn2,dem,led;
/*
 * Khai báo chương trình ngắt (sẽ được trở tới bởi dòng GPIOIntRegister)
 */
extern void HoldButton(void);
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

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	//HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = SW1;

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,  GPIO_PIN_4 | GPIO_PIN_0);				//Thiết lập chân nào là INPUT
//	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,  SW1 | SW2);
	GPIODirModeSet(GPIO_PORTF_BASE,SW1 | SW2 , GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2 , GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	/*
	 * Thiết lập Interrupt
	 */
	GPIOIntTypeSet(GPIO_PORTF_BASE,SW1 | SW2, GPIO_LOW_LEVEL); 		//Thiết lập Interrupt khi có sự kiện ngắt mức thấp
	GPIOIntRegister(GPIO_PORTF_BASE,&HoldButton);					//Thiết lập trỏ tới chương trình con PushButton khi có sự kiện ngắt
	GPIOIntEnable(GPIO_PORTF_BASE, SW1 | SW2);						//Enable Interrupt cho SW1 và SW2
	IntEnable(INT_GPIOF);											//Enable Interrupt cho PORTF
	IntMasterEnable();												//Enable Interrupt Tông


					//Hiệu ứng bài tập về nhà bài GPIO lấy xài tạm :))
	while(1)
	{
		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	0);
	}
}

	void HoldButton(void)
	{
		GPIOIntClear(GPIO_PORTF_BASE,SW1|SW2);											//Clear cờ ngắt
		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	LED_RED);		//Bật led đỏ
	}

