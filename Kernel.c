#include"Headers/Interrput.h"

//0 - QEMU
//1 - Qcom
//2 - Md
#define CPU 0

volatile struct UART UART;
volatile struct GIC GIC;
volatile struct GIC_registers_data GIC_Registers;


int main(void){
#if CPU == 0
    GIC_common_configure(&GIC_Registers);

    check_UART_avaible_common(&UART);

    UART_common_configure(&UART);

    
     

#elif CPU == 1
    UART_qcom_configure();
#elif CPU == 2
    UART_md_configure();
#else
    #error "Error"          
#endif

    write("Hello world from Zero Point! And Hello all! Welcom!");

    while(1){
        
    }
}