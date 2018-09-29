#include "pch.h"




OLED::OLED(SpiDevice device, GpioPin dataCommandPin, GpioPin resetPin)
{
	if (device == nullptr || dataCommandPin == nullptr || resetPin == nullptr) throw;
	this->displayDevice = device;
	this->dataCommandPin = dataCommandPin;
	this->resetPin = resetPin;
	init();
}


void OLED::init()
{
	this->dataCommandPin.Write(GpioPinValue::High);
	this->dataCommandPin.SetDriveMode(GpioPinDriveMode::Output);

	this->resetPin.Write(GpioPinValue::High);
	this->resetPin.SetDriveMode(GpioPinDriveMode::Output);

	reset();
	sendCommand(CMD_CHARGEPUMP_ON);
	sendCommand(CMD_MEMADDRMODE);
	sendCommand(CMD_SEGREMAP);
	sendCommand(CMD_COMSCANDIR);
	sendCommand(CMD_DISPLAY_ON);

}

void OLED::reset()
{
	resetPin.Write(GpioPinValue::Low);
	Sleep(1);
	resetPin.Write(GpioPinValue::High);
	Sleep(100);
}

void OLED::sendCommand(const std::vector<uint8_t> command)
{
	dataCommandPin.Write(GpioPinValue::Low);
	displayDevice.Write(command);
}
void OLED::sendData(const std::vector<uint8_t> data)
{
	dataCommandPin.Write(GpioPinValue::High);
	displayDevice.Write(data);
}

void OLED::sendBitmap(BitmapFrame bitmap) 
{	
	if (bitmap.BitmapPixelFormat() != BitmapPixelFormat::Rgba8) throw;

	uint32_t height = bitmap.PixelHeight();
	uint32_t width = bitmap.PixelWidth();

	if (height != SCREEN_HEIGHT || width != SCREEN_WIDTH) throw;

	auto data = bitmap.GetPixelDataAsync().get();
	auto pixels = data.DetachPixelData();

	// copy the pixel data to display buffer
	// refer to datasheet for arrangement
	for (uint32_t i = 0; i < SCREEN_HEIGHT_PAGES; i++) {
		for (uint32_t j = 0; j < SCREEN_WIDTH; j++) {
			uint8_t block = 0;
			for (uint32_t k = 0; k < 8; k++) {
				// convert RGBA bitmap to monochrome
				// R>127  : 1
				// R<=127 : 0
				uint8_t newBit = (pixels[((i * 8 + k) * SCREEN_WIDTH + j) * 4] > 127) << 7;
				block = (block >> 1) | newBit;
			}
			displayBuffer[i*SCREEN_WIDTH + j] = block;
		}
	}

	refresh();
}

void OLED::refresh()
{
	sendCommand(CMD_RESETCOLADDR);
	sendCommand(CMD_RESETPAGEADDR);
	std::vector<uint8_t> buffer
		(displayBuffer, displayBuffer + sizeof(displayBuffer) / sizeof(uint8_t));
	sendData(buffer);
}