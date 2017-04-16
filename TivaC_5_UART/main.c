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

void Button(void)
		{
			if (GPIOIntStatus(GPIO_PORTF_BASE, true) == GPIO_PIN_4)
			{
				UARTStringPut("Left\n");
				GPIOIntClear(GPIO_PORTF_BASE, SW1);
				SysCtlDelay(SysCtlClockGet()/30);

			}


			if (GPIOIntStatus(GPIO_PORTF_BASE, true) == GPIO_PIN_0)
			{
				UARTStringPut("Right\n");
				GPIOIntClear(GPIO_PORTF_BASE, SW2);
				SysCtlDelay(SysCtlClockGet()/30);
			}

		}

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
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,  SW1 | SW2);
    GPIODirModeSet(GPIO_PORTF_BASE,SW1 | SW2 , GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2 , GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
    GPIOIntTypeSet(GPIO_PORTF_BASE,SW1 | SW2, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE,&Button);
    GPIOIntEnable(GPIO_PORTF_BASE, SW1 | SW2);

    /*
     * Thiết lập UART
     */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);

    /*
     * Thiết lập Interrupt
     */
    IntEnable(INT_GPIOF);
    IntMasterEnable();

	UARTCharPut(UART0_BASE,'H');
	UARTCharPut(UART0_BASE,'E');
	UARTCharPut(UART0_BASE,'L');
	UARTCharPut(UART0_BASE,'L');
	UARTCharPut(UART0_BASE,'O');
	UARTCharPut(UART0_BASE,'\n');
    while (1) {

    }
}
