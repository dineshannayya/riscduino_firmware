// See LICENSE for license details.

#ifndef _RISCDUINO_WIRE_H
#define _RISCDUINO_WIRE_H

/* Register offsets */

#define WIRE_REG_BITRATE_L     0x00
#define WIRE_REG_BITRATE_H     0x04
#define WIRE_REG_CTRL          0x08
#define WIRE_REG_WDATA         0x0C
#define WIRE_REG_RDATA         0x0C
#define WIRE_REG_CMD           0x10
#define WIRE_REG_STATUS        0x10

/* Fields */

#define WIRE_CMD_IDIS(x)          ((x) & 0x1)          // Disable Interrupt
#define WIRE_CMD_ACK(x)           (((x) & 0x1) << 3)   // Transmit Ack, 0 - Ack, 1 - NACK
#define WIRE_CMD_WR(x)            (((x) & 0x1) << 4)   // Write
#define WIRE_CMD_RD(x)            (((x) & 0x1) << 5)   // READ
#define WIRE_CMD_STOP(x)          (((x) & 0x1) << 6)   // STOP
#define WIRE_CMD_START(x)         (((x) & 0x1) << 7)   // START

#define WIRE_CTRL_ENB(x)            (((x) & 0x1) << 7)   // Core Enable
#define WIRE_CTRL_IEN(x)            (((x) & 0x1) << 6)   // I2C Interrupt Enabled

#define WIRE_STAT_RACK(x)          (((x) & 0x1) << 7)   // Receive ACK
#define WIRE_STAT_CMD_BUSY(x)      (((x) & 0x1) << 6)   // I2C CMD Busy
#define WIRE_STAT_AL(x)            (((x) & 0x1) << 5)   // Arbitration Lost
#define WIRE_STAT_FSM_BUSY(x)      (((x) & 0x1) << 4)   // I2C FSM Busy Indication
#define WIRE_STAT_TIP(x)           (((x) & 0x1) << 1)   // Transfer in Progress
#define WIRE_STAT_IRQ(x)           ((x) & 0x1)          // Interrupt




#endif /* _RISCDUINO_WIRE_H */
