#include <gui/screen1_screen/Screen1View.hpp>
#include <BitmapDatabase.hpp>
extern int Y;

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::handleTickEvent()
{
	gauge1.updateValue(Y, 0);
//    static int timeoutCounter = 0;
//    if (timeoutCounter++ == 46)
//    {
//        timeoutCounter = 0;
//    	gauge1.updateValue(y, 10);
//    }
}

void Screen1View::updateIconAndText()
{
    int gaugeValue = gauge1.getValue();
    if (gaugeValue < 150)
    {
        icon.setBitmap(Bitmap(BITMAP_GAUGE02_ICON_LEVEL00_ID));
    }
    else if (gaugeValue < 500)
    {
        icon.setBitmap(Bitmap(BITMAP_GAUGE02_ICON_LEVEL01_ID));
    }
    else if (gaugeValue < 670)
    {
        icon.setBitmap(Bitmap(BITMAP_GAUGE02_ICON_LEVEL02_ID));
    }
    else if (gaugeValue < 830)
    {
        icon.setBitmap(Bitmap(BITMAP_GAUGE02_ICON_LEVEL03_ID));
    }
    else
    {
        icon.setBitmap(Bitmap(BITMAP_GAUGE02_ICON_LEVEL04_ID));
    }
    icon.invalidate();

    Unicode::snprintf(readoutTextBuffer, READOUTTEXT_SIZE, "%i", gaugeValue);
    readoutText.invalidate();
}

void Screen1View::newGaugeValue()
{
    // The icon and text should be updated each time the gauge/needle has
    // a new value during the animation
    updateIconAndText();
}
