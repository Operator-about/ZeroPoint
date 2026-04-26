#include<Interrupts.h>
#include<Kernel-modules.h>
#include<Ella.h>
#include<GIC.h>
#include<MMU.h>
#include<UART.h>

struct UART* UARTPL011;
struct GICv3 GICv3_;
struct GICv2 GICv2_;
struct GICv3_registers_data GICv3_registers;

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
        clear_buffer(_keyboard_buffer_input);
        __asm__("NOP");
    }
}

int main_EL3(void){
    
    Rx_clear();
    if(GIC_version_check() == 2){
        GICv2_.GICC = (struct GICCv2*)0x08010000;
        GICv2_.GICD = (struct GICDv2*)0x08000000;
        GICDv2_init();
        GICCv2_init();
    }
    else if(GIC_version_check() == 3){
        GICv3_registers_init(); //Настройка регистров GIC
        GICv3_.GICD = (struct GICDv3*)0x08000000; //Указание начало GICD
        GICv3_.GICR[0] = (struct GICRv3*)0x080A0000; //Указание начало GICR
        GICDv3_init(); //Настройка GIC для EL3
        GICRv3_init();
    }

    UARTPL011 = (struct UART*)0x09000000; //Указание начало UART
    UARTPL011_init(); //Настройка UART

    EL3_to_EL1(); //Переход в EL1h NS
}

int main_EL1(void){
    MMU_init();
    VBAR_set();
    main();
}


