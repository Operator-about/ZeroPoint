#include<Interrupts.h>


void send(){
    UARTPL011->UART_ICR = (1ULL << 5); //Сброс прерывания
    while(!(UARTPL011->UART_FR & (1ULL << 5)) && Tx_buffer.tail < Tx_buffer.head){
        Tx_buffer.tail++;
        UARTPL011->UART_DR = Tx_buffer.buffer[Tx_buffer.tail]; //Запись значения
    }
}

void receving(){
    while(!(UARTPL011->UART_FR & (1ULL << 4))){ //Вытаскивание значение из FIFO по Rx линии
        Rx_buffer.head++;
        Rx_buffer.buffer[Rx_buffer.head] = UARTPL011->UART_DR;
        if(Rx_buffer.buffer[Rx_buffer.head] == '\n'){ //Если символ из FIFO(DR) равен \n то выход из цикла
            Rx_buffer.end = 1;
            break;
        }
    }
    
}

void GIC_interrupts(){
    switch(GIC_version_check()){
        case 3:
            volatile uint64_t _IAR_ID_64;
            __asm__("MRS %0, ICC_IAR1_EL1" : "=r"(_IAR_ID_64));
            if(_IAR_ID_64 == 33){
                if(UARTPL011->UART_MIS & (1ULL << 5)){
                        send();
                }
                else if(UARTPL011->UART_MIS & (1ULL << 4)){
                    receving();
                }
            }
            //DIR используется из-за включённого в CTLR бита EOImode1NS
            __asm__("MSR ICC_EOIR1_EL1, %0" : : "r"(_IAR_ID_64)); //Завершение прерывание
            __asm__("MSR ICC_DIR_EL1, %0" : : "r"(_IAR_ID_64)); //Сброс прерывания
            break;
        case 2:
            volatile uint32_t _IAR_ID_32;
            _IAR_ID_32 |= (GICv2_.GICC->GICC_IAR << 0);
            if(_IAR_ID_32 == 33){
                if(UARTPL011->UART_MIS & (1ULL << 5)){
                    send();
                }
                else if(UARTPL011->UART_MIS & (1ULL << 4)){
                    receving();
                }
            }
            GICv2_.GICC->GICC_EOIR |= (_IAR_ID_32 << 0);
            GICv2_.GICC->GICC_DIR |= (_IAR_ID_32 << 0);
            break;
        default:
            break;
    }
}





