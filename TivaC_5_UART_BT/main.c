/*
 *  Include thư viện
 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

/*
 * Define
 */
#define LED_RED 	GPIO_PIN_1
#define LED_BLUE 	GPIO_PIN_2
#define LED_GREEN 	GPIO_PIN_3
#define SW1 		GPIO_PIN_4
#define SW2 		GPIO_PIN_0

/*
 * Chương trình gửi chuỗi
 */
bool UARTStringPut(const char *str) {
    while (*str) {
        UARTCharPut(UART0_BASE, *(str++));
    }
    return true;
}

/*
 * Khai báo biến
 */


/*
 * Khai báo chương trình ngắt
 */
void RxInt(void);
int main(void) {
    /*******************************************************************************
     * Cấu hình clock cho hệ thống
     * Cấu hình:
     *          Sử dụng thạch anh ngoài 16MHz
     *          Sử dụng PLL chia xung lên 400MHz
     *          Sau khi qua bộ chia 5, xung đầu ra sẽ là 400/2/5 MHz
     *          (Tham khảo thêm bộ chia vòng khóa pha PLL để biết chi tiết)
     ******************************************************************************/
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    /*
     * Thiết lập GPIO
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, 0xFF);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 0xFF);
    /*
     * Thiết lập UART
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTIntRegister(UART0_BASE,RxInt);
    /*
     * Thiết lập Interrupt
     */
    UARTIntEnable(UART0_BASE,UART_INT_RT);
    IntMasterEnable();
    while (1)
    {

    }
}
void RxInt(void)
		{
			uint32_t IntStatus;
			IntStatus = UARTIntStatus(UART0_BASE, true);	// 	Đọc xem ngắt nào của UART đang được ngắt
			UARTIntClear(UART0_BASE, IntStatus); 			//	Xóa ngắt đó
			unsigned char newchar = UARTCharGetNonBlocking(UART0_BASE);		//Đọc ký tự nhận được từ PC
			if (newchar=='1'){
				GPIOPinWrite(GPIO_PORTF_BASE,0xFF,0);
				GPIOPinWrite(GPIO_PORTF_BASE,0xFF,GPIO_PIN_1);
//				UARTStringPut("LEDRED\n");
				UARTStringPut("TatRelay\n");
				GPIOPinWrite(GPIO_PORTD_BASE,0xFF,0xFF);
			}

			if (newchar=='2'){
				GPIOPinWrite(GPIO_PORTF_BASE,0xFF,GPIO_PIN_0);
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
//				UARTStringPut("LEDBLUE\n");
				UARTStringPut("BatRelay\n");
				GPIOPinWrite(GPIO_PORTD_BASE,0xFF,0);
			}
		}

/*
 * End <main.c>
 */
