
#ifndef _VARIANT_RISCDUINO
#define _VARIANT_RISCDUINO


#include <stdint.h>

#define RISCDUINO_PLATFORM
#define RISCDUINO
#define RISCV
#include "platform.h"


/*----------------------------------------------------------------------------
*        Headers
*----------------------------------------------------------------------------*/

#include "Arduino.h"
#ifdef __cplusplus
#include "UARTClass.h"
#include "TIMERClass.h"
#endif




/* LEDs */
#define PIN_LED_13          13
#define PIN_LED             3
#define LED_BUILTIN         3

#ifdef __cplusplus
extern UARTClass Serial;
extern UARTClass Serial1;
extern TIMERClass Timer;
#endif

/*
 * SPI Interfaces
 */

#define SPI_INTERFACES_COUNT 1
#define SPI_REG(x) SPI0_REG(x)

#define UART_INTERFACES_COUNT 1
//#define UART_REG(x) UART0_REG(x)
#define UART_REGP(base,i) _REG32P(base, (i))

// we only want to enable 3 peripheral managed SPI pins: SCK, MOSI, MISO
// CS pins can either be handled by hardware or bit banged as GPIOs

static const uint8_t SS   = PIN_SPI1_SS0;
static const uint8_t SS1  = PIN_SPI1_SS1;
static const uint8_t SS2  = PIN_SPI1_SS2;
static const uint8_t SS3  = PIN_SPI1_SS3;
static const uint8_t MOSI = PIN_SPI1_MOSI;
static const uint8_t MISO = PIN_SPI1_MISO;
static const uint8_t SCK  = PIN_SPI1_SCK;

static const uint32_t SPI_IOF_MASK  = 0x400;
static const uint32_t IOF_UART_MASK = IOF0_UART0_MASK;
/*********************************************************************
ATMEGA328   Functionality           ARDUINO PIN No    Riscduino       Caravel Pin Mapping
  Pin                                 Pin No           GPIO No
Pin-2	    PD0/RXD[0]	                 D0	           24           digital_io[6]
Pin-3	    PD1/TXD[0]	                 D1	           25           digital_io[7]/analog_io[0]
Pin-4	    PD2/RXD[1]/INT0	         D2	           26           digital_io[8]/analog_io[1]
Pin-5	    PD3/INT1/OC2B(PWM0)	         D3	           27           digital_io[9]/analog_io[2]
Pin-6	    PD4/TXD[1]	                 D4	           28           digital_io[10]/analog_io[3]
Pin-11	    PD5/SS[3]/OC0B(PWM1)/T1	 D5	           29           digital_io[13]/analog_io[6]
Pin-12	    PD6/SS[2]/OC0A(PWM2)/AIN0	 D6	           30           digital_io[14]/analog_io[7]
Pin-13	    PD7/A1N1	                 D7	           31           digital_io[15]/analog_io[8]
Pin-14	    PB0/CLKO/ICP1	             D8	            8           digital_io[16]/analog_io[9]
Pin-15	    PB1/SS[1]OC1A(PWM3)	         D9	            9           digital_io[17]/analog_io[10]
Pin-16	    PB2/SS[0]/OC1B(PWM4)	    D10	           10           digital_io[18]/analog_io[11]
Pin-17	    PB3/MOSI/OC2A(PWM5)	        D11	           11           digital_io[19]/analog_io[12]
Pin-18	    PB4/MISO	                D12	           12           digital_io[20]/analog_io[13]
Pin-19	    PB5/SCK	                    D13	           13           digital_io[21]/analog_io[14]
Pin-23	    PC0/uartm_rxd/ADC0	        D14/A0	      16            digital_io[22]/analog_io[15]
Pin-24	    PC1/uartm/ADC1	            D15/A1	      17            digital_io[23]/analog_io[16]
Pin-25	    PC2/usb_dp/ADC2	            D16/A2	      18            digital_io[24]/analog_io[17]
Pin-26	    PC3/usb_dn/ADC3	            D17/A3	      19            digital_io[25]/analog_io[18]
Pin-27	    PC4/ADC4/SDA	            D18/A4	      20            digital_io[26]/analog_io[19]
Pin-28	    PC5/ADC5/SCL	            D19/A5	      21            digital_io[27]/analog_io[20]
Pin-1	    PC6/RESET		            D20           22            digital_io[5]
Pin-9	    PB6/XTAL1/TOSC1		        D21           14            digital_io[11]/analog_io[4]
Pin-10	    PB7/XTAL2/TOSC2		        D22           15            digital_io[12]/analog_io[5]
SPARE	    PA1		                    D23           1             Caravel_sdo/digital_io[1]
SPARE	    PA2		                    D24           2             Caravel_sdi/digital_io[2]
SPARE	    PA4		                    D25           4             Caravel_sck/digital_io[4]

// Future Use
SPARE	    PA0		                D23           0             Caravel_Jtag/digital_io[0]
SPARE	    PA3		                D26           3             Caravel_csb/digital_io[3]
***********************************************************************/

#define VARIANT_DIGITAL_PIN_MAP  { \
        {.io_port = 0, .bit_pos = 24, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0}, \
	{.io_port = 0, .bit_pos = 25, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0}, \
	{.io_port = 0, .bit_pos = 26, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0}, \
	{.io_port = 0, .bit_pos = 27, .pwm_num = 0,   .pwm_cmp_num = 1, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 28, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 29, .pwm_num = 1,   .pwm_cmp_num = 2, .ws281x_num = 0}, \
	{.io_port = 0, .bit_pos = 30, .pwm_num = 2,   .pwm_cmp_num = 3, .ws281x_num = 0}, \
	{.io_port = 0, .bit_pos = 31, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0}, \
	{.io_port = 0, .bit_pos = 8,  .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0}, \
	{.io_port = 0, .bit_pos = 9,  .pwm_num = 3,   .pwm_cmp_num = 1, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 10, .pwm_num = 4,   .pwm_cmp_num = 2, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 11, .pwm_num = 5,   .pwm_cmp_num = 3, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 12, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 13, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 16, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 17, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 18, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 19, .pwm_num = 0xF, .pwm_cmp_num = 1, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 20, .pwm_num = 0xF, .pwm_cmp_num = 2, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 21, .pwm_num = 0xF, .pwm_cmp_num = 2, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 22, .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 14, .pwm_num = 0xF,  .pwm_cmp_num = 2, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 15, .pwm_num = 0xF, .pwm_cmp_num = 2, .ws281x_num = 1}, \
	{.io_port = 0, .bit_pos = 1,  .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 2,  .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}, \
	{.io_port = 0, .bit_pos = 4,  .pwm_num = 0xF, .pwm_cmp_num = 0, .ws281x_num = 0xF}}

#define VARIANT_NUM_PIN (26)

#define VARIANT_PWM_LIST {(volatile void *) PWM0_BASE_ADDR, \
      (volatile void *) PWM1_BASE_ADDR, \
      (volatile void *) PWM2_BASE_ADDR}





#define VARIANT_NUM_WS281X (4)
#define VARIANT_NUM_PWM (3)
#define VARIANT_NUM_SPI (1)
// For interfacing with the onboard SPI Flash.
#define VARIANT_NUM_QSPI (1)
#define VARIANT_NUM_UART (1)


#define PIN_A0   (14)
#define PIN_A1   (15)
#define PIN_A2   (16)
#define PIN_A3   (17)
#define PIN_A4   (18)
#define PIN_A5   (19)

static const uint8_t A0 = PIN_A0;
static const uint8_t A1 = PIN_A1;
static const uint8_t A2 = PIN_A2;
static const uint8_t A3 = PIN_A3;
static const uint8_t A4 = PIN_A4;
static const uint8_t A5 = PIN_A5;


#endif 
