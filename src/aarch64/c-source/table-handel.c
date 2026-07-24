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
                else if((_IARv2 & 0x3FF) == 158){
                    volatile uint32_t _local_DAT_buffer[8192];

                    for(int _clear = 0; _clear < 8192; _clear++){
                        _local_DAT_buffer[_clear] = 0x0;
                    }

                    for(int _index = 0; _index < 8192; _index++){
                        _local_DAT_buffer[_index] = SD_Registers->BDP_SD;
                    }
                    DAT_buffer = (uint8_t*)_local_DAT_buffer;

                    SD_Registers->NS_SD &= ~(1ULL << 5);
                    SD_Registers->NS_SD &= ~(1ULL << 1);   
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
