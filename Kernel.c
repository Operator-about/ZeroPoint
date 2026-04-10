#include"Headers/Interrput.h"
#include"Headers/Kernel-modules.h"
#include"Headers/Ella.h"

//0 - QEMU
//1 - Qcom
//2 - Md
#define CPU 0

volatile struct UART* UART;
volatile struct GIC GIC;
volatile struct GIC_registers_data GIC_Registers;

int main(void){
    //debugf("Hello world!");
}

int main_EL3(void){
#if CPU == 0

    GIC_common_configure(&GIC_Registers);
    
     

#elif CPU == 1
    UART_qcom_configure();
#elif CPU == 2
    UART_md_configure();
#else
    #error "Error"          
#endif
    //EL3_preparing_for_interrput();
    GICD_common_configure_EL3(&GIC_Registers);
    //GICD_common_configure_EL1();
    //write("Hello world!");
    if(GIC.GICD->GICD_IGROUPR[33 / 32] & (1ULL << (33 % 32))){
        //debugf("33 interrupt - access granted!");
    }
    if(GIC.GICD->GICD_ISENABLER[33 / 32] & (1ULL << (33 % 32))){
        //debugf("33 interrupt - enabler!");
    }
    UART_common_configure(UART);  
#if DEBUG == 1
    //debugf("This EL3 level status - okay!");
#endif
    //__asm__ volatile("MOV PC, X0");
    write("Hello world!");
    //debugf("This EL3 level status - okay!");

    EL3_to_EL1();
}

int main_EL1(void){
    //zero_PSTATE();
    //GICD_common_configure_EL1(GIC);

    main();
}


