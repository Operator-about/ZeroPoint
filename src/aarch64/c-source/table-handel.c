#include<table-handel.h>

void IRQh_handel(){
    __asm__("MSR DAIFSet, #2");

    switch(GIC_version_check()){
        case 2:
            volatile uint32_t _IARv2 = GICv2->GICC_IAR;
            if((_IARv2 & 0x3FF) < 1022){
                if((_IARv2 & 0x3FF) == OutJump->UART_ID){
                    if(OutJump->UART_Standart == 0x504C00B0){
                        irq_switch_pl011();
                    }
                }
                else if((_IARv2 & 0x3FF) == OutJump->SD_ID){
                    if(SD_Registers->NS_SD & (1ULL << 5)){
                        IRQ_read();
                        SD_Registers->NS_SD |= (1ULL << 5);
                        SD_Registers->NS_SD |= (1ULL << 1);
                    }
                    else if(SD_Registers->NS_SD & (1ULL << 4)){
                        //Тут будет функция, често-честно ^_^
                    }
                }
            }
            GICv2->GICC_EOIR = _IARv2;
            _IARv2 = 0x0;
            break;
        case 3:
            volatile uint64_t _IARv3;
            __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IARv3));
            if(_IARv3 < 1022){
                if(_IARv3 == OutJump->UART_ID){
                    if(OutJump->UART_Standart == 0x504C00B0){
                        irq_switch_pl011();
                    }
                }
            }
            __asm__("MSR ICC_EOIR1_EL1, %0" : :"r"(_IARv3));
            break;
        default:
            break;
    }

    __asm__("MSR DAIFClr, #2");
}

void Synch_handel(){
    uint64_t ESR = 0x0;
    uint64_t FAR = 0x0;
    uint64_t ELR = 0x0;
    uint64_t PC = 0x0;

    __asm__("MRS %0, ESR_EL1" : "=r"(ESR));
    __asm__("MRS %0, FAR_EL1" : "=r"(FAR));
    __asm__("MRS %0, ELR_EL1" : "=r"(ELR));

    printh64(ESR);
    printh64((ESR >> 26));
    printh64(FAR);
    printh64(ELR);

    sec_barrier(100);
}
