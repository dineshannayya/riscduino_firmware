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

#ifndef _WS281X_CLASS_
#define _WS281X_CLASS_

#include "platform.h"

#define WS2811_LOW_SPEED   (0x00)
#define WS2811_HIGH_SPEED  (0x01)
#define WS2812_HIGH_SPEED  (0x02)
#define WS2812S_HIGH_SPEED (0x03)
#define WS2812B_HIGH_SPEED (0x04)


#define WS_SPEED_400KHZ (400000) // 400Khz
#define WS_SPEED_800KHZ (800000) // 800Khz

#define WS2811_LS_TOH   (2000000) // 2Mhz   - 0.5us
#define WS2811_LS_T1H   (833333)  // 833Khz - 1.2us

#define WS2811_HS_TOH   (4000000) // 4Mhz   - 0.25us
#define WS2811_HS_T1H   (1666667) // 1667Khz - 0.6us

#define WS2812_TOH   (2857142)    // 0.35us
#define WS2812_T1H   (1428571)    // 0.7us

#define WS2812S_TOH   (2857142)    // 0.35us
#define WS2812S_T1H   (1428571)    // 0.7us

#define WS2812B_TOH   (2857142)    // 0.35us
#define WS2812B_T1H   (1111111)    // 0.9us

#define WS281X_RST    (16667)      // 60us, need to be more that 50us    


class WS281XClass 
{
  public:
    WS281XClass(uint32_t base_addr = WS281X_BASE_ADDR);
    uint32_t base;
  
    void begin(uint8_t mode);
    void end(uint8_t mode);
    void enable(uint8_t pin);
    void disable(uint8_t pin);
    void fast_write(uint8_t pin,uint32_t data);
    void write(uint8_t pin,uint32_t data);
};

extern WS281XClass ws281x;


#endif 
