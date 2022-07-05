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
#include "UARTClass.h"

#include "variant.h"

UARTClass Serial;

int
UARTClass::sio_probe_rx()
{
  int32_t c;

  if ((c = UART_REG(UART_REG_RXFIFO)) >= 0) {
    sio_rxbuf[sio_rxbuf_head++] = c;
    sio_rxbuf_head &= SIO_RXBUFMASK;
    return(1);
  }
  return(0);
}


int
UARTClass::sio_getchar(int blocking)
{
  int c, busy;

  do {
    sio_probe_rx();
    busy = (sio_rxbuf_head == sio_rxbuf_tail);
  } while (blocking && busy);

  if (busy)
    return (-1);
  c = sio_rxbuf[sio_rxbuf_tail++];
  sio_rxbuf_tail &= SIO_RXBUFMASK;
  return (c);
}

int
UARTClass::sio_putchar(char c, int blocking)
{
  volatile uint32_t *val = UART_REGP(UART_REG_STATUS);
  uint32_t busy = (*val) & 0x00000001;
  if (blocking) {
    while (*val & 0x00000001);
  } else if (busy) {
      return 1;
  }
  UART_REG(UART_REG_TXFIFO) = c;
  return 0; 
}



/****************************************************************
// 16x Baud clock generation
//  Baud Rate config = (F_CPU / (BAUD * 16)) - 2 
// Example: to generate 19200 Baud clock from 50Mhz Link clock
//    cfg_baud_16x = ((50 * 1000 * 1000) / (19200 * 16)) - 2
//    cfg_baud_16x = 0xA0 (160)
****************************************************************/

void
UARTClass::sio_setbaud(int bauds)
{

  uint32_t F_Baud; 
  F_Baud = (F_CPU/(bauds * 16)) - 2;

  UART_REG(UART_REG_BAUD_LSB) = F_Baud & 0xFF;
  UART_REG(UART_REG_BAUD_MSB) = (F_Baud >> 8) & 0x0F;
 
}


// Public Methods //////////////////////////////////////////////////////////////

void
UARTClass::begin(unsigned long bauds)
{
  GPIO_REG(GPIO_MULTI_FUNC) |= IOF0_UART0_ENB;

  UART_REG(UART_REG_CTRL) |= UART_TXEN;
  UART_REG(UART_REG_CTRL) |= UART_RXEN;

  sio_setbaud(bauds);
}


void
UARTClass::end(void)
{
  GPIO_REG(GPIO_MULTI_FUNC)    &= ~IOF0_UART0_ENB;

  UART_REG(UART_REG_CTRL) &= ~UART_TXEN;
  UART_REG(UART_REG_CTRL) &= ~UART_RXEN;

}


int
UARTClass::available(void)
{

  sio_probe_rx();
  return (!(sio_rxbuf_head == sio_rxbuf_tail));
}


int
UARTClass::availableForWrite(void)
{
  int busy;
  busy = (((int32_t)UART_REG(UART_REG_STATUS) & 0x1) == 0x1);
  return (!busy);
}


int
UARTClass::peek(void)
{
  sio_probe_rx();
  if (sio_rxbuf_tail == sio_rxbuf_head)
    return (-1);
  else
    return (sio_rxbuf[sio_rxbuf_tail]);
}


int
UARTClass::read(void)
{

  return (sio_getchar(1));
}


void
UARTClass::flush(void)
{
}


size_t
UARTClass::write(const uint8_t uc_data)
{

  sio_putchar(uc_data, 1);
  return (1);
}
