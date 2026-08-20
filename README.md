# **ZeroPoint - Операционная Система**
**Небольшое AArch64 ядро, которое умеет работать с** \
**MMU, GIC, UART, SD картами и exFAT** \
**ZeroPoint является Hobby OS и особых целей пока не приследует**

# **Совместимость**
**ZeroPoint предназначена только для AArch64 и ARMv8. Поддержка ARMv7< и ниже, а так-же AArch32 не планируется** \
**Работоспособность ОС на устройствах зависит от [**Прошивок ZeroPointFirmware**](https://github.com/Operator-about/ZeroPointFirmware)** \
Но если устройство имеет **следующию периферию:** \
1. **UARTPL011/UART 165050A** \
2. **SD Standart/SD DesignWare** \
3. **GICv2/GICv3** \
4. **exFAT раздел на SD карте**

# **Возможности ОС**
ОС имеет свой собственный Shell: **ZeroShell** \
Так же ОС умеет работать с **exFAT** в режиме **Read-only**, с **SD карты**\
Имеется поддержка **MMU** до 32/36 **IPS** \
Имеется **HAL** для **SD** и **UART** \
Версия ОС - **0.0.3** 

# **Важная деталь**
**ZeroPoint - полностью зависит от прошивок: ZeroPointFirmware**\
[**Скачать прошивки можно от сюда**](https://github.com/Operator-about/ZeroPointFirmware)

# **Сборка:**
Проект собирается через компилятор - **aarch64-none-elf-** \
ОС собирается изначально в файл *.elf*. Для перевода его в *.img* используется - **aarch64-none-elf-objcopy** \
Для сборки используется **CMake**

**Следить за новостями по разработке можно тут:** \
[**Мой тг канал**](t.me/operator_about) \
[**Мой YouTube канал**](https://www.youtube.com/@Operator_about)
