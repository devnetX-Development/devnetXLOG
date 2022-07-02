#include <Arduino.h>
#include <devnetXLOG.h>

// =========================================================================
// NOTE: You have to add some build_flags to platformio.ini in your project!
// =========================================================================
// build_flags =
// 	'-D PRODUCT_NAME="devnetXLOG Example Firmware"'
// 	-D FW_MAJOR=1
// 	-D FW_MINOR=0
// 	-D FW_PATCH=0
// 	-D DEVNETXLOG=3

void setup()
{
	// Init devnetXLOG
	LOGBegin(&Serial);
}

void loop()
{
	LOG("This is a %s test.\n", "LOG()");
	PRINT("This is a %s test.\n", "PRINT()");
	delay(1000);
}
