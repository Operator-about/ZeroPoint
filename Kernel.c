#include"Headers/Interrput.h"
#include"Headers/Kernel-modules.h"
#include"Headers/Ella.h"

//0 - QEMU
//1 - Qcom
//2 - Md
#define CPU 0
#define Debug 1

volatile struct UART* UART;
volatile struct GIC GIC;
volatile struct GIC_registers_data GIC_Registers;

int main(void){
    //write("Hello world from Zero Point! And Hello all! Welcom!");
    debugf("Hello world!");
}

int main_EL3(void){
#if CPU == 0
    GIC_common_configure(&GIC_Registers, GIC);

    UART_common_configure(UART);
    
     

#elif CPU == 1
    UART_qcom_configure();
#elif CPU == 2
    UART_md_configure();
#else
    #error "Error"          
#endif

    GICD_common_configure_EL1(GIC);
    write("Welcome!");
    EL3_to_EL1();
}

int main_EL1(void){
    zero_PSTATE();
    //GICD_common_configure_EL1(GIC);

    main();
}


