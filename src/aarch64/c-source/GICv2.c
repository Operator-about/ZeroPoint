#include<GIC.h>

void GICCv2_init(){
    GICv2_.GICC->GICC_CTLR |= (1ULL << 1) | (1ULL << 9);
    GICv2_.GICC->GICC_PMR |= (0xFF << 0);
}

void GICDv2_init(){
    GICv2_.GICD->GICD_CTLR |= (1ULL << 1);
    GICv2_.GICD->GICD_IGROUER[33 / 32] |= (1ULL << (33 % 32));
    GICv2_.GICD->GICD_ICFGR[33 / 16] &= ~(1ULL << ((33 % 16) * 2 + 1));
    GICv2_.GICD->GICD_ITARGETSR[33 / 4] |= (0x01 << ((33 % 4) * 8));
    GICv2_.GICD->GICD_IPRIORITYR[33 / 4] |= (1ULL << ((33 % 4) * 8));
    GICv2_.GICD->GICD_ISENABLER[33 / 32] |= (1ULL << (33 % 32));
}