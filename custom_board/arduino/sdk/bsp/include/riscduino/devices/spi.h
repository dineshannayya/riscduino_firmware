// See LICENSE for license details.

#ifndef _RISCDUINO_SPI_H
#define _RISCDUINO_SPI_H

/* Register offsets */

#define SPI_REG_CTRL          0x00
#define SPI_REG_WDATA         0x04
#define SPI_REG_RDATA         0x08

/* Fields */

#define SPI_CTRL_CS_BIT(x)        ((x) & 0xFF)           // cs bit information
#define SPI_CTRL_CS_TIM(x)        (((x) & 0x1F) << 8)    // cs setup/hold period
#define SPI_CTRL_SCK(x)           (((x) & 0x3F) << 13)   // sck clock period
#define SPI_CTRL_TSIZE(x)         (((x) & 0x3) << 19)    // SPI transfer size
#define SPI_CTRL_OP(x)            (((x) & 0x3) << 21)    // SPI operation type , 0 - WR, 1 - RD, 2 - WR/RD
#define SPI_CTRL_TSEL(x)          (((x) & 0x3) << 23)    // target chip select
#define SPI_CTRL_BYTE_ENDIAN(x)   (((x) & 0x1) << 25)    // Byte Endian, 0 - little, 1 - Big
#define SPI_CTRL_MODE(x)          (((x) & 0x3) << 26)    // SPI Mode
#define SPI_CTRL_BIT_ENDIAN(x)    (((x) & 0x1) << 28)    // Bit Endian, 1 -> LSBFIRST or  0 -> MSBFIRST


#define SPI_CTRL_OP_REQ(x)    (((x) & 0x1) << 31)    // cpu request



/* Values */

#define SPI_CSMODE_AUTO         0
#define SPI_CSMODE_HOLD         2
#define SPI_CSMODE_OFF          3

#define SPI_DIR_TX_RX           2
#define SPI_DIR_RX              1
#define SPI_DIR_TX              0

#define SPI_LEN_0               0
#define SPI_LEN_1               1
#define SPI_LEN_2               2
#define SPI_LEN_3               3

#define SPI_BIT_ENDIAN_LITTEL   1
#define SPI_BIT_ENDIAN_BIG      0

#define SPI_BYTE_ENDIAN_LITTEL  0
#define SPI_BYTE_ENDIAN_BIG     1

#define SPI_CLOCK_DIV2          1
#define SPI_CLOCK_DIV4          3
#define SPI_CLOCK_DIV8          7
#define SPI_CLOCK_DIV16         15
#define SPI_CLOCK_DIV32         31
#define SPI_CLOCK_DIV64         63


#endif /* _RISCDUINO_SPI_H */
