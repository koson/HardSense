#include "DisplayHandler.h"
#include "../HSSerial/SerialInterface.h"
#include "GUI/SplashScreen.h"
#include "GUI/Screen_ConnectToNetwork.h"
#include "GUI/Screen_HomeScreen.h"
#include "GUI/Screen_BluetoothConfigurator.h"

DisplayHandler::DisplayHandler()
{
}

DisplayHandler::~DisplayHandler()
{

}

void DisplayHandler::Init(DataQueue<QUEUE_ITEM>* newDisplayQueue, portMUX_TYPE& newDisplayQueueMux, void(*AddItemToOutputQueue_Func)(char key, char* value))
{
	displayDataQueue = newDisplayQueue;
	AddItemToOutputQueue = AddItemToOutputQueue_Func;
	displayQueueMux = newDisplayQueueMux;

	tftDisplay.init();
	tftDisplay.setRotation(1);
	tftDisplay.fillScreen(TFT_BLACK);
}

void DisplayHandler::Run()
{
	while (true)
	{
		DispatchCommand();
		//HandleButtons();
		delay(20);
	}

}

void DisplayHandler::LoadNewScreen(char screenID)
{
	//Serial.println("LoadNewScreen");
	if (DestoryCurrentScreen != NULL) {
		DestoryCurrentScreen(tftDisplay);
		DestoryCurrentScreen = NULL;
		UpdateCureentScreen = NULL;
	}
	AddItemToOutputQueue(TRANS__KEY::CLEAR_SENSOR_LIST, "");

	char key = screenID;
	switch (key) {
	case ScreenTypes::SplashScreen:
		DestoryCurrentScreen = Destroy_SplashScreen;
		UpdateCureentScreen = Update_SplashScreen;
		Create_SplashScreen(tftDisplay);
		break;
	case ScreenTypes::ConnectToNetwork:
		DestoryCurrentScreen = Destroy_Screen_ConnectToNetwork;
		UpdateCureentScreen = Update_Screen_ConnectToNetwork;
		Create_Screen_ConnectToNetwork(tftDisplay);
		break;
	case ScreenTypes::Home:
		DestoryCurrentScreen = Destroy_Screen_Home;
		UpdateCureentScreen = Update_Screen_Home;
		Create_Screen_Home(tftDisplay);
		AddItemToOutputQueue(TRANS__KEY::ADD_SENSORS_TO_SENSOR_LIST, Screen_Home_SensorList());
		break;
	case ScreenTypes::BluetoothConfigurator:
		DestoryCurrentScreen = Destroy_Screen_BluetoothConfigurator;
		UpdateCureentScreen = Update_Screen_BluetoothConfigurator;
		Create_Screen_BluetoothConfigurator(tftDisplay);
		break;
	default:
		break;
	}
}

void DisplayHandler::DispatchCommand()
{
	while (!displayDataQueue->isEmpty())
	{
		portENTER_CRITICAL(&displayQueueMux);
		QUEUE_ITEM currItem = displayDataQueue->dequeue();
		portEXIT_CRITICAL(&displayQueueMux);

		switch (currItem.key) {
		case DisplayCommands::ChangeScreen:
			LoadNewScreen(currItem.value[0]);
			break;
		case DisplayCommands::UpdateValue:
			//Serial.printf("DisplayHandler::DispatchCommand()");
			if (UpdateCureentScreen != NULL) {
				//Serial.printf("Updateing Screen with sensor data: '%s'\n",currItem.value);
				UpdateCureentScreen(tftDisplay, currItem.value);
			}
			break;
		default:
			break;
		}
	}
}