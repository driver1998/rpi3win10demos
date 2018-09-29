# SPI OLED demo

This is a basic demo for Windows 10 on Raspberry Pi.

It displays the Windows on Raspberry Logo on a SSD1306 based 128x64 OLED display.

It can also be used as a basic C++/WinRT library for such type of display. (Check out OLED.cpp and OLED.h for details)

## Default Pinout

This demo uses SPI0 by default, and the pinout is the following:

OLED| Raspberry Pi
----|-------------
CLK|23
DATA|19
RST|16
DC|15
CS|24

## Before You Run

This demo needs `logo.bmp` in the current directory, you can get one from the repo, it just doesn't auto copy when compiling yet.

You can also use your custom bitmap, it has to be a B&W image in 128x64 RGB8 bmp format (At least for now).

## Acknowledgements
- The [UWP version](https://github.com/Microsoft/Windows-iotcore-samples/tree/develop/Samples/SPIDisplay) of SSD1306 sample from Microsoft
- Big Bob from the [Windows 10 ARM for Raspberry Pi discord server](https://discord.gg/uCV4ZYC) for the awesome logo
