/// \file      devnetXLOG.cpp
/// \brief     A simple Arduino logging library that provides easy printf-style outputs to Serial.
/// \author    Alexej Goujine (dev@devnetx.at)
/// \copyright Copyright (c) 2019-2021 devnetX Development

#include <Arduino.h>
#include <devnetXLog.h>

#if defined(DEVNETXLOG) && (DEVNETXLOG > DEVNETXLOG_OFF)

	HardwareSerial *LOGSerial;

	#if defined(ARDUINO_ARCH_AVR)

		extern "C" int SerialPutChar(char c, FILE *)
		{
			LOGSerial->write(c);
			return c;
		}

	#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

	#elif defined(ARDUINO_ARCH_SAMD)

		extern "C" {
			char LOGBuffer[DEVNETXLOG_BUFFER_SIZE];
		}

	#endif

	#if (DEVNETXLOG == DEVNETXLOG_MAX)

		extern "C" {
			uint32_t LOGMillisLast;
			uint32_t LOGMillisCurrent;
		}

	#endif
	
	extern "C" bool LOGBegin(HardwareSerial *serial) {
		if (serial == NULL)
			return false;

		LOGSerial = serial;
		LOGSerial->begin(DEVNETXLOG_BAUD_RATE);
		
		// Controller specific pre-initialization
		#if defined(ARDUINO_ARCH_AVR)
			fdevopen(&SerialPutChar, 0);
		#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

		#elif defined(ARDUINO_ARCH_SAMD)

		#endif

		// Welcome String output
		#if !defined(DEVNETXLOG_NO_WELCOME)

			#if defined(ARDUINO_ARCH_AVR)
				printf_P(PSTR(DEVNETXLOG_WELCOME), PRODUCT_NAME, FW_MAJOR, FW_MINOR, FW_PATCH, __DATE__, __TIME__);
			#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
				LOGSerial->printf_P(PSTR(DEVNETXLOG_WELCOME), PRODUCT_NAME, FW_MAJOR, FW_MINOR, FW_PATCH, __DATE__, __TIME__);
			#elif defined(ARDUINO_ARCH_SAMD)
				snprintf(LOGBuffer, sizeof(LOGBuffer), DEVNETXLOG_WELCOME , PRODUCT_NAME, FW_MAJOR, FW_MINOR, FW_PATCH, __DATE__, __TIME__);
				LOGSerial->print(LOGBuffer);
			#endif

		#endif
		
		// Controller specific post-initialization
		#if defined(ARDUINO_ARCH_AVR)

		#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

		#elif defined(ARDUINO_ARCH_SAMD)

		#endif

		// devnetXLOG is activated
		return true;
	}

#else

	// devnetXLOG is deactivated
	extern "C" bool LOGBegin(HardwareSerial *serial) {
		return false;
	}

#endif
