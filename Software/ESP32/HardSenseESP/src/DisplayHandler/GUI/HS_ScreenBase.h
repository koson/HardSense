#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../../GlobalDefinitions.h"
#include "../../HSSerial/SerialInterface.h"
#include "../../QueueItem.h"

#define HIDDEN_HOMESCREEN_TOUCH_PANEL_LOW_X 270
#define HIDDEN_HOMESCREEN_TOUCH_PANEL_HIGH_X 320
#define HIDDEN_HOMESCREEN_TOUCH_PANEL_LOW_Y 190
#define HIDDEN_HOMESCREEN_TOUCH_PANEL_HIGH_Y 240

class HS_ScreenBase
{
private:
	void HS_Load_Fonts();

public:
	HS_ScreenBase(Queues *newQueues, TFT_eSPI *newTFT);
	~HS_ScreenBase();

	String degreesC = "";
	char degreesC_char[3];

	TFT_eSPI *TFT;
	Queues *allQueues;
	TFT_eSprite* textPrinter_Sprite;

	virtual void UpdateScreen(String value);
	
	virtual void HandleTouch(int x, int y);
	bool HiddenHomeScreen_Touched(int x, int y);

	void DrawBoxWithBorderAndDropShadow(HS_Coords hs_coords, HS_Theme hs_theme);
	void DisplayFreeHeap();
};

