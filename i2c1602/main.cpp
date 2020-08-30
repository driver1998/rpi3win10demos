#include "pch.h"

#include <iostream>
#include <clocale>
#include "LiquidCrystal_I2C.h"

using namespace winrt;
using namespace Windows::Devices::I2c;

const char* weekdays[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

int main()
{
	init_apartment();
	setlocale(LC_CTYPE, "");

	try {
		I2cController i2cController = I2cController::GetDefaultAsync().get();
		if (i2cController == nullptr) throw std::runtime_error("I2C controller not found");

		I2cDevice i2cDevice = i2cController.GetDevice(I2cConnectionSettings(0x27));
		LiquidCrystal_I2C lcd1602(i2cDevice, 16, 2);

		lcd1602.init();
		lcd1602.backlight();
		char s[20];

		while (true) {

			time_t t = time(nullptr);
			tm ti;
			localtime_s(&ti, &t);

			sprintf_s(s, "%04d-%02d-%02d %s", ti.tm_year + 1900, ti.tm_mon, ti.tm_mday, weekdays[ti.tm_wday]);
			lcd1602.setCursor(1, 0);
			lcd1602.print(s);

			sprintf_s(s, "%02d:%02d:%02d", ti.tm_hour, ti.tm_min, ti.tm_sec);
			lcd1602.setCursor(4, 1);
			lcd1602.print(s);

			Sleep(1000);
		}
	}
	catch (hresult_error const & ex) {
		wprintf(L"%08X %ws\n", ex.to_abi().value, ex.message().c_str());
	}
	catch (std::runtime_error const& ex) {
		puts(ex.what());
	}

	return 0;
}
