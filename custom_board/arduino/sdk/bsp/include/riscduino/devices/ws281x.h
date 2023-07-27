
#ifndef _RISCDUINO_WS281X_H
#define _RISCDUINO_WS281X_H

#define WS281X_CMD          (0x00)  // COMMAND REG
#define WS281X_CFG0          (0x04)  // CONFIG-0
#define WS281X_CFG1          (0x08)  // CONFIG-1
#define WS281X_STATUS        (0x0C)  // STATUS

#define WS281X_FIFO_BASE     (0x10)  // TX DATA FIFO BASE
#define WS281X_FIFO_PORT0    (0x10)  // PORT-0 FIFO
#define WS281X_FIFO_PORT1    (0x14)  // PORT-1 FIFO
#define WS281X_FIFO_PORT2    (0x18)  // PORT-2 FIFO
#define WS281X_FIFO_PORT3    (0x1C)  // PORT-3 FIFO

#endif /* _RISCDUINO_WS281X_H */
