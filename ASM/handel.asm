.global synchronous_elxh_handel
.global irq_handel_elxh_handel

synchronous_elxh_handel:
    MOV X5, #1ULL
    ERET

irq_handel_elxh_handel:
    