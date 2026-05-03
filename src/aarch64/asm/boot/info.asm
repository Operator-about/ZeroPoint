/*
    Данный файл представляет из себя "склад" для хранения таблиц, SP.
*/
.global EL1h_SP_top

.section .bss
.align 15
EL1h_SP_top:
    .SPACE 32768
EL1h_SP_bottom: