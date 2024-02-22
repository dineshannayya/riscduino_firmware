// See LICENSE for license details.

#ifndef _RISCDUINO_UART_H
#define _RISCDUINO_UART_H

/* Register offsets */
#define UART_REG_CTRL           0x00
#define UART_REG_INTR_STAT      0x04
#define UART_REG_BAUD_LSB       0x08  // Baud [7:0]
#define UART_REG_BAUD_MSB       0x0C  // Baud [11:8]
#define UART_REG_STATUS         0x10
#define UART_REG_TXFIFO         0x14
#define UART_REG_RXFIFO         0x18
#define UART_REG_TXFIFO_STATUS  0x1C
#define UART_REG_RXFIFO_STATUS  0x20
#define UART_REG_DIV            0x18

/* CTRL register */
#define UART_TXEN               0x1
#define UART_TXWM(x)            (((x) & 0xffff) << 16)

/* CTRL register */
#define UART_RXEN               0x2
#define UART_RXWM(x)            (((x) & 0xffff) << 16)

#define UART_TX_IFG(x)         ((x & 0x3) << 6)

#define UART_TX_IFGM           (0x3 << 6)  // IFG MASK


#endif /* _RISCDUINO_UART_H */
