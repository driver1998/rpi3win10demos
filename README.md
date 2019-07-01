# Demo apps for Windows 10 on Raspberry Pi

This is a collection of demo apps for Windows 10 on Raspberry.

These are all console apps, and should work in both IoT Core ARM32 and Desktop ARM64.

## Build
You'll need the following prerequisites:

- Visual Studio 2019
- C/C++ ARM Support
- Windows 10 SDK 17763+

Then you can build them in Visual Studio.

## Contents
For detailed information, check out README.md in the sub-directories.

- `hwmon` A simple tool to check system temperature, clock speeds and voltages, based on the RPIQ driver.
- `spi_oled` Display the Windows on Raspberry logo on a SSD1306 based 128x64 OLED display.
- `i2cdetect` Inspired from the Linux tool with the same name, which detects all devices on the I2C bus. 
