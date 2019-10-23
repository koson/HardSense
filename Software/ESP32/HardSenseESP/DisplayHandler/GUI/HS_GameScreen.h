#pragma once
#include "HS_ScreenBase.h"
//#include "Panels/HS_NetPanel.h"
#include "Panels/HS_MemPanel.h"
#include "Panels/HS_GPU_TempAndFanChart_Panel.h"
#include "Widgets/HS_Dial_Widget.h"

/*  Light Blue
#define TEXT_COLOR TFT_WHITE
#define PANEL_HCOLOR 0x3BB6
#define PANEL_BGCOLOR 0x7D19
#define BOX_BORDER_COLOR 0x9DDB
#define BOX_DROP_SHADOW 0x63F2
*/

#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208

//#define NET_PANEL_X 227
//#define NET_PANEL_Y 0
#define RAM_PANEL_X 227
#define RAM_PANEL_Y 0

#define GPU_TEMP_PANEL_X 0
#define GPU_TEMP_PANEL_Y 0

#define SCREEN_GAME_LOAD_DIAL_MIN -120
#define SCREEN_GAME_LOAD_DIAL_MAX 120

#define SCREEN_GAME_GPU_LOAD_DIAL_X 228
#define SCREEN_GAME_GPU_LOAD_DIAL_Y 56

class HS_GameScreen :
	public HS_ScreenBase
{
private:
	HS_Theme gameScreenTheme;

	HS_GPU_TempAndFanChart_Panel* GPU_TempAndFanChart;
	void Draw_Temp_Panel();

	//HS_NetPanel *netPanel;
	//void Draw_Net_Panel();
	HS_MemPanel* memPanel;
	double gpuMemoryTotal = 1;
	void Draw_Mem_Panel();

	HS_Dial_Widget* gpuCoreLoadWidget;
	uint16_t gpuCoreLoadDial_CurrentRingColor;
	void DrawGPUCoreLoadPanel();
	void UpdateGPUCoreLoad(double load);

	unsigned long lastUpdate = 0;
	int gpuTempAndFanGraphUpdateTime = 500;

public:
	HS_GameScreen(TFT_eSPI* newTFT);
	~HS_GameScreen();
	void UpdateScreen(String value);
	void UpdateScreenOnInterval();
	void SetSensorList(void(*AddItemToOutputQueue_func)(char key, String value));
	void HandleTouch(int x, int y);
};
