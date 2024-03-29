#include <Arduino.h>
#include <devnetXLOG.h>

// =========================================================================
// NOTE: You have to add some build_flags to platformio.ini in your project!
// See: https://registry.platformio.org/libraries/devnetx/devnetXLOG
// =========================================================================

void setup()
{
	// Init devnetXLOG
	LOGBegin(&Serial);
}

void loop()
{
	LOG("This is a test %s, followed by an integer: %i", "String", 42);
	PRINTF("This is a %s without line termination. Loading ", "message");

	for (uint8_t n = 0; n < 10; n++)
	{
		PRINT(".");
		delay(500);	
	}

	PRINTLN(" done.");

	HALT("Actually, no error occured. Demo done.");
}
