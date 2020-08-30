#include "pch.h"

#include <iostream>
#include <fstream>
#include <clocale>

#include <wil/resource.h>
#include "resource.h"
#include "OLED.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Devices::Enumeration;

int main()
{
    init_apartment();
	setlocale(LC_CTYPE, "");

	try {
		const int32_t RST_PIN = 23; // pin16
		const int32_t DC_PIN = 22;  // pin15
		const int32_t CS = 0;       // pin24

		// SP10 on Raspberry Pi
		const wchar_t *spiHardwareId = L"SPI0";

		// Get a selector string that will return our wanted SPI controller
		auto spi = SpiDevice::GetDeviceSelector(spiHardwareId);

		// Find the SPI bus controller devices with our selector string
		auto deviceInfo = DeviceInformation::FindAllAsync(spi).get();

		// Use chipselect line CS, 10Mhz Mode 3
		auto connSettings = SpiConnectionSettings(CS);
		connSettings.ClockFrequency(10000000);
		connSettings.Mode(SpiMode::Mode3);

		// Create SPI device from controller settings
		auto displayDevice = SpiDevice::FromIdAsync(deviceInfo.GetAt(0).Id(), connSettings).get();

		// Get the default GPIO controller
		// and open other necessary pins
		auto gpioController = GpioController::GetDefaultAsync().get();
		auto dataCommandPin = gpioController.OpenPin(DC_PIN);
		auto resetPin = gpioController.OpenPin(RST_PIN);

		// Initialize OLED
		auto oled = OLED(displayDevice, dataCommandPin, resetPin);
		std::wcout << L"OLED initialized on " << spiHardwareId << L" CS=" << CS;
		std::wcout << L" DC=" << DC_PIN << L" RST=" << RST_PIN << std::endl;

		// Load logo from resources
		HINSTANCE hInstance = GetModuleHandle(NULL);
		HRSRC hResInfo = FindResource(hInstance, MAKEINTRESOURCEW(IDB_LOGO), L"IMAGE");
		if (hResInfo == nullptr) throw std::runtime_error("Cannot find resource.");

		DWORD size = SizeofResource(hInstance, hResInfo);
		HGLOBAL hRes = LoadResource(hInstance, hResInfo);
		if (hRes == 0) throw std::runtime_error("Cannot load resource.");

		auto data = static_cast<const uint8_t*>(LockResource(hRes));
		if (data == nullptr) throw std::runtime_error("Cannot lock resource.");

		// Copy to a WinRT stream, it sucks but that's
		// what happened when you don't want to use 3rd party libraries...
		auto stream = InMemoryRandomAccessStream();
		auto writer = DataWriter(stream);
		array_view<const uint8_t> fromData(data, size);
		writer.WriteBytes(fromData);
		writer.StoreAsync().get();
		writer.DetachStream();

		// Decode the image using WinRT API
		auto decoder = BitmapDecoder::CreateAsync(stream).get();

		// Show the image
		std::cout << "Rendering Logo Image..." << std::endl;
		auto frame = decoder.GetFrameAsync(0).get();
		oled.updateBuffer(frame);
		
		// Pause so that the image will stay on screen
		system("pause");
	}
	catch (std::runtime_error const& ex) {
		std::cout << ex.what() << std::endl;
	}
	catch (hresult_error const & ex) {
		std::wcout << std::hex << ex.to_abi().value << " ";
		std::wcout << std::wstring(ex.message().c_str()) << std::endl;
	}

}
