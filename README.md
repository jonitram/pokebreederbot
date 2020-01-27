# Poke Breeder Bot

As long as the in-game preconditions are met before plugging in the device, plugging the Arduino directly into the switch through a USB Type B to USB Type C Cable should enable the bot to start picking up and hatching eggs automatically without any user input

## In-Game Information

### Preconditions

- You are not on your bike
- You are currently at the Nursery in the Wild Area
- There is an egg waiting for you at the Nursery
- Ensure that when you hit "X" the cursor is on the "Town Map" icon
- Your party is full of non-egg pokemon (The first pokemon will never be swapped out so it can be a flame body pokemon)
- In-Game Options:
    - Text Speed: Fast
    - Send to Boxes: Manual
    - Give Nicknames: Don't Give
    - Vertical Camera Control: Regular
- It's recommended to direct your box to a series of empty boxes before beginning the automatic breeding process

### Breeding Duration Values

| Breeding Duration | Pokemon   | Cycles    | Steps | Cycling Time  |
| ----------------- | --------- | --------- | ----- | ------------- |
| 3500 (83 seconds) | Dreepy    | 40        | 10k   | 123 seconds   |

- To use the `steps2breedingduration.py` script type:

    `./steps2breedingduration.py <number of steps to hatch the egg>`

Where the `number of steps to hatch the egg` is the value taken from this [link](https://bulbapedia.bulbagarden.net/wiki/List_of_Pok%C3%A9mon_by_base_Egg_cycles) in the far right column of the species of pokemon you are trying to hatch

## Setting up the Arduino

- Disclaimer: These instructions were written for Mac OSX in the context of an Arduino with an atmega16u2 chip

### Dependencies and Compiling

- Install dfu-programmer through homebrew with:

    `brew install dfu-programmer`
- To compile this project you need AVR GCC Compiler and Tools. This can be installed through homebrew with:

    `brew tap osx-cross/avr`

    followed by

    `brew install avr-gcc`
- Download the LUFA library through this [link](http://www.fourwalledcubicle.com/LUFA.php) and put it in this directory
- Make sure the `LUFA_PATH` inside the `makefile` points to the LUFA subdirectory that you just created inside this directory
- Ensure that the `makefile` setting for `MCU` is set to `MCU = atmega16u2` and compile by typing `make`

### Putting the Arduino in DFU Mode

- Plug the Arduino into a computer
- Put the Arduino in DFU mode if it is not already by bridging the reset pin with the ground
- If necessary, ensure the Arduino is in DFU mode by typing:

    `sudo dfu-programmer atmega16u2 get ID1`

### Flashing onto the Arduino

- Navigate to the folder containing the `.hex` file you are flashing onto the Arduino
- In this case, the `.hex` file to be flashed onto the Arduino is called `Joystick.hex` generated by the `makefile`
- Follow these commands to flash the `.hex` file:

    `sudo dfu-programmer atmega16u2 erase`

    `sudo dfu-programmer atmega16u2 flash <.hex file>`

    `sudo dfu-programmer atmega16u2 reset`

- If you receive this error:

    `dfu-programmer: no device present.`

but your Arduino is plugged in and the lights are on then you most likely need to put the Arduino into DFU mode by bridging its reset and ground pins