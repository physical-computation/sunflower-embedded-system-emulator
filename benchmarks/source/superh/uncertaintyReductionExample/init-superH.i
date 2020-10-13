# 1 "init-superH.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "init-superH.S"
# 1 "asm.h" 1
# 2 "init-superH.S" 2


 .global ___errno
 .global _sleep

 .align 2
start:

 AND #0, r0
 LDC r0, sr



 MOV.L stack_addr, r15
 MOV.L start_addr, r0
 JSR @r0
 NOP


 mov #1, r4
 trapa #34


  .align 2

stack_addr:
 .long (0x8001000 + 1<<26)
start_addr:
 .long _main

___errno:
 .long 0

_sleep:
 SLEEP
 RTS
 NOP
