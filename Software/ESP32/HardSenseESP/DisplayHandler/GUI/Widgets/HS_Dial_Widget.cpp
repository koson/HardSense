#include "HS_Dial_Widget.h"
#include "FS.h"
#include "SPIFFS.h"


#define AA_FONT_SMALL "SegoeUI-14"
#define AA_FONT_LARGE "SegoeUI-18"

HS_Dial_Widget::HS_Dial_Widget(TFT_eSPI TFT)
{

	if (!SPIFFS.begin()) {
		Serial.println("HS_Dial_Widget:: SPIFFS initialisation failed!");
		while (1) yield(); // Stay here twiddling thumbs waiting
	}

	// ESP32 will crash if any of the fonts are missing
	bool font_missing = false;
	if (SPIFFS.exists("/SegoeUI-14.vlw") == false) font_missing = true;
	if (SPIFFS.exists("/SegoeUI-18.vlw") == false) font_missing = true;

	if (font_missing)
	{
		Serial.println("\r\nHS_Dial_Widget:: Font missing in SPIFFS, did you upload it?");
		while (1) yield();
	}
	else Serial.println("\r\nHS_Dial_Widget:: Fonts found OK.");

	dial = new TFT_eSprite(&TFT);
	needle = new TFT_eSprite(&TFT);
	// Create the dial Sprite
	dial->setColorDepth(8);       // Size is odd (i.e. 91) so there is a centre pixel at 45,45
	dial->createSprite(91, 91);   // 8bpp requires 91 * 91 = 8281 bytes
	dial->setPivot(45, 45);       // set pivot in middle of dial Sprite
	dial->loadFont(AA_FONT_SMALL);

	CreateNeedle();
}

HS_Dial_Widget::~HS_Dial_Widget()
{
	delete(dial);
	delete(dial);
}

void HS_Dial_Widget::DrawDialScale(TFT_eSPI TFT, int16_t start_angle, int16_t end_angle, int16_t increment, uint16_t ringColor)
{
	// Draw dial outline
	dial->fillSprite(TFT_TRANSPARENT);           // Fill with transparent colour
	dial->fillCircle(45, 45, 43, ringColor);  // Draw dial outer

	// Hijack the use of the needle Sprite since that has not been used yet!

	TFT_eSprite tickSprite = TFT_eSprite(&TFT);
	tickSprite.setColorDepth(8);
	tickSprite.createSprite(3, 3);     // 3 pixels wide, 3 high
	tickSprite.fillSprite(TFT_WHITE);  // Fill with white
	tickSprite.setPivot(1, 43);        //  Set pivot point x to the Sprite centre and y to marker radius

	for (int16_t angle = start_angle; angle <= end_angle; angle += increment) {
		tickSprite.pushRotated(dial, angle); // Sprite is used to make scale markers
		yield(); // Avoid a watchdog time-out
	}
}


void HS_Dial_Widget::CreateNeedle()
{
	needle->setColorDepth(8);
	needle->createSprite(11, 49); // create the needle Sprite 11 pixels wide by 49 high

	needle->fillSprite(TFT_BLACK);          // Fill with black

	// Define needle pivot point
	uint16_t piv_x = needle->width() / 2;   // x pivot of Sprite (middle)
	uint16_t piv_y = needle->height() - 10; // y pivot of Sprite (10 pixels from bottom)
	needle->setPivot(piv_x, piv_y);         // Set pivot point in this Sprite

	// Draw the red needle with a yellow tip
	// Keep needle tip 1 pixel inside dial circle to avoid leaving stray pixels
	needle->fillRect(piv_x - 1, 2, 3, piv_y + 8, TFT_RED);
	needle->fillRect(piv_x - 1, 2, 3, 5, TFT_YELLOW);

	// Draw needle centre boss
	needle->fillCircle(piv_x, piv_y, 5, TFT_MAROON);
	needle->drawPixel(piv_x, piv_y, TFT_WHITE);     // Mark needle pivot point with a white pixel
}

void HS_Dial_Widget::PlotDial(int16_t x, int16_t y, int16_t angle, String label, float val)
{
	DrawEmptyDial(label, val);

	// Push a rotated needle Sprite to the dial Sprite, with black as transparent colour
	needle->pushRotated(dial, angle, TFT_BLACK); // dial is the destination Sprite

	// Push the resultant dial Sprite to the screen, with transparent colour
	dial->pushSprite(x, y, TFT_TRANSPARENT);
}

void HS_Dial_Widget::DrawEmptyDial(String label, float val)
{
	// Draw black face
	dial->fillCircle(45, 45, 40, TFT_BLACK);
	dial->drawPixel(45, 45, TFT_WHITE);        // For demo only, mark pivot point with a while pixel
	
	dial->setTextDatum(TC_DATUM);              // Draw dial text
	dial->drawString(label, 45, 15, 2);
	dial->drawFloat(val, 1, 45, 60, 2);
}