ENTRY(start)

<<<<<<< Updated upstream

SECTIONS{

    . = 0x02000000;

    .text : {
        KEEP(*(.text.boot))
        *(.text .text.*)
=======
SECTIONS{

    . = 0x10000000;

    .text : {
        *(.text.boot)
        *(.text)
>>>>>>> Stashed changes
    }

    . = ALIGN(2048);
    .vectors : {
<<<<<<< Updated upstream
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
=======
        *(.vectors)
    }

    .bss : {
        *(.bss)
>>>>>>> Stashed changes
    }
}