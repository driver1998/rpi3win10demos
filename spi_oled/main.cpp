// main.cpp : Defines the entry point for the console application.
//


#include "pch.h"


using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Devices::Enumeration;

int main()
{
    init_apartment();
	setlocale(LC_CTYPE, "");

	try {
		const int32_t RST_PIN = 23; // pin16
		const int32_t DC_PIN = 22;  // pin15
		const int32_t CS = 0;       // pin24

		// SP10 or SPI1 on Raspberry Pi
		const wchar_t *spiHardwareId = L"SPI0";

		// Get a selector string that will return our wanted SPI controller
		auto spi = SpiDevice::GetDeviceSelector(spiHardwareId);

		// Find the SPI bus controller devices with our selector string
		auto deviceInfo = DeviceInformation::FindAllAsync(spi).get();

		// Use chipselect line CS, 10Mhz Mode 3
		auto connSettings = SpiConnectionSettings(CS);
		connSettings.ClockFrequency(10000000);
		connSettings.Mode(SpiMode::Mode3);

		// create SPI device from controller settings
		auto displayDevice = SpiDevice::FromIdAsync(deviceInfo.GetAt(0).Id(), connSettings).get();

		// Get the default GPIO controller
		// and open other necessary pins
		auto gpioController = GpioController::GetDefaultAsync().get();
		auto dataCommandPin = gpioController.OpenPin(DC_PIN);
		auto resetPin = gpioController.OpenPin(RST_PIN);

		// initialize OLED
		auto oled = OLED(nullptr, dataCommandPin, resetPin);
		printf("OLED initialized on %ls, CS=%d DC=%d RST=%d.\n", spiHardwareId, CS, DC_PIN, RST_PIN);

		// load bitmap and draw it on the OLED.
		wchar_t currentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, currentDir);
		auto dir = StorageFolder::GetFolderFromPathAsync(currentDir).get();
		auto file = dir.GetFileAsync(L"logo.bmp").get();
		auto stream = file.OpenReadAsync().get();
		auto decoder = BitmapDecoder::CreateAsync(stream).get();
		if (decoder.FrameCount() > 0 && decoder.BitmapPixelFormat() == BitmapPixelFormat::Rgba8) {
			printf("Rendering test image...\n");
			auto frame = decoder.GetFrameAsync(0).get();
			oled.sendBitmap(frame);
		}

		// pause so that the image will stay on screen
		system("pause");
	}
	catch (hresult_error const & ex) {
		HRESULT hr = ex.to_abi();
		winrt::hstring message = ex.message();
		wprintf(L"%lx\n%ws\n", hr, message.c_str());
	}

}
