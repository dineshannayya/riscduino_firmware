// See LICENSE for license details.

#ifndef _RISCDUINO_GLBL_H
#define _RISCDUINO_GLBL_H

#define GLBL_CHIP_ID        (0x00)  // CHIP OD
#define GLBL_CFG0           (0x04)  // Global Config-0
#define GLBL_CFG1           (0x08)  // Global Config-1
#define GLBL_INTR_ENB       (0x0C)  // Global Interrupt Enable
#define GLBL_INTR_STAT      (0x10)  // Global Interrupt Status
#define GLBL_INTR_CLR       (0x10)  // Global Interrupt Clear
#define GLBL_MULTI_FUNC     (0x14)  // GPIO Multi Function
#define GLBL_CLK_CTRL       (0x18)  // RTC/USB clock control;

#define GLBL_PAD_STRAP      (0x30)  // Strap as seen in PAD
#define GLBL_STRAP_STICKY   (0x34)  // Sticky strap used in soft reboot
#define GLBL_SYSTEM_STRAP   (0x38)  // System Strap
#define GLBL_MAIL_BOX       (0x3C)  // Mail Box Register

#define GLBL_SOFT_REG0      (0x40)  // Software Reg-0
#define GLBL_SOFT_REG1      (0x44)  // Software Reg-1
#define GLBL_SOFT_REG2      (0x48)  // Software Reg-2
#define GLBL_SOFT_REG3      (0x4C)  // Software Reg-3
#define GLBL_SOFT_REG4      (0x50)  // Software Reg-4
#define GLBL_SOFT_REG5      (0x54)  // Software Reg-5
#define GLBL_SOFT_REG5      (0x58)  // Software Reg-6
#define GLBL_SOFT_REG5      (0x5C)  // Software Reg-7



#endif /* _RISCDUINO_GLBL_H */
