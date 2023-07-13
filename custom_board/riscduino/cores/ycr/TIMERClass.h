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

#ifndef _TIMER_CLASS_
#define _TIMER_CLASS_

#include "platform.h"


#define timerToInterrupt(P) (INT_TIMER_BASE+P) 


class TIMERClass 
{
  public:
    TIMERClass(uint32_t base_addr = TIMER_BASE_ADDR);
    uint32_t base;
  
    void begin(void);
    void end(void);
    void enable(int timer_id, int source,int threshold);
    void disable(int timer_id);
};

#endif // _UART_CLASS_
