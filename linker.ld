ENTRY(start)


SECTIONS{

    . = 0x02000000;

    .text : {
        KEEP(*(.text.boot))
        *(.text .text.*)
    }

    . = ALIGN(2048);
    .vectors : {
        KEEP(*(.vectors))
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        . = ALIGN(8);
        *(.data .data.*)
    }

    . = ALIGN(16);
    .bss : {
        *(bss. .bss.*)
    }
}