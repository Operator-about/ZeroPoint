/*
    Данный файл представляет из себя "склад" для хранения таблиц, SP.
*/
.global L0_table
.global L1_table
.global L2_table
.global EL1h_SP_top
.global EL3h_SP_top

.section .data
.align 12
L0_table:
    .SPACE 4096
.align 12
L1_table:
    .SPACE 4096
.align 12
L2_table:
    .SPACE 4096

.section .bss
.align 15
EL1h_SP_top:
    .SPACE 32768
EL1h_SP_bottom:
.align 15
EL3h_SP_top:
    .SPACE 32768
EL3h_SP_bottom: