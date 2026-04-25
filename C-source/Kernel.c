#include<Interrupts.h>
#include<Kernel-modules.h>
#include<Ella.h>
#include<MMU.h>

volatile struct UART* UART;
volatile struct GIC GIC;
struct GIC_registers_data GIC_Registers;

int main(void){
    write("Welcome to ZeroPoint!\r\n");
    char _keyboard_buffer_input[100];
    while(1){
        read(_keyboard_buffer_input);
        Rx_clear();
        if(compare_s(_keyboard_buffer_input, "About") == 1){
            write("ZeroPoint\r\nVersion - 0.0.1\r\nStatus - Power On \r\n");
        }
        else{
            write("Unknow command\r\n");
        }
        __asm__("NOP");
    }
}

int main_EL3(void){
    
    Rx_clear();
    GIC_common_configure_registers(&GIC_Registers); //Настройка регистров GIC
    GIC.GICD = (struct GICD*)0x08000000; //Указание начало GICD
    GIC.GICR[0] = (struct GICR*)0x080A0000; //Указание начало GICR
    GICD_common_configure_EL3(&GIC_Registers); //Настройка GIC для EL3
    UART = (struct UART*)0x09000000; //Указание начало UART
    UART_common_configure(); //Настройка UART

    EL3_to_EL1(); //Переход в EL1h NS
}

int main_EL1(void){
    MMU_init();
    VBAR_set();
    main();
}


