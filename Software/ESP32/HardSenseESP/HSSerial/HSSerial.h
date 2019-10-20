#pragma once
#include "Arduino.h"
#include <WiFi.h>
#include <BluetoothSerial.h>
#include "SerialInterface.h"
#include "../HSFileSystem/HSFileSystem.h"
#include <Queue.h>
#include "../QueueItem.h"

#define MAX_HEARTBEATS_MISSED 3
#define MAX_BUF_SIZE 256

class HSSerial
{
private:
	WiFiClient *wifiSerial;
	BluetoothSerial *btSerial;
	S_SETTNGS hardsenseSettings;
	
	DataQueue<QUEUE_ITEM>* outputDataQueue;
	portMUX_TYPE outputDataMux;
	portMUX_TYPE outputQueueMux;

	char* OutputData;
	int OutputDataLength = 0;

	int(HSSerial::*InputAvailable)();
	int(HSSerial::*ReadInputByte)();
	String(HSSerial::*ReadInputStringUntil)(char);
	int(HSSerial::*PrintMessageToOutput)(char);

	bool WaitForBTConnection();
	void AcceptNewBTConnection();
	void NewSocketRequestAccepted();

	int BT_Available();
	int BT_Read();
	int BT_PrintChar(char c);
	String BT_ReadStringUntil(char terminator);
	int WiFi_Available();
	int WiFi_Read();
	String WiFi_ReadStringUntil(char terminator);
	int WiFi_PrintChar(char c);
	void ParseInput(String input);
	void DispatchCommand(char key, String val);

	bool IsPasswordSet();
	bool UpdateSetting(char key, String value);
	bool SaveSettingsToFS();
	
	//void UpdateSensorValuesToDisplay(String value);

	void(*AddItemToDisplayQueue)(char key, String value);

	portMUX_TYPE heartbeatMux;
	int heartbeatCounter;
	bool IncrementHeartbeatCounter();
	void(*HeartbeatTimerEnabled)(bool);
	bool AddHeartbeatToOutput = false;

public:
	HSSerial();
	~HSSerial();
	bool Init(DataQueue<QUEUE_ITEM> *newOutputQueue, portMUX_TYPE &newOutputQueueMux, void(*AddItemToDisplayQueue_Func)(char key, String value), void(*HeartbeatTimerEnabled_Func)(bool));


	void AddKeyToOutputMessage(TRANS__KEY key);
	void AddIntToOutputMessage(TRANS__KEY key, int val);
	void AddBoolToOutputMessage(TRANS__KEY key, bool value);
	void AddStringToOutputMessage(TRANS__KEY key, String value);
	void AddStringToOutputMessage(TRANS__KEY key, char *value);

	bool ConnectedToWifi;
	bool connectedToSomething = false;
	void HandleBluetoothConnection();
	void ConnectToHardsenseServer();
	void HandleWiFiConnection();
	void HandleInput();
	void HandleOutput();

	void IRAM_ATTR FireHeartbeat();
};

