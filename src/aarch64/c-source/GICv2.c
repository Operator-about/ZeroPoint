#include<GIC.h>

void GICCv2_init(){
    GICv2_.GICC->GICC_CTLR |= (1ULL << 1) | (1ULL << 9);
    GICv2_.GICC->GICC_PMR |= (0xFF << 0);
}

void GICDv2_init(){
    GICv2_.GICD->GICD_CTLR |= (1ULL << 1);
    for(int _interrupts_index = 32; _interrupts_index < 128; _interrupts_index++){
        GICv2_.GICD->GICD_IGROUER[_interrupts_index / 32] |= (1ULL << (_interrupts_index % 32));
        GICv2_.GICD->GICD_ICFGR[_interrupts_index / 16] &= ~(1ULL << ((_interrupts_index % 16) * 2 + 1));
        GICv2_.GICD->GICD_ITARGETSR[_interrupts_index / 4] |= (0x01 << ((_interrupts_index % 4) * 8));
        GICv2_.GICD->GICD_IPRIORITYR[_interrupts_index / 4] |= (1ULL << ((_interrupts_index % 4) * 8));
        GICv2_.GICD->GICD_ISENABLER[_interrupts_index / 32] |= (1ULL << (_interrupts_index % 32));
    }
}