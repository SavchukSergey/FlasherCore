# FlasherCore

Implementation of STK 500 v1 protocol for programming both Atmega and PIC microcontrollers

# Service mode
This firmware contains special service menu for accessing PIC and AVR devices raw protocol.
Service menu is available via COM-port.

All data values used as input or as output are specified in hex format: 0-9, A-F, a-f

Enter Service Mode

| Sequence  | Target      | Example | Output                  |
|-----------|-------------|---------|-------------------------|
| `SP`      | PIC devices | `SP`    | Hello. Pic Service Mode |
| `SA`      | AVR devices | `SA`    | Hello. Avr Service Mode |

## Common service commands
| Sequence | Command                          | Example | Output |
|----------|----------------------------------|---------|--------|
| `P`      | Start programming mode           | `P`     |        | 
| `Q`      | Quit programming mode            | `Q`     |        |
| `E`      | Quit service menu                | `E`     | Bye    |
| [space]  | Put additional space into output |         |        |

## Pic service commands 

| Sequence | Command                            | Example | Output |
|----------|------------------------------------|---------|--------|
| `C`      | Switch to configuration  space     | `C`     |        |
| `I`      | Increment address                  | `I`     |        |
| `R`      | Read program/configuration memory  | `R`     | 3FFF   |
| `Wxxxx`  | Write program/configuration memory | `W1234` |        |
| `Gxxxx`  | Go to address                      | `G1234` |        |
| `A`      | Show current address               | `A`     | C1234  |
| `p0`     | Set power off                      | `p0`    |        |
| `p1`     | Set power on                       | `p1`    |        |
| `m0`     | Set MCLR off                       | `m0`    |        |
| `m1`     | Set MCLR on                        | `m1`    |        |

### Popular PIC commands

| Sequence          | Description                        |
|-------------------|------------------------------------|
| `PCIIIIIIIRQ`     | Read configuration word            |
| `PCIIIIIIIW1234Q` | Write 0x1234 to configuration word |
| `PCRIRIRIRIIIRQ`  | Read device ID                     |

## Avr service commands
| Sequence    | Command                  | Example     | Output |
--------------|--------------------------|-------------|--------|
| `Uaabbccdd` | Send generic SPI command | `Uac530000` | 78     |

### Popular AVR commands

| Sequence    | Description                              |
|-------------|------------------------------------------|
| `UAC530000` | Start serial programming                 |
| `UAC800000` | Chip erase                               |
| `U30000000` | Get signature byte 0                     |
| `U30010000` | Get signature byte 1                     |
| `U30020000` | Get signature byte 2                     |
| `U20xxxx00` | Read flash low byte at address xxxx      |
| `U28xxxx00` | Read flash high byte at address xxxx     |
| `U30xxxxyy` | Write flash low byte yy at address xxxx  |
| `U38xxxxyy` | Write flash high byte yy at address xxxx |

# Tested platforms
FlasherCore successfully tested on

Intermediate microcontrollers:
* Atmega8
* Atmega328

Target microcontrollers:
* ATmega8
* ATmega328
* ATtiny2313
* PIC16F628
