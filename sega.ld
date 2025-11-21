OUTPUT_ARCH(m68k)
SEARCH_DIR(.)
__DYNAMIC  =  0;

/* Define the memory regions of the Mega Drive */
MEMORY
{
    rom : ORIGIN = 0x00000000, LENGTH = 4M
    ram : ORIGIN = 0x00FF0000, LENGTH = 64K
}

SECTIONS
{
    /* Code and Read-Only Data go to ROM */
    .text :
    {
        *(.text)      /* The actual code */
        *(.rodata)    /* Constants */
        . = ALIGN(2); /* Align to 2 bytes (16-bit bus) */
    } > rom

    /* Initialized Data (Variables with values) */
    /* LOADED in ROM, but RUNS in RAM */
    .data : 
    {
        _data_start = .;
        *(.data)
        _data_end = .;
    } > ram AT > rom

    /* Uninitialized Data (Variables without values) */
    /* Runs in RAM */
    .bss :
    {
        _bss_start = .;
        *(.bss)
        *(COMMON)
        _bss_end = .;
    } > ram
}