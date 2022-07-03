/// \file      devnetXLOG.h
/// \brief     A simple Arduino logging library that provides easy printf-style outputs to Serial.
/// \author    Alexej Goujine (dev@devnetx.at)
/// \copyright Copyright (c) 2019-2021 devnetX Development

#pragma once

#include <Arduino.h>
#include <string.h>

#ifdef __cplusplus
	extern "C" {
#endif

	#if !(defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_SAMD))
		#error "Unsupported platform."
	#endif

	// devnetXLOG is deactivated
	#define DEVNETXLOG_OFF 0U

	// devnetXLOG minimal output
	#define DEVNETXLOG_MIN 1U

	// devnetXLOG medium output
	#define DEVNETXLOG_MED 2U

	// devnetXLOG maximal output
	#define DEVNETXLOG_MAX 3U

	// Baud Rate
	#if !defined(DEVNETXLOG_BAUD_RATE)
		#define DEVNETXLOG_BAUD_RATE 57600UL
	#endif

	// Line Break
	#if (defined(DEVNETXLOG_LINEBREAK_WINDOWS) && !defined(DEVNETXLOG_LINEBREAK_UNIX)) || \
		(!defined(DEVNETXLOG_LINEBREAK_WINDOWS) && !defined(DEVNETXLOG_LINEBREAK_UNIX))
		#define __BR__ "\r\n"
	#elif (defined(DEVNETXLOG_LINEBREAK_UNIX) && !defined(DEVNETXLOG_LINEBREAK_WINDOWS))
		#define __BR__ "\n"
	#else
		#error "Please define only one line break!"
	#endif

	// Termination
	#if defined(DEVNETXLOG_NOTERMINATION)
		#define __TERMINATION_STRING__ ""
	#else
		#define __TERMINATION_STRING__ "" __BR__
	#endif

	// Path Separator
	#if defined(DEVNETXLOG_NOPATHS)
	
		#if (defined(DEVNETXLOG_PATHSEPARATOR_WINDOWS) && !defined(DEVNETXLOG_PATHSEPARATOR_UNIX)) || \
			(!defined(DEVNETXLOG_PATHSEPARATOR_WINDOWS) && !defined(DEVNETXLOG_PATHSEPARATOR_UNIX))
			#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
		#elif (defined(DEVNETXLOG_PATHSEPARATOR_UNIX) && !defined(DEVNETXLOG_PATHSEPARATOR_WINDOWS))
			#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
		#else
			#error "Please define only one path separator!"
		#endif
	
	#else

		#define __FILENAME__ __FILE__

	#endif

	// Product Name
	#if !defined(PRODUCT_NAME)
		#define PRODUCT_NAME "PRODUCT_NAME not specified"
	#endif

	// Major Firmware Version
	#if !defined(FW_MAJOR)
		#define FW_MAJOR 0
	#endif

	// Minor Firmware Version
	#if !defined(FW_MINOR)
		#define FW_MINOR 0
	#endif

	// Patch Firmware Version
	#if !defined(FW_PATCH)
		#define FW_PATCH 0
	#endif

	// Welcome Message
	#if !defined(DEVNETXLOG_NOWELCOME)
		#define DEVNETXLOG_WELCOME __BR__ __BR__ " / \\  %s" __BR__ "( C ) Firmware Ver.: %u.%u.%u" __BR__ " \\ /  Compiled: %s, %s" __BR__ __BR__
	#endif

	// Fatal Error Message
	#if !defined(DEVNETXLOG_FATALERROR)
		#define DEVNETXLOG_FATALERROR "FATAL ERROR - System HALTED: "
	#endif

	#if defined(DEVNETXLOG) && (DEVNETXLOG > DEVNETXLOG_OFF)

		extern HardwareSerial *LOGSerial;
		bool LOGBegin(HardwareSerial *serial);
		
		// ##########################################################
		// ##   devnetXLOG PRINT() FLUSH() HALT() Implementation   ##
		// ##########################################################

		#if defined(ARDUINO_ARCH_AVR)

			#define PRINT(fmt, ...) do { \
				printf_P(PSTR(fmt), ##__VA_ARGS__); \
			} while (0)

			#define FLUSH() do { \
				LOGSerial->flush(); \
			} while (0)

			#define HALT(fmt, ...) do { \
				printf_P(PSTR(DEVNETXLOG_FATALERROR fmt), ##__VA_ARGS__); \
				while (true) { delay(1000); }; \
			} while (0)

		#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
			
			#define PRINT(fmt, ...) do { \
				LOGSerial->printf_P(PSTR(fmt), ##__VA_ARGS__); \
			} while (0)

			#define FLUSH() do { \
				LOGSerial->flush(); \
			} while (0)

			#define HALT(fmt, ...) do { \
				LOGSerial->printf_P(PSTR(DEVNETXLOG_FATALERROR fmt), ##__VA_ARGS__); \
				while (true) { delay(1000); }; \
			} while (0)

		#elif defined(ARDUINO_ARCH_SAMD)

			#if !defined(DEVNETXLOG_BUFFER_SIZE)
				#define DEVNETXLOG_BUFFER_SIZE 256UL
			#endif

			extern char LOGBuffer[DEVNETXLOG_BUFFER_SIZE];

			#define PRINT(fmt, ...) do { \
				snprintf(LOGBuffer, sizeof(LOGBuffer), fmt, ##__VA_ARGS__); \
				LOGSerial->print(LOGBuffer); \
			} while (0)

			#define FLUSH() do { \
				LOGSerial->flush(); \
			} while (0)

			#define HALT(fmt, ...) do { \
				snprintf(LOGBuffer, sizeof(LOGBuffer), DEVNETXLOG_FATALERROR fmt, ##__VA_ARGS__); \
				LOGSerial->print(LOGBuffer); \
				while (true) { delay(1000); }; \
			} while (0)

		#endif

		#if (DEVNETXLOG == DEVNETXLOG_MIN)

			// #################################################
			// ##   devnetXLOG LOG() Minimal Implementation   ##
			// #################################################

			#if defined(ARDUINO_ARCH_AVR)

				#define LOG(fmt, ...) do { \
					printf_P(PSTR(fmt __TERMINATION_STRING__), ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

				#define LOG(fmt, ...) do { \
					LOGSerial->printf_P(PSTR(fmt __TERMINATION_STRING__), ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_SAMD)

				#define LOG(fmt, ...) do { \
					snprintf(LOGBuffer, sizeof(LOGBuffer), fmt __TERMINATION_STRING__, ##__VA_ARGS__); \
					LOGSerial->print(LOGBuffer); \
				} while (0)

			#endif

		#elif (DEVNETXLOG == DEVNETXLOG_MED)

			// ################################################
			// ##   devnetXLOG LOG() Medium Implementation   ##
			// ################################################

			#if defined(ARDUINO_ARCH_AVR)

				#define LOG(fmt, ...) do { \
					printf_P(PSTR("%s:%u | " fmt __TERMINATION_STRING__), __FILENAME__, __LINE__, ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

				#define LOG(fmt, ...) do { \
					LOGSerial->printf_P(PSTR("%s:%u | " fmt __TERMINATION_STRING__), __FILENAME__, __LINE__, ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_SAMD)

				#define LOG(fmt, ...) do { \
					snprintf(LOGBuffer, sizeof(LOGBuffer), "%s:%u | " fmt __TERMINATION_STRING__, __FILENAME__, __LINE__, ##__VA_ARGS__); \
					LOGSerial->print(LOGBuffer); \
				} while (0)

			#endif

		#elif (DEVNETXLOG == DEVNETXLOG_MAX)

			// #################################################
			// ##   devnetXLOG LOG() Maximal Implementation   ##
			// #################################################

			extern uint32_t LOGMillisLast;
			extern uint32_t LOGMillisCurrent;

			#if defined(ARDUINO_ARCH_AVR)

				#define LOG(fmt, ...) do { \
					LOGMillisCurrent = millis(); \
					printf_P(PSTR("%07lu (%+06lu) | %s:%u | " fmt __TERMINATION_STRING__), LOGMillisCurrent, LOGMillisCurrent - LOGMillisLast, __FILENAME__, __LINE__, ##__VA_ARGS__); \
					LOGMillisLast = LOGMillisCurrent; \
				} while (0)

			#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

				#define LOG(fmt, ...) do { \
					LOGMillisCurrent = millis(); \
					LOGSerial->printf_P(PSTR("%07lu (%+06lu) | %s:%u | " fmt __TERMINATION_STRING__), LOGMillisCurrent, LOGMillisCurrent - LOGMillisLast, __FILENAME__, __LINE__, ##__VA_ARGS__); \
					LOGMillisLast = LOGMillisCurrent; \
				} while (0)

			#elif defined(ARDUINO_ARCH_SAMD)

				#define LOG(fmt, ...) do { \
					LOGMillisCurrent = millis(); \
					snprintf(LOGBuffer, sizeof(LOGBuffer), "%07lu (%+06ld) | %s:%u | " fmt __TERMINATION_STRING__, LOGMillisCurrent, LOGMillisCurrent - LOGMillisLast, __FILENAME__, __LINE__, ##__VA_ARGS__); \
					LOGSerial->print(LOGBuffer); \
					LOGMillisLast = LOGMillisCurrent; \
				} while (0)

			#endif

		#endif

	#else

		// devnetXLOG is deactivated
		extern "C" bool LOGBegin(HardwareSerial *serial);
		
		#define LOG(...)		do { (void)0; } while(0)
		#define PRINT(...)		do { (void)0; } while(0)
		#define FLUSH()			do { (void)0; } while(0)
		#define HALT(...)		do { while (true) { delay(1000); }; } while (0)

	#endif

#ifdef __cplusplus
}
#endif
