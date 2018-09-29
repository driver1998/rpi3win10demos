// main.cpp : Defines the entry point for the console application.
//

#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::I2c;
using namespace Windows::Devices::Enumeration;

int main()
{
    init_apartment();
	setlocale(LC_CTYPE, "");

	try {
		I2cController controller = I2cController::GetDefaultAsync().get();
		
		printf("Searching I2C devices...\n");
		printf("     ");

		for (int32_t j = 0x0; j <= 0xF; j++) printf("%02x ", j);
		
		for (int32_t i = 0x00; i <= 0x70; i += 0x10) {
			printf("\n0x%02x ", i);
			for (int32_t j = 0x0; j <= 0xF; j++) {
				int32_t address = i | j;
				if (address < 0x08) {
					wprintf(L"   ");  
					continue;
				}
				if (address > 0x77) break;

				try {
					I2cConnectionSettings connSettings = I2cConnectionSettings(address);
					I2cDevice device = controller.GetDevice(connSettings);
					std::vector<uint8_t> buff = std::vector(1, (uint8_t)0);
					device.Read(buff);
					device.Close();
				}
				catch (hresult_error const & ex) {
					printf("-- ");
					continue;
				}

				printf("%02x ", address);
			}
		}
		printf("\n");
	}
	catch (hresult_error const & ex) {
		HRESULT hr = ex.to_abi();
		winrt::hstring message = ex.message();
		wprintf(L"%lx\n%ws\n", hr, message.c_str());
	}


}
