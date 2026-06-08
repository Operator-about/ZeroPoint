#include<table-handel.h>

void IRQh_handel(){
    switch(GIC_version_check()){
        case 2:
            volatile uint32_t _IARv2 = GICv2->GICC_IAR;
            if((_IARv2 & 0x3FF) < 1022){
                if((_IARv2 & 0x3FF) == 153){
                    if(*UART.UART_MIS & (1ULL << 5)){
                        send();
                    }
                    else if(*UART.UART_MIS & (1ULL << 4)){
                        receving();
                    }
                    else if(*UART.UART_MIS & (1ULL << 6)){
                        receving();
                    }
                }
            }  
            GICv2->GICC_EOIR = _IARv2;
            break;
        case 3:
            volatile uint64_t _IARv3;
            __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IARv3));
            if(_IARv3 < 1022){
                if(_IARv3 == 153){
                    if(*UART.UART_MIS & (1ULL << 5)){
                        send();
                    }
                    else if(*UART.UART_MIS & (1ULL << 4)){
                        receving();
                    }
                    else if(*UART.UART_MIS & (1ULL << 6)){
                        receving();
                    }
                }
            }
            __asm__("MSR ICC_EOIR1_EL1, %0" : :"r"(_IARv3));
            break;
        default:
            break;
    }
}
