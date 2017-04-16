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
#include "inc/hw_ints.h"		//Thư viện thiết lập chân của interrupt
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
void PushButton(void);
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
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE);			//Cách khác

	/*
	 * Unlock SW2 PF0
	 */
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	//HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = SW1;

	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,  GPIO_PIN_4 | GPIO_PIN_0);				//Thiết lập chân nào là INPUT
//	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,  SW1 | SW2);
//	GPIODirModeSet(GPIO_PORTF_BASE,SW1 | SW2 , GPIO_DIR_MODE_IN);					//Lệnh DirModeSet tương đương PinTypeOutput nhưng có chức năng rộng hơn.
	/*
	 * Lệnh Padconfigset dùng để thiết lập dòng ra tối đa nếu là chân Ouput
	 * Thiết lập trở kéo lên, kéo xuống nếu là chân Input
	 */
	GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2 , GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
	/*
	 * Thiết lập Interrupt
	 */
	GPIOIntTypeSet(GPIO_PORTF_BASE,SW1 | SW2, GPIO_FALLING_EDGE); 	//Thiết lập Interrupt khi có sự kiện ngắt cạnh xuống
	GPIOIntRegister(GPIO_PORTF_BASE,PushButton);					//Khai báo chương trình ngắt sẽ xảy ra khi có sự kiện Falling Edge
	GPIOIntEnable(GPIO_PORTF_BASE, SW1 | SW2);						//Enable Interrupt cho SW1 và SW2
	IntEnable(INT_GPIOF);											//Enable Interrupt cho PORTF
	IntMasterEnable();												//Enable Interrupt Tông


					//Hiệu ứng bài tập về nhà bài GPIO lấy xài tạm :))
	while(1)
	{
		GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE ,	led);
	}
}

	void PushButton(void)						//Tạm thời không quan tâm nút nào được nhấn cho đơn giản
	{											//Chương trình hoạt động giống bài tập về nhà của phần GPIO nên khỏi giải thích nha
		SysCtlDelay(SysCtlClockGet()/50); 		//Chống rung phím
		GPIOIntClear(GPIO_PORTF_BASE,SW1|SW2);	//Xóa cờ ngắt
		dem++;
		if(dem==9)
		{
			dem=0;
		}
		led=dem*2;
	}

	/*
	 * Nâng cao
	 */

//	void PushButton(void)
//	{
//		SysCtlDelay(SysCtlClockGet()/50); //Chống rung phím
//		if (GPIOIntStatus(GPIO_PORTF_BASE, true) == GPIO_PIN_4)		//Kiểm tra xem có ngắt ở SW1 không
//		{
//			GPIOIntClear(GPIO_PORTF_BASE, SW1);
//			dem++;
//			if(dem==9)
//			{
//				dem=0;
//			}
//			led=dem*2;
//		}
//		else
//		{
//			GPIOIntClear(GPIO_PORTF_BASE, SW2);
//			if (dem==0)
//				{dem=8;}
//			dem--;
//			led=dem*2;
//		}
//	}

