/*
 Name:		HardSenseESP.ino
 Created:	10/7/2019 7:07:38 AM
 Author:	Kitecraft
*/

#include "DisplayHandler/DisplayHandler.h"
#include "HSSerial/HSSerial.h"
#include <Queue.h>
#include "QueueItem.h"


TaskHandle_t TFT_Core_Handle;
DataQueue<QUEUE_ITEM> displayQueue(20);
DataQueue<QUEUE_ITEM> outputQueue(5);

DisplayHandler displayHandler;
HSSerial hsSerial;

byte btButton = 22;

int counter = 0;

portMUX_TYPE displayQueueMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE outputQueueMux = portMUX_INITIALIZER_UNLOCKED;


void setup() {
	Serial.begin(115200);
	displayHandler.Init(&displayQueue, AddItemToOutputQueue, displayQueueMux);
	
	InitButtons();

	QUEUE_ITEM qi;
	qi.key = DisplayCommands::ChangeScreen;
	sprintf(qi.value, "%c", ScreenTypes::SplashScreen);
	displayQueue.enqueue(qi);

	xTaskCreatePinnedToCore(
		TFT_Core_Proc,                  /* pvTaskCode */
		"DisplayHandlerTask",            /* pcName */
		4000,                   /* usStackDepth */
		NULL,                   /* pvParameters */
		1,                      /* uxPriority */
		&TFT_Core_Handle,                 /* pxCreatedTask */
		0);

	delay(2000);

	if (!hsSerial.Init(&outputQueue, AddItemToDisplayQueue, outputQueueMux))
	{
		Serial.println("Failed to init SPIFFS");
		Spin();
	}

	if (IsBTButtonPressed())
	{
		Serial.println("Starting bluetooth...");
		hsSerial.HandleBluetoothConnection();
	}

	hsSerial.HandleWiFiSocketConnection();

}


void loop() {
	
	if (hsSerial.connectedToSomething)
	{
		hsSerial.HandleInput();
		hsSerial.HandleOutput();
	} else 
	{
		//hsSerial.HandleWiFiSocketConnection();
	}
	
	delay(20);
}

void TFT_Core_Proc(void* parameter)
{
	displayHandler.Run();

	/*
	while (true)
	{
		HandleButtons();
		displayHandler.UpdateDisplay();
		delay(20);
	}
	*/

	delay(20);
}




void InitButtons()
{
	pinMode(btButton, INPUT_PULLUP);
}


bool IsBTButtonPressed()
{
	if (!digitalRead(btButton))
	{
		return true;
	}
	return false;
}


void AddItemToDisplayQueue(char key, char* value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	strcpy(qi.value, value);
	portENTER_CRITICAL(&displayQueueMux);
	displayQueue.enqueue(qi);
	portEXIT_CRITICAL(&displayQueueMux);
}

void AddItemToOutputQueue(char key, char* value)
{
	QUEUE_ITEM qi;
	qi.key = key;
	strcpy(qi.value, value);
	portENTER_CRITICAL(&outputQueueMux);
	outputQueue.enqueue(qi);
	portEXIT_CRITICAL(&outputQueueMux);
}

void Spin()
{
	while (true) {
		delay(100);
	}
}