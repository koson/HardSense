# HardSense
Monitor local system sensors and display that data on a remote 2.2" LCD screen.

https://youtu.be/yXnQ3QwIAQI


The hardware monitoring is provided by the Open Hardware Monitor library:  https://github.com/openhardwaremonitor/openhardwaremonitor

NOTE:
To buld HardSenseESP, you need to change the partition scheme.

HardSenseESP Tools configuration settings:
	Board:  "ESP32 Dev Module"
	Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS)"

In Arduino IDE, these are under the 'Tools' menu item.
In vMicro (Visual Micro for Visual Studio), they are under 'vMicro' menu item.

