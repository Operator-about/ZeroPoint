/*
    Данный файл представляет из себя "склад" для хранения таблиц, SP.
*/
.global EL1h_SP_bottom
.global EL2h_SP_bottom

.section .bss
.align 15
EL2h_SP_top:
    .SPACE 32768
EL2h_SP_bottom:
.align 15
EL1h_SP_top:
    .SPACE 32768
EL1h_SP_bottom: