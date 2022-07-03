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

	/// devnetXLOG is deactivated: no output to device monitor at all + no UART initialization (build_flags = -D DEVNETXLOG=0)
	#define DEVNETXLOG_OFF 0U

	/// devnetXLOG minimal output: printf-style formatting only (build_flags = -D DEVNETXLOG=1)
	#define DEVNETXLOG_MIN 1U

	/// devnetXLOG medium output: file name + line number + printf-style formatting (build_flags = -D DEVNETXLOG=2)
	#define DEVNETXLOG_MED 2U

	/// devnetXLOG maximal output: millis + millis since last LOG() + file name + line number + printf-style formatting (build_flags = -D DEVNETXLOG=3)
	#define DEVNETXLOG_MAX 3U

	#if defined(DEVNETXLOG) && (DEVNETXLOG > DEVNETXLOG_OFF)

		#if !defined(DEVNETXLOG_LINEBREAK)
			#define DEVNETXLOG_LINEBREAK "\r\n"
		#endif

		#if !defined(DEVNETXLOG_BAUD_RATE)
			#define DEVNETXLOG_BAUD_RATE 57600UL
		#endif

		#if defined(DEVNETXLOG_NOPATHS)
			/*
				#define STRIPPATH(s) \
					(sizeof(s) > 2  && (s)[sizeof(s)-2]  == '\\' ? (s) + sizeof(s) - 1  : \
					sizeof(s) > 3  && (s)[sizeof(s)-3]  == '\\' ? (s) + sizeof(s) - 2  : \
					sizeof(s) > 4  && (s)[sizeof(s)-4]  == '\\' ? (s) + sizeof(s) - 3  : \
					sizeof(s) > 5  && (s)[sizeof(s)-5]  == '\\' ? (s) + sizeof(s) - 4  : \
					sizeof(s) > 6  && (s)[sizeof(s)-6]  == '\\' ? (s) + sizeof(s) - 5  : \
					sizeof(s) > 7  && (s)[sizeof(s)-7]  == '\\' ? (s) + sizeof(s) - 6  : \
					sizeof(s) > 8  && (s)[sizeof(s)-8]  == '\\' ? (s) + sizeof(s) - 7  : \
					sizeof(s) > 9  && (s)[sizeof(s)-9]  == '\\' ? (s) + sizeof(s) - 8  : \
					sizeof(s) > 10 && (s)[sizeof(s)-10] == '\\' ? (s) + sizeof(s) - 9  : \
					sizeof(s) > 11 && (s)[sizeof(s)-11] == '\\' ? (s) + sizeof(s) - 10 : (s))

				#define __FILENAME__ STRIPPATH(__FILE__)
			*/
			
			#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

		#else

			#define __FILENAME__ __FILE__

		#endif

		#if !defined(DEVNETXLOG_NO_WELCOME)
			//#define DEVNETXLOG_WELCOME "\n\n / \\  %s\n( C ) Firmware Ver.: %u.%u.%u\n \\ /  Compiled: %s, %s\n\n"
			#define DEVNETXLOG_WELCOME DEVNETXLOG_LINEBREAK DEVNETXLOG_LINEBREAK " / \\  %s" DEVNETXLOG_LINEBREAK "( C ) Firmware Ver.: %u.%u.%u" DEVNETXLOG_LINEBREAK " \\ /  Compiled: %s, %s" DEVNETXLOG_LINEBREAK DEVNETXLOG_LINEBREAK
		#endif

		#if !defined(DEVNETXLOG_FATAL_ERROR)
			#define DEVNETXLOG_FATAL_ERROR "FATAL ERROR - System HALTED: "
		#endif

		#if !defined(PRODUCT_NAME)
			#define PRODUCT_NAME "PRODUCT_NAME"
		#endif

		#if !defined(FW_MAJOR)
			#define FW_MAJOR 0
		#endif

		#if !defined(FW_MINOR)
			#define FW_MINOR 0
		#endif

		#if !defined(FW_PATCH)
			#define FW_PATCH 0
		#endif

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
				printf_P(PSTR(DEVNETXLOG_FATAL_ERROR fmt), ##__VA_ARGS__); \
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
				LOGSerial->printf_P(PSTR(DEVNETXLOG_FATAL_ERROR fmt), ##__VA_ARGS__); \
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
				snprintf(LOGBuffer, sizeof(LOGBuffer), DEVNETXLOG_FATAL_ERROR fmt, ##__VA_ARGS__); \
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
					printf_P(PSTR(fmt), ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

				#define LOG(fmt, ...) do { \
					LOGSerial->printf_P(PSTR(fmt), ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_SAMD)

				#define LOG(fmt, ...) do { \
					snprintf(LOGBuffer, sizeof(LOGBuffer), fmt, ##__VA_ARGS__); \
					LOGSerial->print(LOGBuffer); \
				} while (0)

				#define LOG(fmt, ...) \
				{ \
					snprintf(LOGBuffer, sizeof(LOGBuffer), fmt, ##__VA_ARGS__); \
					LOGSerial->print(LOGBuffer); \
				}

			#endif

		#elif (DEVNETXLOG == DEVNETXLOG_MED)

			// ################################################
			// ##   devnetXLOG LOG() Medium Implementation   ##
			// ################################################

			#if defined(ARDUINO_ARCH_AVR)

				#define LOG(fmt, ...) do { \
					printf_P(PSTR("%s:%u | " fmt), __FILENAME__, __LINE__, ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

				#define LOG(fmt, ...) do { \
					LOGSerial->printf_P(PSTR("%s:%u | " fmt), __FILENAME__, __LINE__, ##__VA_ARGS__); \
				} while (0)

			#elif defined(ARDUINO_ARCH_SAMD)

				#define LOG(fmt, ...) do { \
					snprintf(LOGBuffer, sizeof(LOGBuffer), "%s:%u | " fmt, __FILENAME__, __LINE__, ##__VA_ARGS__); \
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
					printf_P(PSTR("%07lu (%+06lu) | %s:%u | " fmt), LOGMillisCurrent, LOGMillisCurrent - LOGMillisLast, __FILENAME__, __LINE__, ##__VA_ARGS__); \
					LOGMillisLast = LOGMillisCurrent; \
				} while (0)

			#elif defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)

				#define LOG(fmt, ...) do { \
					LOGMillisCurrent = millis(); \
					LOGSerial->printf_P(PSTR("%07lu (%+06lu) | %s:%u | " fmt), LOGMillisCurrent, LOGMillisCurrent - LOGMillisLast, __FILENAME__, __LINE__, ##__VA_ARGS__); \
					LOGMillisLast = LOGMillisCurrent; \
				} while (0)

			#elif defined(ARDUINO_ARCH_SAMD)

				#define LOG(fmt, ...) do { \
					LOGMillisCurrent = millis(); \
					snprintf(LOGBuffer, sizeof(LOGBuffer), "%07lu (%+06ld) | %s:%u | " fmt, LOGMillisCurrent, LOGMillisCurrent - LOGMillisLast, __FILENAME__, __LINE__, ##__VA_ARGS__); \
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
