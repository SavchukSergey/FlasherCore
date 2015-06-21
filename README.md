# FlasherCore

Implementation of STK 500 v1 protocol for programming both Atmega and PIC microcontrollers

# Service mode
This firmware contains special service menu for accessing PIC and AVR devices raw protocol.
Service menu is available via COM-port.
All data values used as input or as output are specified in hex format: 0-9, A-F, a-f

Enter Service Mode

| Sequence  | Target      | Example | Output                  |
|-----------|-------------|---------|-------------------------|
| SP        | PIC devices | SP      | Hello. Pic Service Mode |
| SA        | AVR devices | SA      | Hello. Avr Service Mode |

## Common service commands
| Sequence | Command                | Example | Output |
|----------|------------------------|---------|--------|
| P        | Start programming mode | P       |        | 
| Q        | Quit programming mode  | Q       |        |
| E        | Quit service menu      | E       | Bye    |
| {"[space]"}  | Put additional space into output |  | |

## Pic service commands 

| Sequence | Command                            | Example | Output |
|----------|------------------------------------|---------|--------|
| C        | Switch to configuration  space     | C       |        |
| I        | Increment address                  | I       |        |
| R        | Read program/configuration memory  | R       | 3FFF   |
| W_xxxx_  | Write program/configuration memory | W1234   |        |
| G_xxxx_  | Go to address                      | G1234   |        |
| A        | Display current address and address space | A | C1234 |
| p0       | Set power off                      | p0      |        |
| p1       | Set power on                       | p1      |        |
| m0       | Set MCLR off                       | m0      |        |
| m1       | Set MCLR on                        | m1      |        |

### Popular commands

PCIIIIIIIRQ - Read configuration word

PCIIIIIIIW1234Q - Write 0x1234 to configuration word

PCRIRIRIRIIIRQ - Read device ID

## Avr service commands
| Sequence    | Command                  | Example   | Output |
--------------|--------------------------|-----------|--------|
| U_aabbccdd_ | Send generic SPI command | Uac530000 | 78     |

# Tested platforms
FlasherCore successfully tested on

Intermediate microcontrollers:
* Atmega8
* Atmega328

Target microcontrollers:
* Atmega8
* Atmega328
* PIC16F628
