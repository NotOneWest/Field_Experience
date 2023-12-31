/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

Screen1ViewBase::Screen1ViewBase() :
    gaugeValueSetCallback(this, &Screen1ViewBase::gaugeValueSetCallbackHandler),
    buttonCallback(this, &Screen1ViewBase::buttonCallbackHandler)
{
    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    background.setBitmap(touchgfx::Bitmap(BITMAP_BACKGROUND_ID));
    background.setPosition(0, 0, 480, 272);
    background.setOffset(0, 0);
    add(background);

    btnNext.setXY(435, 226);
    btnNext.setBitmaps(touchgfx::Bitmap(BITMAP_NEXT_BUTTON_RELEASED_ID), touchgfx::Bitmap(BITMAP_NEXT_BUTTON_PRESSED_ID));
    btnNext.setAction(buttonCallback);
    add(btnNext);

    gauge1.setBackground(touchgfx::Bitmap(BITMAP_GAUGE02_BACKGROUND_ID));
    gauge1.setPosition(130, 26, 220, 220);
    gauge1.setCenter(110, 110);
    gauge1.setStartEndAngle(-130, 130);
    gauge1.setRange(0, 1000);
    gauge1.setValue(0);
    gauge1.setEasingEquation(touchgfx::EasingEquations::cubicEaseInOut);
    gauge1.setNeedle(BITMAP_GAUGE02_NEEDLE_ID, 13, 50);
    gauge1.setMovingNeedleRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);
    gauge1.setSteadyNeedleRenderingAlgorithm(touchgfx::TextureMapper::BILINEAR_INTERPOLATION);
    gauge1.setValueSetAction(gaugeValueSetCallback);
    add(gauge1);

    icon.setXY(219, 198);
    icon.setBitmap(touchgfx::Bitmap(BITMAP_GAUGE02_ICON_LEVEL00_ID));
    add(icon);

    readoutText.setPosition(210, 159, 60, 39);
    readoutText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    readoutText.setLinespacing(0);
    Unicode::snprintf(readoutTextBuffer, READOUTTEXT_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_50X2).getText());
    readoutText.setWildcard(readoutTextBuffer);
    readoutText.setTypedText(touchgfx::TypedText(T___SINGLEUSE_MLHW));
    add(readoutText);
}

Screen1ViewBase::~Screen1ViewBase()
{

}

void Screen1ViewBase::setupScreen()
{

}

void Screen1ViewBase::gaugeValueSetCallbackHandler(const touchgfx::AbstractProgressIndicator& src)
{
    if (&src == &gauge1)
    {
        //newGaugeValue
        //When gauge1 gauge value set call virtual function
        //Call newGaugeValue
        newGaugeValue();
    }
}

void Screen1ViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btnNext)
    {
        //ChangeScreen
        //When btnNext clicked change screen to Main
        //Go to Main with screen transition towards East
        application().gotoMainScreenWipeTransitionEast();
    }
}
