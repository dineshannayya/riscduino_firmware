/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

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

#include "Arduino.h"
#include "wiring_private.h"
#include "encoding.h"
#include "drivers/plic/plic_driver.h"

#include <string.h>

// This includes all interrupts, like Watchdog Timer and
// Real-Time Clock, not just external interrupts.
static voidFuncPtr callbacksInt[PLIC_NUM_INTERRUPTS];


/* Configure I/O interrupt sources */
static void __initialize()
{
  // Clear out all the callback registers
  memset(callbacksInt, 0, sizeof(callbacksInt));

  // PLIC Registers are not eset in general. Clear
  // them out.
  //PLIC_init(&g_plic,
  //	    PLIC_BASE_ADDR,
  //	    PLIC_NUM_INTERRUPTS,
  //	    PLIC_NUM_PRIORITIES);

  // Set the global PLIC interrupt.
  set_csr(mie, MIP_MEIP);
  set_csr(mtvec, 0); // Set Non-Vector Mode Interrupt
  set_csr(mstatus, MSTATUS_MIE); // Global Interrupt Enable

}

/*
 * \brief Specifies a named Interrupt Service Routine (ISR) to call when an interrupt occurs.
 *        Replaces any previous function that was attached to the interrupt.
 */
void attachInterrupt(uint32_t intnum, voidFuncPtr callback, uint32_t mode)
{
  static int enabled = 0;

  if (intnum > PLIC_NUM_INTERRUPTS) {
    return;
  }

  if (!enabled) {
    __initialize();
    enabled = 1;
  }

  // Assign callback to interrupt
  callbacksInt[intnum] = callback;

  // In general, you'd have to configure the device to raise an
  // interrupt. We make an exception for the GPIO pins and do the
  // configuration here.
  // In Riscduino intnum 8 to 32 is corresponds to gpio
  if (intnum >= INT_GPIO_BASE && intnum < (INT_GPIO_BASE + NUM_GPIO)) {
    switch (mode)
      {
      case LOW: // This feature not supported in Riscduino
	break;
      case HIGH: // This feature not supported in Riscduino
	break;
      case CHANGE: // Any Change
	GPIO_REG(GPIO_RISE_IE) |= (1 << intnum );
	GPIO_REG(GPIO_FALL_IE) |= (1 << intnum );
	GPIO_REG(GPIO_INTR_ENB) |= (1 << intnum);
	GLBL_REG(GLBL_INTR_ENB) |= (1 << intnum);
	break;
      case FALLING:
	GPIO_REG(GPIO_FALL_IE) |= (1 << intnum );
	GPIO_REG(GPIO_INTR_ENB) |= (1 << intnum );
	GLBL_REG(GLBL_INTR_ENB) |= (1 << intnum);
	break;
      case RISING:
	GPIO_REG(GPIO_RISE_IE) |= (1 << intnum );
	GPIO_REG(GPIO_INTR_ENB) |= (1 << intnum );
	GLBL_REG(GLBL_INTR_ENB) |= (1 << intnum );
	break;
      }
  } else { // Other then GPIO interrupt, config only GLBL
	  GLBL_REG(GLBL_INTR_ENB) |= (1 << intnum );
  }
  // Assuming single core case, interrupt enable for core0
  RISCV_REG(RISCV_CORE0_INTR_MASK) |= (1 << intnum );
  
  //write_csr(CSR_IPIC_IDX, intnum);
  write_csr(0xbf6, intnum);
  //write_csr(CSR_IPIC_ICSR, (IPIC_ICSR_IE | IPIC_ICSR_IM));
  write_csr(0xbf7, (IPIC_ICSR_IE | IPIC_ICSR_IM));

  // Enable the interrupt in the PLIC (External interrupts are already
  // globally enabled)
  //PLIC_enable_interrupt (&g_plic, intnum);

  // Threshold is assumed to be 0. We have to set the priority > threshold
  // to trigger the interrupt.
  //PLIC_set_priority(&g_plic, intnum, 1);
  
}

/*
 * \brief Turns off the given interrupt.
 */
void detachInterrupt(uint32_t intnum)
{
  
  //PLIC_disable_interrupt(&g_plic, intnum);
  //PLIC_set_priority(&g_plic, intnum, 0);

  // In general, you have to turn off the interrupt source
  // For GPIO, we do it here.
  if (intnum >= INT_GPIO_BASE  && intnum < (INT_GPIO_BASE + NUM_GPIO)) {
    GPIO_REG(GPIO_LOW_IE)  &= ~( 1 << intnum );
    GPIO_REG(GPIO_HIGH_IE) &= ~( 1 << intnum );
    GPIO_REG(GPIO_RISE_IE) &= ~( 1 << intnum );
    GPIO_REG(GPIO_FALL_IE) &= ~( 1 << intnum );
  } else { // Other then GPIO interrupt, config only GLBL
	  GLBL_REG(GLBL_INTR_ENB) &= ~(1 << intnum );
  }

}


/*Entry Point for PLIC Interrupt Handler*/
void handle_m_ext_interrupt(){

  // Start the Interrupt Processing
  //write_csr(CSR_IPIC_SOI, 0);
  write_csr(0xbf5, 0);
  
  //plic_source intnum  = PLIC_claim_interrupt(&g_plic);
  //plic_source intnum  = read_csr(CSR_IPIC_CISV);
  plic_source intnum  = read_csr(0xbf0);
  if ((intnum >=0 ) &&
      (intnum < PLIC_NUM_INTERRUPTS) &&
      callbacksInt[intnum] != 0) {
    callbacksInt[intnum]();
  }

  // Close the current interrupt processing 
  //write_csr(CSR_IPIC_EOI, 0);
  write_csr(0xbf4, 0);

  if (intnum >= INT_GPIO_BASE  && intnum < (INT_GPIO_BASE + NUM_GPIO)) {
	GPIO_REG(GPIO_INTR_CLR)  = (1 << intnum );
	GLBL_REG(GLBL_INTR_CLR)  = (1 << intnum);
  } else {
	GLBL_REG(GLBL_INTR_CLR)  = (1 << intnum);
  }

  //PLIC_complete_interrupt(&g_plic, intnum);
}

/* TODO: Entry Point for Timer Interrupt Handler*/
void handle_m_time_interrupt() {

  // Does nothing for now.
  
}

uintptr_t handle_trap(uintptr_t mcause, uintptr_t epc)
{
  // External Machine-Level interrupt from PLIC
  if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_EXT)) {
    handle_m_ext_interrupt();
  } else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_TIMER)){
    // Timer Interrupt
    handle_m_time_interrupt();
  }else {
    while(1) {};
  }
  return epc;
}
