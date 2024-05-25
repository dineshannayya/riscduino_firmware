// See LICENSE for license details.

#ifndef _RISCDUINO_PLATFORM_H
#define _RISCDUINO_PLATFORM_H

// Some things missing from the official encoding.h
#define MCAUSE_INT         0x80000000
#define MCAUSE_CAUSE       0x7FFFFFFF

#include "riscduino/const.h"
#include "riscduino/devices/aon.h"
#include "riscduino/devices/clint.h"
#include "riscduino/devices/glbl.h"
#include "riscduino/devices/gpio.h"
#include "riscduino/devices/ws281x.h"
#include "riscduino/devices/timer.h"
#include "riscduino/devices/plic.h"
#include "riscduino/devices/pwm.h"
#include "riscduino/devices/spi.h"
#include "riscduino/devices/wire.h"
#include "riscduino/devices/uart.h"
#include "riscduino/devices/riscv.h"

/****************************************************************************
 * Platform definitions
 *****************************************************************************/

#define TRAPVEC_TABLE_BASE_ADDR _AC(0x00001010,UL)
#define GLBL_BASE_ADDR          _AC(0x10020000,UL)
#define GPIO_BASE_ADDR          _AC(0x10020080,UL)
#define PWM_BASE_ADDR           _AC(0x10020100,UL)
#define TIMER_BASE_ADDR         _AC(0x10020180,UL)
#define SEMA_BASE_ADDR          _AC(0x10020200,UL)
#define WS281X_BASE_ADDR        _AC(0x10020280,UL)
#define UART0_BASE_ADDR         _AC(0x10010000,UL)
#define UART1_BASE_ADDR         _AC(0x10010100,UL)
#define WIRE_BASE_ADDR          _AC(0x10010040,UL)
#define SPI0_BASE_ADDR          _AC(0x100100C0,UL)
#define RISCV_BASE_ADDR         _AC(0x0C490000,UL)

// Need to Remove It - Dinesh A
#define CLINT_BASE_ADDR _AC(0x02000000,UL)
#define PLIC_BASE_ADDR _AC(0x0C000000,UL)
#define AON_BASE_ADDR _AC(0x10000000,UL)
#define SPI1_BASE_ADDR _AC(0x10024000,UL)
#define SPI2_BASE_ADDR _AC(0x10034000,UL)
#define SPI0_MMAP_ADDR _AC(0x20000000,UL)
#define PWM0_BASE_ADDR _AC(0x10015000,UL)
#define PWM1_BASE_ADDR _AC(0x10025000,UL)
#define PWM2_BASE_ADDR _AC(0x10035000,UL)
#define MEM_BASE_ADDR _AC(0x80000000,UL)

// IOF Mappings
#define IOF0_SPI1_MASK          _AC(0x000007FC,UL)
#define SPI11_NUM_SS     (4)
#define IOF_SPI1_SS0          (2u)
#define IOF_SPI1_SS1          (8u)
#define IOF_SPI1_SS2          (9u)
#define IOF_SPI1_SS3          (10u)
#define IOF_SPI1_MOSI         (3u)
#define IOF_SPI1_MISO         (4u)
#define IOF_SPI1_SCK          (5u)
#define IOF_SPI1_DQ0          (3u)
#define IOF_SPI1_DQ1          (4u)
#define IOF_SPI1_DQ2          (6u)
#define IOF_SPI1_DQ3          (7u)

#define IOF0_SPI2_MASK          _AC(0xFC000000,UL)
#define SPI2_NUM_SS       (1)
#define IOF_SPI2_SS0          (26u)
#define IOF_SPI2_MOSI         (27u)
#define IOF_SPI2_MISO         (28u)
#define IOF_SPI2_SCK          (29u)
#define IOF_SPI2_DQ0          (27u)
#define IOF_SPI2_DQ1          (28u)
#define IOF_SPI2_DQ2          (30u)
#define IOF_SPI2_DQ3          (31u)

// MULTI FUNCTION ENABLE
// MASTER UART ENABLE
#define IOF0_PWM_ENB            _AC(0x00000001, UL)
#define IOF0_PWM0_ENB           _AC(0x00000001, UL)
#define IOF0_PWM1_ENB           _AC(0x00000002, UL)
#define IOF0_PWM2_ENB           _AC(0x00000004, UL)
#define IOF0_PWM3_ENB           _AC(0x00000008, UL)
#define IOF0_PWM4_ENB           _AC(0x00000010, UL)
#define IOF0_PWM5_ENB           _AC(0x00000020, UL)
#define IOF0_INT0_ENB           _AC(0x00000040, UL)
#define IOF0_INT1_ENB           _AC(0x00000080, UL)
#define IOF0_UART0_ENB          _AC(0x00000100, UL)
#define IOF0_UART1_ENB          _AC(0x00000200, UL)
#define IOF0_SPI_ENB            _AC(0x00000400, UL)
#define IOF0_SPI_CS0_ENB        _AC(0x00000800, UL)
#define IOF0_SPI_CS1_ENB        _AC(0x00001000, UL)
#define IOF0_SPI_CS2_ENB        _AC(0x00002000, UL)
#define IOF0_SPI_CS3_ENB        _AC(0x00004000, UL)
#define IOF0_WIRE_ENB           _AC(0x00008000, UL)
#define IOF0_USB_ENB            _AC(0x00010000, UL)
#define IOF0_TAP_ENB            _AC(0x40000000, UL)
#define IOF0_MUART_ENB          _AC(0x80000000, UL)

// RESET CONTROL
#define IOF0_SPI_RST            _AC(0x00000004, UL)
#define IOF0_UART0_RST          _AC(0x00000008, UL)
#define IOF0_WIRE_RST           _AC(0x00000010, UL)
#define IOF0_USB_RST            _AC(0x00000020, UL)
#define IOF0_UART1_RST          _AC(0x00000040, UL)
#define IOF0_CORE0_RST          _AC(0x00000100, UL)
#define IOF0_CORE1_RST          _AC(0x00000200, UL)
#define IOF0_CORE2_RST          _AC(0x00000400, UL)
#define IOF0_CORE3_RST          _AC(0x00000800, UL)

#define IOF0_UART0_MASK         _AC(0x00000100, UL)
#define IOF_UART0_RX   (16u)
#define IOF_UART0_TX   (17u)

#define IOF0_UART1_MASK         _AC(0x00000200, UL)
#define IOF_UART1_RX (24u)
#define IOF_UART1_TX (25u)

#define IOF1_PWM0_MASK          _AC(0x0000000F, UL)
#define IOF1_PWM1_MASK          _AC(0x00780000, UL)
#define IOF1_PWM2_MASK          _AC(0x00003C00, UL)

#define WIRE_IOF_MASK           _AC(0x00008000, UL)


// Interrupt Numbers
#define INT_TIMER_BASE   0
#define INT_WIRE         2
#define INT_USB          3
#define INT_UART0_BASE   3
#define INT_UART1_BASE   4
#define INT_SPI0_BASE    5
#define INT_SPI1_BASE    6
#define INT_SPI2_BASE    7
#define INT_GPIO_BASE    8
#define INT_PWM0_BASE    40
#define INT_PWM1_BASE    44
#define INT_PWM2_BASE    48

// Helper functions
#define _REG32(p, i) (*(volatile uint32_t *) ((p) + (i)))
#define _REG32P(p, i) ((volatile uint32_t *) ((p) + (i)))
#define _REG8(p, i) (*(volatile uint8_t *) ((p) + (i)))
#define _REG8P(p, i) ((volatile uint8_t *) ((p) + (i)))
#define AON_REG(offset) _REG32(AON_BASE_ADDR, offset)
#define CLINT_REG(offset) _REG32(CLINT_BASE_ADDR, offset)
#define GLBL_REG(offset)  _REG32(GLBL_BASE_ADDR, offset)
#define GPIO_REG(offset)  _REG32(GPIO_BASE_ADDR, offset)
#define PWM_REG(offset)   _REG32(PWM_BASE_ADDR, offset)
#define TIMER_REG(offset) _REG32(TIMER_BASE_ADDR, offset)
#define SEMA_REG(offset)  _REG32(SEMA_BASE_ADDR, offset)
#define WS281X_REG(offset)_REG32(WS281X_BASE_ADDR, offset)
#define OTP_REG(offset)   _REG32(OTP_BASE_ADDR, offset)
#define PLIC_REG(offset)  _REG32(PLIC_BASE_ADDR, offset)
#define PRCI_REG(offset)  _REG32(PRCI_BASE_ADDR, offset)
#define PWM0_REG(offset)  _REG32(PWM0_BASE_ADDR, offset)
#define PWM1_REG(offset)  _REG32(PWM1_BASE_ADDR, offset)
#define PWM2_REG(offset)  _REG32(PWM2_BASE_ADDR, offset)
#define SPI0_REG(offset)  _REG32(SPI0_BASE_ADDR, offset)
#define SPI1_REG(offset)  _REG32(SPI1_BASE_ADDR, offset)
#define SPI2_REG(offset)  _REG32(SPI2_BASE_ADDR, offset)
#define UART0_REG(offset) _REG32(UART0_BASE_ADDR, offset)
#define UART1_REG(offset) _REG32(UART1_BASE_ADDR, offset)
#define UART_REG(base,offset) _REG32(base, offset)
#define WIRE_REG(offset) _REG32(WIRE_BASE_ADDR, offset)
#define RISCV_REG(offset) _REG32(RISCV_BASE_ADDR, offset)


// GPIO Port Defination
// GPIO DIRECT SEL
#define DDRA  _REG8(GPIO_BASE_ADDR, GPIO_DSEL+0)
#define DDRB  _REG8(GPIO_BASE_ADDR, GPIO_DSEL+1)
#define DDRC  _REG8(GPIO_BASE_ADDR, GPIO_DSEL+2)
#define DDRD  _REG8(GPIO_BASE_ADDR, GPIO_DSEL+3)

// GPIO OUTPUT PORT
#define PORTA  _REG8(GPIO_BASE_ADDR, GPIO_ODATA+0)
#define PORTA7  7
#define PORTA6  6
#define PORTA5  5
#define PORTA4  4
#define PORTA3  3
#define PORTA2  2
#define PORTA1  1
#define PORTA0  0

#define PORTB  _REG8(GPIO_BASE_ADDR, GPIO_ODATA+1)
#define PORTB7  7
#define PORTB6  6
#define PORTB5  5
#define PORTB4  4
#define PORTB3  3
#define PORTB2  2
#define PORTB1  1
#define PORTB0  0

#define PORTC  _REG8(GPIO_BASE_ADDR, GPIO_ODATA+2)
#define PORTC7  7
#define PORTC6  6
#define PORTC5  5
#define PORTC4  4
#define PORTC3  3
#define PORTC2  2
#define PORTC1  1
#define PORTC0  0

#define PORTD  _REG8(GPIO_BASE_ADDR, GPIO_ODATA+3)
#define PORTD7  7
#define PORTD6  6
#define PORTD5  5
#define PORTD4  4
#define PORTD3  3
#define PORTD2  2
#define PORTD1  1
#define PORTD0  0

// GPIO IN PORT
#define PINA  _REG8(GPIO_BASE_ADDR, GPIO_IDATA+0)
#define PINA7  7
#define PINA6  6
#define PINA5  5
#define PINA4  4
#define PINA3  3
#define PINA2  2
#define PINA1  1
#define PINA0  0

#define PINB  _REG8(GPIO_BASE_ADDR, GPIO_IDATA+1)
#define PINB7  7
#define PINB6  6
#define PINB5  5
#define PINB4  4
#define PINB3  3
#define PINB2  2
#define PINB1  1
#define PINB0  0

#define PINC  _REG8(GPIO_BASE_ADDR, GPIO_IDATA+2)
#define PINC7  7
#define PINC6  6
#define PINC5  5
#define PINC4  4
#define PINC3  3
#define PINC2  2
#define PINC1  1
#define PINC0  0

#define PIND  _REG8(GPIO_BASE_ADDR, GPIO_IDATA+3)
#define PIND7  7
#define PIND6  6
#define PIND5  5
#define PIND4  4
#define PIND3  3
#define PIND2  2
#define PIND1  1
#define PIND0  0

// Misc

#include <stdint.h>


#define NUM_GPIO 24

#define PLIC_NUM_INTERRUPTS 52
#define PLIC_NUM_PRIORITIES 7

#define HAS_BOARD_BUTTONS
#include "riscduino.h"

unsigned long get_timer_freq(void);
uint64_t get_timer_value(void);

#endif /* _RISCDUINO_PLATFORM_H */
