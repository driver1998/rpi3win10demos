#include "pch.h"
#include <clocale>
#include <vector>

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
		if (controller == nullptr) throw std::runtime_error("I2C controller not found");

		printf("Searching I2C devices...\n");
		printf("     0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");

		for (int i = 0x00; i <= 0x70; i += 0x10) {
			printf("\n%02x: ", i);
			for (int j = 0x0; j <= 0xF; j++) {
				int address = i | j;

				// Current supported range: 0x08-0x77
				if (address < 0x08 || address > 0x77) {
					printf("   ");  
					continue;
				}

				I2cConnectionSettings connSettings(address);
				I2cDevice device = controller.GetDevice(connSettings);
				std::vector<uint8_t> buff(1);

				// Try to read one byte from device
				try {
					device.Read(buff);
					printf("%02x ", address);
				}
				catch (hresult_error const & ex) {
					UNREFERENCED_PARAMETER(ex);
					printf("-- ");
				}

				device.Close();
			}
		}
		printf("\n");
	}
	catch (hresult_error const & ex) {
		wprintf(L"%08X %ws\n", ex.to_abi().value, ex.message().c_str());
	}
	catch (std::runtime_error const& ex) {
		puts(ex.what());
	}
}
