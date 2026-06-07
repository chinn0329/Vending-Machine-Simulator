;/*****************************************************************************/
;/* startup_LPC21xx.s: Startup file for Philips LPC21xx                       */
;/*****************************************************************************/

                PRESERVE8
                THUMB

; Stack and Heap sizes
Stack_Size      EQU     0x00000200
Heap_Size       EQU     0x00000000

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp
                DCD     Reset_Handler

                AREA    |.text|, CODE, READONLY

Reset_Handler   PROC
                EXPORT  Reset_Handler [WEAK]
                IMPORT  __main
                LDR     R0, =__main
                BX      R0
                ENDP

                ALIGN
                END