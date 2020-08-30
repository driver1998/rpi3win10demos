#pragma once
#include "pch.h"

using namespace winrt::Windows::Devices::Spi;
using namespace winrt::Windows::Devices::Gpio;
using namespace winrt::Windows::Graphics::Imaging;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_HEIGHT_PAGES SCREEN_HEIGHT/8                         // Screen hight in units of pages

/* Display commands. See the datasheet for details on commands: http://www.adafruit.com/datasheets/SSD1306.pdf                      */
const std::vector<uint8_t> CMD_DISPLAY_OFF = { 0xAE };              /* Turns the display off                                    */
const std::vector<uint8_t> CMD_DISPLAY_ON = { 0xAF };               /* Turns the display on                                     */
const std::vector<uint8_t> CMD_CHARGEPUMP_ON = { 0x8D, 0x14 };      /* Turn on internal charge pump to supply power to display  */
const std::vector<uint8_t> CMD_MEMADDRMODE = { 0x20, 0x00 };        /* Horizontal memory mode                                   */
const std::vector<uint8_t> CMD_SEGREMAP = { 0xA1 };                 /* Remaps the segments, which has the effect of mirroring the display horizontally */
const std::vector<uint8_t> CMD_COMSCANDIR = { 0xC8 };               /* Set the COM scan direction to inverse, which flips the screen vertically        */
const std::vector<uint8_t> CMD_RESETCOLADDR = { 0x21, 0x00, 0x7F }; /* Reset the column address pointer                         */
const std::vector<uint8_t> CMD_RESETPAGEADDR = { 0x22, 0x00, 0x07 };/* Reset the page address pointer                           */

class OLED
{
private:
	// SPI device for the SSD1306 display controller.
	SpiDevice displayDevice = nullptr;

	// DC pin
	GpioPin dataCommandPin = nullptr;

	// RST pin
	GpioPin resetPin = nullptr;

	// memory clone of the display buffer
	// 128px * 8 pages, each page is 1 pixel wide, 8 pixels tall.
	std::vector<uint8_t> buffer;

	// display init
	void init();

	// upload the display buffer to the screen
	// and refresh the display
	void refresh();

public:
	OLED(SpiDevice device, GpioPin dataCommandPin, GpioPin resetPin);

	// reset the display
	void reset();

	// send command to the display
	void sendCommand(const std::vector<uint8_t> &command);

	// send data to the display
	void sendData(const std::vector<uint8_t> &data);

	// update the screen buffer with a 128x64 RGBA8 bitmap
	void updateBuffer(const BitmapFrame &bitmap);
};

