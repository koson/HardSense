/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include <SPI.h>
#include <TFT_eSPI.h>
#include <FS.h>
#include <SPIFFS.h>

#include "Bluetooth/BTConfigurator.h"

byte btButton = 22;
BTConfigurator *btConfig;

TFT_eSPI tftDisplay = TFT_eSPI();

void setup() {
	tftDisplay.init();
	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);

	Serial.begin(115200);
	pinMode(btButton, INPUT_PULLUP);
	
	tftDisplay.setCursor(20, 20);
	if (!SystemChecks()) {
		Spin();
	}

	if (!digitalRead(btButton))
	{
		tftDisplay.print("Entering Bluetooth Configurator");
		btConfig = new BTConfigurator();

	}
	else
	{
		tftDisplay.print("Entering Normal Run Mode");
	}


}


void loop() {
  
}


bool SystemChecks()
{
	if (!SPIFFS.begin()) {
		tftDisplay.print("SPIFFS is not available.");
		return false;
	}

	return true;
}

void Spin()
{
	while (true) {
		delay(100);
	}
}