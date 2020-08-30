# i2cdetect for Windows 10

This is a simple little tool that detects all your devices on the I2C bus, inspired by the tool on Linux.

It tries to read one byte from every device on address 0x08 - 0x77. When it succeed, it assumes that a device is available, and when it failed, it assumes that there is no device there.

Because of the way this program works, it may screw up your I2C bus. USE IT AT YOUR OWN RISK!

It display the info in a nice little grid:
- "--" means no device in that address
- a hexdecimal address like "3A" means there is a device in 0x3A.

```
     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
00:                         -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- 3A -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```
