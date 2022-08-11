# devnetXLOG
A simple Arduino logging library that provides easy printf-style outputs to Serial.

## Library Functions
### Initialization
```c++
bool LOGBegin(HardwareSerial *serial);
```
Initializes the library and the Serial specified by ```serial```.
Displays the welcome message if not deactivated with ```DEVNETXLOG_NOWELCOME```.
The initialization must be done **before** calling any other library functions.

*Returns:* ```true``` on success or ```false``` if ```DEVNETXLOG``` is not defined or set to ```0```.

### LOG
```c++
void LOG(const char *fmt, ...);
```
Sends a log entry over Serial, according to the selected feature level and specified parameters and options.
The message can be formatted using printf-style syntax.

For more details on printf formatting, see: https://www.tutorialspoint.com/c_standard_library/c_function_printf.htm

### PRINT
```c++
void PRINT(const char *fmt, ...);
```
Sends a raw message over Serial without any features defined by the feature level or specified parameters and options.
The message can be formatted using printf-style syntax.

### FLUSH
```c++
void FLUSH();
```
Waits until the transmit buffer is empty. 
This can be useful if you want to ensure that all data has been transmitted by the Serial before sending the MCU to sleep.

### HALT
```c++
void HALT(const char *fmt, ...);
```
Sends the predefined HALT message over Serial followed by your message.
The message can be formatted using printf-style syntax.
Puts the MCU in a simple endless loop and provides no power saving features.


## Feature Levels
devnetXLOG can be configured to display multiple information before your printf-styled output using the provided ```LOG(...)``` command.
The displayed information is defined by the feature level and can be specified by the ```build_flags``` parameter ```-D DEVNETXLOG```. See: [Supported Parameters](#supported-parameters).
- ```0```
	devnetXLOG is deactivated with *no output*. This is the default behavior!
	Displays: *nothing* - no output will be sent to device monitor at all and no UART/Serial initialization.
	The compiler will optimize out all of your ```LOG()```, ```PRINT()``` and ```HALT()``` messages and no memory will be used.
- ```1```
	devnetXLOG is activated with the *minimal output*.
	Displays: printf-style formatted message only.
- ```2```
	devnetXLOG is activated with the *medium output*.
	Displays: File names, line numbers, printf-style formatted message.
- ```3```
	devnetXLOG is activated with the *maximal output*.
	Displays: ```millis()```, time passed since last ```LOG(...)```, file names, line numbers, printf-style formatted message.
	Note: This can eat up most of your microcontroller's memory!

## Library Configuration
Basically, all of the library configuration is done within the ```platformio.ini``` inside your project using ```build_flags```.
These parameters will be passed to the compiler and processed by the library. Feel free to re-use the parameters in your code.
Learn more: https://docs.platformio.org/en/stable/projectconf/section_env_build.html#build-flags

Following parameters and options are supported in the latest version of this library:

### Supported Parameters
- ```-D DEVNETXLOG``` Activates or deactivates devnetXLOG with the specified feature level. See: [Feature Levels](#feature-levels).
- ```-D DEVNETXLOG_BAUD_RATE``` Defines the Serial Baud Rate (default: ```57600UL```).
- ```-D DEVNETXLOG_BUFFER_SIZE``` Defines the printf buffer size in RAM (default: ```256U```).
	*Note:* The buffer is only used by ``` ARDUINO_ARCH_SAMD ```.
- ```-D PRODUCT_NAME``` Defines the product name, displayed in the ```LOGBegin(...)``` welcome message.
- ```-D FW_MAJOR``` Defines the major firmware version, displayed in the ```LOGBegin(...)``` welcome message.
- ```-D FW_MINOR``` Defines the minor firmware version, displayed in the ```LOGBegin(...)``` welcome message.
- ```-D FW_PATCH``` Defines the patch firmware version, displayed in the ```LOGBegin(...)``` welcome message.

### Supported Options
- ```-D DEVNETXLOG_PATHSEPARATOR_UNIX``` Enables path separators used by Linux and macOS.
- ```-D DEVNETXLOG_PATHSEPARATOR_WINDOWS``` Enables path separators used by Windows (enabled by default).
- ```-D DEVNETXLOG_NOPATHS``` Display only file names. Removes paths from the files in ```LOG(...)```. 
- ```-D DEVNETXLOG_LINEBREAK_UNIX ``` devnetXLOG will usw line breaks used by Linux and macOS.
	*Note:* This will also define ```__BR__``` as ```\n``` than can be used for line breaks in your code.
- ```-D DEVNETXLOG_LINEBREAK_WINDOWS ``` devnetXLOG will usw line breaks used by Windows.
	*Note:* This will also define ```__BR__``` as ```\r\n``` than can be used for line breaks in your code. (enabled by default).
- ```-D DEVNETXLOG_NOTERMINATION``` Disables the line termination defined by ```DEVNETXLOG_LINEBREAK_UNIX``` or ```DEVNETXLOG_LINEBREAK_WINDOWS``` during ```LOG(...)```.
- ```-D DEVNETXLOG_NOWELCOME``` Disables the devnetXLOG welcome message that is displayed during ```LOGBegin(...)```.


### Example Configuration: Minimal feature level
```ini
[env:]
build_flags =
	-D DEVNETXLOG=1
	-D DEVNETXLOG_NOWELCOME
```
### Example Configuration: Maximal feature level
```ini
[env:]
build_flags =
	'-D PRODUCT_NAME="devnetXLOG Example Firmware"'
	-D DEVNETXLOG_BAUD_RATE=115200
	-D DEVNETXLOG=3
	-D FW_MAJOR=1
	-D FW_MINOR=2
	-D FW_PATCH=3	
	-D DEVNETXLOG_NOPATHS
	-D DEVNETXLOG_LINEBREAK_UNIX
	-D DEVNETXLOG_PATHSEPARATOR_UNIX
```

## PlatformIO Device Monitor Configuration (optional)
You may want to improve your PlatformIO device monitor (aka Serial Terminal) e.g. by adding time stamps and other features.
This can be done by defining ```monitor_flags``` in the ```platformio.ini``` inside your project.

We use the following flags on Windows:
```ini
[env:]
monitor_flags =
	--quiet
	--echo
	--eol
	CRLF
	--filter
	colorize
	--filter
	time
```
For more details, see:
- https://docs.platformio.org/en/stable/projectconf/section_env_monitor.html#monitor-flags
- https://docs.platformio.org/en/stable/core/userguide/device/cmd_monitor.html#cmd-device-monitor-filters

---
Have fun! :)
