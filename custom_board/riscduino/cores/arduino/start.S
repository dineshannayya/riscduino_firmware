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

	.section .init
	.globl _start
	.type _start,@function

    .text
    .org (64*4)

_start:
	la gp, _gp
	la sp, _sp

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
    // Dynamic Clock 
    li  t0, WBI_CFG_DCG
    li  t1, WBI_DCG_CMD               
    sw  t1, (t0)                       

	/* argc = argv = 0 */
	li a0, 0
	li a1, 0
	call main
	tail exit
