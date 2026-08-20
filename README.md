# **ZeroPoint - Операционная Система**
**Небольшое AArch64 ядро, которое умеет работать с** \
**MMU, GIC, UART, SD картами и exFAT** \
**ZeroPoint является Hobby OS и особых целей пока не приследует**

# **Совместимость**
**ZeroPoint предназначена только для AArch64 и ARMv8. Поддержка ARMv7 и ниже, а так-же AArch32 не планируется** \
**Работоспособность ОС на устройствах зависит от [**Прошивок ZeroPointFirmware**](https://github.com/Operator-about/ZeroPointFirmware)** \
Но если устройство имеет **следующию периферию:**
1. **UARTPL011/UART 165050A** 
2. **SD Standart/SD DesignWare**
3. **GICv2/GICv3** 
4. **exFAT раздел на SD карте**

# **Возможности ОС**
1. ОС имеет свой собственный Shell: **ZeroShell** \
<img width="661" height="418" alt="Снимок экрана 2026-08-20 184649" src="https://github.com/user-attachments/assets/f31a6f63-a297-4a3c-9191-64c2046f4160" /> \
2. Так же ОС умеет работать с **exFAT** в режиме **Read-only**, с **SD карты** \
<img width="661" height="418" alt="Снимок экрана 2026-08-20 184713" src="https://github.com/user-attachments/assets/ff93ef04-37cc-42e6-b9c5-17bf1ad6f997" /> \
<img width="661" height="418" alt="Снимок экрана 2026-08-20 184725" src="https://github.com/user-attachments/assets/fb38b970-c541-4c74-a67f-2b78d41025ab" /> \
3. Имеется поддержка **MMU** до 32/36 **IPS**
4. Имеется **HAL** для **SD** и **UART**
5. Версия ОС - **0.0.3**
<img width="661" height="418" alt="Снимок экрана 2026-08-20 184736" src="https://github.com/user-attachments/assets/c40a300b-d774-41b7-8d9c-22a2bc44099f" /> 

# **ZeroShell**
В **ZeroShell** имеются следующие команды:
1. **about** - информация о системе
2. **help** - выводит список команд
3. **open** - открывает папки/файлы. Является **ls**, **cd**, **cat** одновременно. Для того, чтобы вернуться в корневой каталог, нужно ввести символ: **/**

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
