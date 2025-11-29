.section .text
    .global _start

    /* --- 1. Vector Table --- */
    .long   0x00FFE000      /* Stack Pointer */
    .long   _start          /* Entry Point */

    /* Interrupt Vectors (Placeholders to prevent crashes on unexpected interrupts) */
    .long   _start, _start, _start, _start, _start, _start, _start, _start
    .long   _start, _start, _start, _start, _start, _start, _start, _start
    .long   _start, _start, _start, _start, _start, _start, _start, _start
    .long   _start, _start, _start, _start, _start, _start, _start, _start

    /* --- 2. SEGA Header (Starts at 0x100) --- */
    /* Must be exactly at this offset for the console to recognize it */
    .org    0x100
    .ascii  "SEGA GENESIS    "  /* Console Name */
    .ascii  "(C) YOU 2024    "  /* Copyright */
    .ascii  "BARE METAL GAME                                 " /* Domestic Name */
    .ascii  "BARE METAL GAME                                 " /* Overseas Name */
    .ascii  "GM 00000000-00"    /* Serial Number */
    .word   0x0000              /* Checksum (Ignored by most emulators if TMSS passes) */
    .ascii  "J               "  /* Device Support */
    .long   0x00000000, 0x003FFFFF /* ROM Start/End */
    .long   0x00FF0000, 0x00FFFFFF /* RAM Start/End */
    .ascii  "RA"                /* SRAM Type */
    .word   0xF820              /* SRAM Support */
    .long   0x00200000, 0x00203FFF /* SRAM Start/End */
    .ascii  "            "      /* Modem Support */
    .ascii  "        "          /* Memo */
    .ascii  "JUE             "  /* Region (Japan/USA/Europe) */

_start:
    /* --- 3. Disable Interrupts --- */
    move.w  #0x2700, %sr

    /* --- 4. TMSS Security Logic --- */
    move.b  0xA10001, %d0
    andi.b  #0x0F, %d0
    beq.s   skip_tmss
    move.l  #0x53454741, 0xA14000

skip_tmss:
    /* --- 5. Jump to Main --- */
    jsr     main

loop:
    bra.s   loop

