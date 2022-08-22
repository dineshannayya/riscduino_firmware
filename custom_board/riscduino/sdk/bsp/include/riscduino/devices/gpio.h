// See LICENSE for license details.

#ifndef _RISCDUINO_GPIO_H
#define _RISCDUINO_GPIO_H

#define GPIO_DSEL        (0x00)  // GPIO Direction Select
#define GPIO_TYPE        (0x04)  // GPIO TYPE - Static/Waveform
#define GPIO_IDATA       (0x08)  // GPIO Data In
#define GPIO_ODATA       (0x0C)  // GPIO Data Out
#define GPIO_INTR_STAT   (0x10)  // GPIO Interrupt status
#define GPIO_INTR_CLR    (0x10)  // GPIO Interrupt Clear
#define GPIO_INTR_SET    (0x14)  // GPIO Interrupt Set
#define GPIO_INTR_ENB    (0x18)  // GPIO Interrupt Enabled
#define GPIO_RISE_IE     (0x1C)  // GPIO Posedge Interrupt
#define GPIO_FALL_IE     (0x20)  // GPIO Neg Interrupt

// Need to clean-up - Dinesh A

#define GPIO_INPUT_VAL   (0x40)
#define GPIO_INPUT_EN    (0x40)
#define GPIO_OUTPUT_EN   (0x40)
#define GPIO_OUTPUT_VAL  (0x40)
#define GPIO_PULLUP_EN   (0x40)
#define GPIO_DRIVE       (0x40)
#define GPIO_RISE_IP     (0x40)
#define GPIO_FALL_IP     (0x40)
#define GPIO_HIGH_IE     (0x40)
#define GPIO_HIGH_IP     (0x40)
#define GPIO_LOW_IE      (0x40)
#define GPIO_LOW_IP      (0x40)
#define GPIO_IOF_EN      (0x40)
#define GPIO_IOF_SEL     (0x40)
#define GPIO_OUTPUT_XOR  (0x40)


#endif /* _RISCDUINO_GPIO_H */
