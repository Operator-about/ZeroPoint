ENTRY(_start)

MEMORY{
    RAM(rwx) : ORIGIN = 0x40000000, LENGTH = 128M
}


SECTIONS{

    . = ORIGIN(RAM);

    .text : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    . = ALIGN(2048);
    .vectors : {
        KEEP(*(.vectors))
    } > RAM

    .rodata : {
        *(.rodata .rodata.*)
    } > RAM

    .data : {
        . = ALIGN(8);
        *(.data .data.*)
    } > RAM

    . = ALIGN(16);
    __bss_start = .;
    .bss : {
        *(bss. .bss.*)
        *(COMMON)
    } > RAM
    . = ALIGN(16);
    __bss_end = .;

    . = ALIGN(16);
    .stack (NOLOAD) : {
        . += 0x10000;
        _stack_top = .;
    } > RAM

    __end = .;
}