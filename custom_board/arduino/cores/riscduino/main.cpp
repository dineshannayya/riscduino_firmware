/*
  main.cpp - Main loop for Arduino sketches
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#define ARDUINO_MAIN
#include "Arduino.h"
#include "encoding.h"
#include "fe300prci/fe300prci_driver.h"

extern uint32_t trap_entry;

#define cmb() __asm__ __volatile__ ("" ::: "memory")

uint32_t mtime_lo(void)
{
  return *(volatile uint32_t *)(CLINT_BASE_ADDR + CLINT_MTIME);
}



static void riscduino_clock_setup () {

  // This is a very coarse parameterization. To revisit in the future
  // as more chips and boards are added.
   
}

void riscduino_specific_initialization(void)
{

  write_csr(mtvec, &trap_entry);
  if (read_csr(misa) & (1 << ('F' - 'A'))) { // if F extension is present
    write_csr(mstatus, MSTATUS_FS); // allow FPU instructions without trapping
    write_csr(fcsr, 0); // initialize rounding mode, undefined at reset
  }
  
  riscduino_clock_setup();

  // Disable TAP, as these TAP used io[3:0]. If user want to using it as gpio
  //GLBL_REG(GLBL_MULTI_FUNC) &= ~IOF0_TAP_ENB; // Disable Master UART
  
}

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
  //	init();
  // Compute F_CPU inverse, used for millis and micros functions.
  calc_inv(F_CPU/1000, &f_cpu_1000_inv);
  calc_inv(F_CPU/1000000, &f_cpu_1000000_inv);
  riscduino_specific_initialization();
  #if defined(RISCDUINO_SIM)
  #else
  delay(2000); // add 2 second delay to uart master to take over programming
  #endif
  setup();
  
  do {
    loop();
    if (serialEventRun)
      serialEventRun();
  } while (1);
  
  return 0;
}
