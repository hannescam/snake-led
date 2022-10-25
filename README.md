![GitHub all releases](https://img.shields.io/github/downloads/hannescam/snake-led/total)
# snake-led
- a snake for the rpi-rgb-led-matrix library
- the controll is with the keyboard with WASD keys (like the most games).
- it is needed to have this in the same folder with the rpi-rgb-led-matrix while compiling afterwards it isnt needed,
- or just download the binary file (for raspbian 64 bit) so pi zero 2, 3 and 4,
- i havent tested it with anything other than a pi 3b+ but should work with all pis.
- it is based on: https://github.com/HailTheBDFL/esp32-snake
## Installation
To compile, execute the following command.

```bash
make
```
To install (after compiling), execute the following command as root or using `sudo` (which is used in the following command).

```bash
sudo make install
```
To clean, execute the following command.

```bash
make clean
```
