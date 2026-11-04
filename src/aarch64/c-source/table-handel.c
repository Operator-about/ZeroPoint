#include<table-handel.h>

void IRQh_handel(){
    __asm__("MSR DAIFSet, #2");

    switch(GIC_version_check()){
        case 2:
            volatile uint32_t _IARv2 = GICv2->GICC_IAR;
            if((_IARv2 & 0x3FF) < 1022){
                if((_IARv2 & 0x3FF) == OutJump->UART_ID){
                    if(OutJump->UART_Standart == 0x504C00B0){
                        UART.UARTF->IRQ_handel();
                    }
                }
                else if((_IARv2 & 0x3FF) == OutJump->SD_ID){
                    if(OutJump->SD_Standart == 0x5354414E){
                        SD.SDF->IRQ_read();
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
                        UART.UARTF->IRQ_handel();
                    }
                }
                else if(_IARv3 == OutJump->SD_ID){
                    if(OutJump->SD_Standart == 0x5354414E){
                        SD.SDF->IRQ_read();
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
    uint64_t _resone = 0x0;
    __asm__("MRS %0, ESR_EL1" : "=r"(_resone));

    switch((_resone >> 21) & 0x3F){
        case 0x15:
            SVC_handel();
            break;
        default:
            SyncE_handel();
            break;
    }
}

void SVC_handel(){
    uint64_t _number_SVC = 0x0;
    uint64_t _arg[5];
    __asm__("MOV %0, X8" : "=r"(_number_SVC));
    
    switch(_number_SVC){
        case 0: //write
            __asm__("MOV %0, X0" : "=r"(_arg[0]));
            print((uint8_t*)_arg[0]);
            break;
        case 1: //read
            __asm__("MOV %0, X0" : "=r"(_arg[0]));    
            input((uint8_t*)_arg[0]);
            break;
        case 10: //open
            __asm__("MOV %0, X0" : "=r"(_arg[0]));
            CurrentIndex = 0;
            open((uint8_t*)_arg[0]);
            CurrentIndex = 0;
            break;
        case 20:
            __asm__("MOV X27, %0" : :"r"(&shell_init));
            break;
        default:
            break;
    }
}

void SyncE_handel(){
    uint64_t _ESR;
    uint64_t _FAR;
    uint64_t _ELR;

    __asm__("MRS %0, ESR_EL1" : "=r"(_ESR));
    __asm__("MRS %0, FAR_EL1" : "=r"(_FAR));
    __asm__("MRS %0, ELR_EL1" : "=r"(_ELR));

    printh64(_ESR);
    sec_barrier(50);
    printh64(_FAR);
    sec_barrier(50);
    printh64(_ELR);
    sec_barrier(50);

    sec_barrier(500);
}