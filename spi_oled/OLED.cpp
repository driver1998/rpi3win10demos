#include "pch.h"
#include "OLED.h"


using namespace winrt::Windows::Devices::Spi;
using namespace winrt::Windows::Devices::Gpio;
using namespace winrt::Windows::Graphics::Imaging;


OLED::OLED(SpiDevice device, GpioPin dataCommandPin, GpioPin resetPin)
{
	if (device == nullptr)
		throw std::runtime_error("device is null");
	if (dataCommandPin == nullptr)
		throw std::runtime_error("dataCommandPin is null");
	if (dataCommandPin == nullptr)
		throw std::runtime_error("resetPin is null");

	this->displayDevice = device;
	this->dataCommandPin = dataCommandPin;
	this->resetPin = resetPin;
	this->buffer = std::vector<uint8_t>(SCREEN_WIDTH * SCREEN_HEIGHT_PAGES);

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

void OLED::sendCommand(const std::vector<uint8_t> &command)
{
	dataCommandPin.Write(GpioPinValue::Low);
	displayDevice.Write(command);
}
void OLED::sendData(const std::vector<uint8_t> &data)
{
	dataCommandPin.Write(GpioPinValue::High);
	displayDevice.Write(data);
}

void OLED::updateBuffer(const BitmapFrame &bitmap)
{
	if (bitmap.BitmapPixelFormat() != BitmapPixelFormat::Rgba8 
		&& bitmap.BitmapPixelFormat() != BitmapPixelFormat::Bgra8)
		throw std::runtime_error("Invalid pixel format, only RGBA8 and BGRA8 is supported");

	uint32_t height = bitmap.PixelHeight();
	uint32_t width = bitmap.PixelWidth();

	if (height != SCREEN_HEIGHT || width != SCREEN_WIDTH) throw;

	auto data = bitmap.GetPixelDataAsync().get();
	auto pixels = data.DetachPixelData();

	// copy the pixel data to display buffer
	// refer to datasheet for arrangement
	for (int i = 0; i < SCREEN_HEIGHT_PAGES; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			uint8_t block = 0;
			for (int k = 0; k < 8; k++) {
				
				int p = ((i * 8 + k) * SCREEN_WIDTH + j) * 4;
				uint8_t G = pixels[p + 1];

				uint8_t newBit = (G > 127) << 7;
				block = (block >> 1) | newBit;
			}
			buffer[i * SCREEN_WIDTH + j] = block;
		}
	}

	refresh();
}

void OLED::refresh()
{
	sendCommand(CMD_RESETCOLADDR);
	sendCommand(CMD_RESETPAGEADDR);
	sendData(buffer);
}