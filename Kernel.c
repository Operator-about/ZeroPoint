#include"Headers/Interrput.h"
#include"Headers/Kernel-modules.h"
#include"Headers/Ella.h"

//0 - QEMU
//1 - Qcom
//2 - Md
#define CPU 0

volatile struct UART* UART;
volatile struct GIC GIC;
struct GIC_registers_data GIC_Registers;

int main(void){
    write("Welcome to ZeroPoint!");
    while(1){
        char a[100];
        read(a);
        if(a == "Hello"){
            write("Hello");
            __asm__("MOV X19, #40");
            break;
        }
    }
    while(1){
        __asm__("NOP");
    }
}

int main_EL3(void){
#if CPU == 0
    GIC_common_configure_registers(&GIC_Registers); //Настройка регистров GIC
    GIC.GICD = (struct GICD*)0x08000000; //Указание начало GICD
    GIC.GICR[0] = (struct GICR*)0x080A0000; //Указание начало GICR
    GICD_common_configure_EL3(&GIC_Registers); //Настройка GIC для EL3
    UART = (struct UART*)0x09000000; //Указание начало UART
    UART_common_configure(); //Настройка UART
#elif CPU == 1
    UART_qcom_configure();
#elif CPU == 2
    UART_md_configure();
#else
    #error "Error"          
#endif
    EL3_to_EL1(); //Переход в EL1h NS
}

int main_EL1(void){
    main();
}


