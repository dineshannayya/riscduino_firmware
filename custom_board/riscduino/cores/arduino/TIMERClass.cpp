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
#include "TIMERClass.h"

#include "variant.h"

TIMERClass Timer(TIMER_BASE_ADDR);

TIMERClass::TIMERClass(uint32_t base_addr) {
    base = base_addr;
}




// Public Methods //////////////////////////////////////////////////////////////

void TIMERClass::begin() {

   // Configure 1us period based on CPU clock
   // Assumption CPU clock in MHZ
   TIMER_REG(TIMER_GLBL_CFG) = (F_CPU/1000000) -1;
 
}


void TIMERClass::enable(int timer_id, int source,int threshold) {
   // Register Bit decoding
   // [15:0]    - Timer Threshold
   // [16]      - Timer Enable
   // [18:17]   - Timer Source

   TIMER_REG(TIMER_TIMET0_CFG+(timer_id*4)) =  (source & 0x3) << 17 | 1 << 16 | ((threshold-1) & 0xFFFF);
 
}

void TIMERClass::disable(int timer_id) {
   TIMER_REG(TIMER_TIMET0_CFG+(timer_id*4)) =  0;
}


