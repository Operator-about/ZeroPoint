#include<GIC.h>

void GICv3_registers_init(){
    GICv3_affinity_router();

    GICv3_registers.CTLR |= (1ULL << 6) | (1ULL << 1);
    GICv3_registers.PMR = 0xFF;
    GICv3_registers.GROUP1ENABLE |= (1ULL << 0);
    GICv3_registers.SRE |= (1ULL << 0);

    __asm__("MSR ICC_CTLR_EL1, %0" : :"r"(GICv3_registers.CTLR));
    __asm__("MSR ICC_PMR_EL1, %0" : :"r"(GICv3_registers.PMR));
    __asm__("MSR ICC_SRE_EL1, %0" : :"r"(GICv3_registers.SRE));
    __asm__("MSR ICC_IGRPEN1_EL1, %0" : :"r"(GICv3_registers.GROUP1ENABLE));
    __asm__("ISB");
}

void GICDv3_init(){
    GICv3_.GICD->GICD_CTLR |= (1ULL << 4) | (1ULL << 1);
    for(int _interrupts_index = 32; _interrupts_index < 128; _interrupts_index++){
        GICv3_.GICD->GICD_IGROUPR[_interrupts_index / 32] |= (1ULL << (_interrupts_index % 32));
        GICv3_.GICD->GICD_IGRPMODR[_interrupts_index / 32] &= ~(1ULL << (_interrupts_index % 32));
        GICv3_.GICD->GICD_ICFGR[_interrupts_index / 16] |= (1ULL << ((_interrupts_index % 4) * 2 + 1));
        GICv3_.GICD->GICD_IROUTER[_interrupts_index] |= (GICv3_registers.AFF[0] << 0) | (GICv3_registers.AFF[1] << 8) | (GICv3_registers.AFF[2] << 16) | (GICv3_registers.AFF[3] << 32);
        GICv3_.GICD->GICD_IROUTER[_interrupts_index] &= ~(1ULL << 31);
        GICv3_.GICD->GICD_IPRIORITYR[_interrupts_index / 4] |= (1ULL << (_interrupts_index % 4));
        GICv3_.GICD->GICD_ISENABLER[_interrupts_index / 32] |= (1ULL << (_interrupts_index % 32)); 
    }
}

void GICRv3_init(){
    GICv3_.GICR[0]->GICR_WAKER &= ~(1ULL << 1);
    while(GICv3_.GICR[0]->GICR_WAKER & (1ULL << 2)){
        __asm__("NOP");
    }
}

void GICv3_affinity_router(){
    //Получение полного Aff для IROUTER
    volatile uint64_t _aff_local; //Локальная переменная для хранения временного Aff значения
    volatile int _LSR = 0;
    for(int _index = 0; _index < 4; _index++){
        __asm__ volatile("MRS %0, MPIDR_EL1" : "=r"(_aff_local)); //Получение из MPDIR_EL1 параметров ядра
        __asm__ volatile("LSR %0, %0, %0" : "=r"(_aff_local) : "r"(_aff_local), "r"(_LSR));
        __asm__ volatile("AND %0, %0, 0xFF" : "=r"(_aff_local) : "r"(_aff_local)); //Извлечение
        GICv3_registers.AFF[_index] = _aff_local; //Получение ядра для Aff
        _LSR+=8;
    }
}