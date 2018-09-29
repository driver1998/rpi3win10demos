# i2cdetect for Windows 10

This is a simple little tool that detects all your devices on the I2C bus.

It tries to read one byte from every device on address 0x08 - 0x77. When it succeed, it assumes that a device is available, and when it failed, it assumes that there is no device there.

Because of the way this program works, it may screw up your I2C bus. USE IT AT YOUR OWN RISK!

It display the info in a nice little grid:
- "--" means no device in that address
- a hexdecimal address like "3A" means there is a device in 0x3A.

```
      00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
0x00                          -- -- -- -- -- -- -- --
0x10  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
0x20  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
0x30  -- -- -- -- -- -- -- -- -- -- 3A -- -- -- -- --
0x40  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
0x50  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
0x60  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
0x70  -- -- -- -- -- -- -- --
```
