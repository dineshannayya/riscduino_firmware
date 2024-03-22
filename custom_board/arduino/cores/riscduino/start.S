// See LICENSE for license details.

#include "riscv_csr_encoding.h"
#include "sc_test.h"

# define LREG lw
# define SREG sw
# define REGBYTES 4

.altmacro
#define GLBL_CFG_MAIL_BOX              0x1002003C      // Mail Box for TB and RISCV Communication
#define TEST_START_CMD                 0x00000001      // Test Start Indication
#define WBI_CFG_DCG                    0x10030004      // Wishbone Interrconnect Dynamic Clock Gate Control
#define WBI_DCG_CMD                    0x00000001      // Dynamic Clock Gate Command
#define CONFIG_RV_BOOT_HART            0

	.section .init
	.globl _start
	.type _start,@function

    .text
    .org (64*4)

_start:
	la gp, _gp
	la sp, _sp
    addi  sp, sp, -4
# This piece of code allocate 0x200 ( 512 Byte out of 2KB) stack memory for each core
# core #0 uses 0x000600~0x007ff as stack
# core #1 uses 0x000400-0x005ff as stack
# core #2 uses 0x000200-0x003ff as stack
# core #3 uses 0x000000-0x001ff as stack
   lui   t0, 0
   addi  t0, t0, 0x200
   csrr  t1, mhartid
   mul   t2, t0, t1
   sub   sp, sp, t2


	csrr a0, mhartid
	li t0, CONFIG_RV_BOOT_HART
	beq a0, t0, boot_first_core
	j boot_secondary_core

boot_first_core:
	/* Load data section */
	la a0, _data_lma
	la a1, _data
	la a2, _edata
	bgeu a1, a2, 2f
1:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a1, a2, 1b
2:

	/* Clear bss section */
	la a0, __bss_start
	la a1, _end
	bgeu a0, a1, 2f
1:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, 1b
2:

	/* Call global constructors */
	la a0, __libc_fini_array
	call atexit
	call __libc_init_array

#ifndef __riscv_float_abi_soft
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrs mstatus, t0
	csrr t1, mstatus
	and t1, t1, t0
	beqz t1, 1f
	fssr x0
1:
#endif

    li  t0, GLBL_CFG_MAIL_BOX
    li  t1, TEST_START_CMD             // Set Text Start Command
    sw  t1, (t0)                       //send command to Mail Box Register
    /*
     Dynamic Clock :
      Need to See How to Enable the clock gating check 
     as this supported only from MPW-9 onwards

     li  t0, WBI_CFG_DCG
     li  t1, WBI_DCG_CMD               
      sw  t1, (t0)                       
     */

boot_secondary_core:
	/* argc = argv = 0 */
	li a0, 0
	li a1, 0
	call main
	tail exit
