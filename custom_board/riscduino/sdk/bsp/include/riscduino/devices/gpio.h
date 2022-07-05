// See LICENSE for license details.

#ifndef _RISCDUINO_GPIO_H
#define _RISCDUINO_GPIO_H

#define GPIO_IDATA       (0x14)  // GPIO Data In
#define GPIO_ODATA       (0x18)  // GPIO Data Out
#define GPIO_DSEL        (0x1C)  // GPIO Direction Select
#define GPIO_TYPE        (0x20)  // GPIO TYPE - Static/Waveform
#define GPIO_INTR_STAT   (0x24)  // GPIO Interrupt status
#define GPIO_INTR_CLR    (0x24)  // GPIO Interrupt Clear
#define GPIO_INTR_SET    (0x28)  // GPIO Interrupt Set
#define GPIO_INTR_MASK   (0x2C)  // GPIO Interrupt Mask
#define GPIO_POS_INTR    (0x30)  // GPIO Posedge Interrupt
#define GPIO_NEG_INTR    (0x34)  // GPIO Neg Interrupt
#define GPIO_MULTI_FUNC  (0x38)  // GPIO Multi Function

// Need to clean-up - Dinesh A

#define GPIO_INPUT_VAL   (0x40)
#define GPIO_INPUT_EN    (0x40)
#define GPIO_OUTPUT_EN   (0x40)
#define GPIO_OUTPUT_VAL  (0x40)
#define GPIO_PULLUP_EN   (0x40)
#define GPIO_DRIVE       (0x40)
#define GPIO_RISE_IE     (0x40)
#define GPIO_RISE_IP     (0x40)
#define GPIO_FALL_IE     (0x40)
#define GPIO_FALL_IP     (0x40)
#define GPIO_HIGH_IE     (0x40)
#define GPIO_HIGH_IP     (0x40)
#define GPIO_LOW_IE      (0x40)
#define GPIO_LOW_IP      (0x40)
#define GPIO_IOF_EN      (0x40)
#define GPIO_IOF_SEL     (0x40)
#define GPIO_OUTPUT_XOR  (0x40)


#endif /* _RISCDUINO_GPIO_H */
