#include <gui/screen1_screen/Screen1View.hpp>

#include <touchgfx/Bitmap.hpp>

#include <string.h>

#define SCREEN_WIDTH		320
#define SCREEN_HEIGHT		240

#define BITMAP_CACHE_SIZE	0x100000		//1MB Cache For Bitmap Data

#define BITMAP_EXTRA_BUFFER_ADDR D0100000

extern uint16_t* image_buffer;

extern uint8_t flag;
extern uint8_t image_fetched;
extern uint8_t exec_once;

BitmapId bmpId;

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();

	uint16_t* const cacheStartAddr = (uint16_t*)0xD0000000;

	Bitmap::setCache(cacheStartAddr, BITMAP_CACHE_SIZE, 1);

	bmpId = Bitmap::dynamicBitmapCreate(SCREEN_HEIGHT, SCREEN_WIDTH, Bitmap::RGB565);

	if(bmpId != BITMAP_INVALID)
	{
		image_buffer = (uint16_t*)Bitmap::dynamicBitmapGetAddress(bmpId);

		memset(image_buffer, 0xFF, SCREEN_WIDTH * SCREEN_HEIGHT * 2);
	}
	image1.setBitmap(Bitmap(bmpId));

	image1.setXY(0, 0);

	flag = 1;
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
	if((flag == 1) && (image_fetched == 1))
	{
		if(exec_once == 0)
		{
			image1.invalidate();

			exec_once = 1;
		}
	}
}
