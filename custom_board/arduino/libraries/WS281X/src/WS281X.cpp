/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "WS281X.h"

#include "variant.h"
/************************************************************

WS2811
    LOW Speed Mode: 400Khz (2.5us)
                                      Actual Time     Tolerence
   TOH    0 code, high voltage time    0.5us           +/- 150ns
0  T0L    0 code, low voltage time     2.0us           +/- 150ns

   T1H    1 code, high voltage time    1.2us           +/- 150ns
1  TIL    1 code  low voltage time     1.3us           +/- 150ns
   RESET      LOW Voltage Time         Above 50Us


    High Speed Mode: 800Khz (1.25us)
                                      Actual Time     Tolerence
   TOH    0 code, high voltage time    0.25us           +/- 150ns
0  T0L    0 code, low voltage time     1.0us           +/- 150ns

   T1H    1 code, high voltage time    0.6us           +/- 150ns
1  TIL    1 code  low voltage time     0.65us          +/- 150ns
   RESET      LOW Voltage Time         Above 50Us





WS2812/WS2812S
   Data Transfer Time( TH+TL = 1.25us +/- 600ns)
                                      Actual Time     Tolerence
   TOH    0 code, high voltage time    0.35us          +/- 150ns
0  T0L    0 code, low voltage time     0.8us           +/- 150ns

   T1H    1 code, high voltage time    0.7us           +/- 150ns
1  TIL    1 code  low voltage time     0.6us           +/- 150ns
   RESET      LOW Voltage Time         Above 50Us

WS2812B
   Data Transfer Time( TH+TL = 1.25us +/- 150ns)
                                      Actual Time     Tolerence
0  TOH    0 code, high voltage time    0.35us          +/- 150ns
   T0L    0 code, low voltage time     0.9us           +/- 150ns

   T1H    1 code, high voltage time    0.9us           +/- 150ns
1  TIL    1 code  low voltage time     0.35us          +/- 150ns
   RESET      LOW Voltage Time         Above 50Us

*************************************************************/



WS281XClass ws281x(WS281X_BASE_ADDR);

WS281XClass::WS281XClass(uint32_t base_addr) {
    base = base_addr;
}


// Public Methods //////////////////////////////////////////////////////////////

void WS281XClass::begin(uint8_t mode) {

   // Configure 1us period based on CPU clock
   // Assumption CPU clock in MHZ
   TIMER_REG(TIMER_GLBL_CFG) = (F_CPU/1000000) -1;

   // RESET PERIOD
   WS281X_REG(WS281X_CFG0) = (F_CPU/WS281X_RST)-1;

  // Set the PERIOD and HIGH Period for 0 & 1
  switch(mode) {
  case WS2811_LOW_SPEED:
       WS281X_REG(WS281X_CFG1) = (((F_CPU/WS2811_LS_T1H)  & 0x3FF) << 20) | (((F_CPU/WS2811_LS_TOH)  & 0x3FF) << 10) | (((F_CPU/WS_SPEED_400KHZ) -1)&0x3FF);
       break;
  case WS2811_HIGH_SPEED:
       WS281X_REG(WS281X_CFG1) = (((F_CPU/WS2811_HS_T1H)  & 0x3FF) << 20) | (((F_CPU/WS2811_HS_TOH)  & 0x3FF) << 10) | (((F_CPU/WS_SPEED_800KHZ) -1)&0x3FF);
       break;
  case WS2812_HIGH_SPEED:
       WS281X_REG(WS281X_CFG1) = (((F_CPU/WS2812_T1H)  & 0x3FF) << 20) | (((F_CPU/WS2812_TOH)  & 0x3FF) << 10) | (((F_CPU/WS_SPEED_800KHZ) -1)&0x3FF);
       break;
  case WS2812S_HIGH_SPEED:
       WS281X_REG(WS281X_CFG1) = (((F_CPU/WS2812S_T1H)  & 0x3FF) << 20) | (((F_CPU/WS2812S_TOH)  & 0x3FF) << 10) | (((F_CPU/WS_SPEED_800KHZ) -1)&0x3FF);
       break;

  case WS2812B_HIGH_SPEED:
       WS281X_REG(WS281X_CFG1) = (((F_CPU/WS2812B_T1H)  & 0x3FF) << 20) | (((F_CPU/WS2812B_TOH)  & 0x3FF) << 10) | (((F_CPU/WS_SPEED_800KHZ) -1)&0x3FF);
       break;

  }
 
}
void WS281XClass::enable(uint8_t pin) {
    // ws281x pin mumber corresponds to arduino port
    uint8_t gpio_num   = variant_pin_map[pin].bit_pos;
    uint8_t ws281x_num = variant_pin_map[pin].ws281x_num;
 
    GPIO_REG(GPIO_TYPE)    |= 1 << gpio_num;
    WS281X_REG(WS281X_CMD) |= 1 << ws281x_num;
}

// fast Write, without checking fifo full condition
void WS281XClass::fast_write(uint8_t pin,uint32_t data) {
    // ws281x pin mumber corresponds to arduino port
    uint8_t ws281x_num = variant_pin_map[pin].ws281x_num;

    // Don't check FIFO status as Hardware made sure that FIFO is not over-written and ack will be delayed 
    WS281X_REG(WS281X_FIFO_BASE | (ws281x_num << 2)) = data;
}

// Normal Write, checking fifo full condition
void WS281XClass::write(uint8_t pin,uint32_t data) {
    volatile int32_t x;

    // ws281x pin mumber corresponds to arduino port
    uint8_t ws281x_num = variant_pin_map[pin].ws281x_num;

    // Check FIFO is not full 
    while((((x = WS281X_REG(WS281X_STATUS)) >>  (ws281x_num << 2)) & 0x1) == 1);
    WS281X_REG(WS281X_FIFO_BASE | (ws281x_num << 2)) = data;
}

void WS281XClass::disable(uint8_t pin) {
    // ws281x pin mumber corresponds to arduino port
    uint8_t gpio_num   = variant_pin_map[pin].bit_pos;
    uint8_t ws281x_num = variant_pin_map[pin].ws281x_num;
 
    GPIO_REG(GPIO_TYPE)    &= ~(1 << gpio_num);
    WS281X_REG(WS281X_CMD) &= ~(1 << ws281x_num);
}


