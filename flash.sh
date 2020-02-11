#!/bin/bash
make
dfu-programmer atmega16u2 erase
dfu-programmer atmega16u2 flash Joystick.hex
dfu-programmer atmega16u2 reset

