/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/main_screen/MainViewBase.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>

MainViewBase::MainViewBase() :
    buttonCallback(this, &MainViewBase::buttonCallbackHandler)
{
    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    background.setBitmap(touchgfx::Bitmap(BITMAP_BACKGROUND_ID));
    background.setPosition(0, 0, 480, 272);
    background.setOffset(0, 0);
    add(background);

    digitalClock.setPosition(34, 46, 150, 30);
    digitalClock.setColor(touchgfx::Color::getColorFromRGB(0, 240, 255));
    digitalClock.setTypedText(touchgfx::TypedText(T___SINGLEUSE_D757));
    digitalClock.displayLeadingZeroForHourIndicator(true);
    digitalClock.setDisplayMode(touchgfx::DigitalClock::DISPLAY_24_HOUR);
    digitalClock.setTime24Hour(12, 0, 0);
    add(digitalClock);

    analogClock.setXY(202, 15);
    analogClock.setBackground(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_ANALOGCLOCK_BACKGROUNDS_SMALL_GLOSSY_DARK_ID, 120, 120);
    analogClock.setupSecondHand(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_ANALOGCLOCK_HANDS_SMALL_SECOND_GLOSSY_DARK_ID, 2, 100);
    analogClock.setupMinuteHand(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_ANALOGCLOCK_HANDS_SMALL_MINUTE_GLOSSY_DARK_ID, 10, 87);
    analogClock.setMinuteHandSecondCorrection(false);
    analogClock.setupHourHand(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_ANALOGCLOCK_HANDS_SMALL_HOUR_GLOSSY_DARK_ID, 9, 69);
    analogClock.setHourHandMinuteCorrection(false);
    analogClock.initializeTime24Hour(10, 10, 0);
    analogClock.setAnimation(10, touchgfx::EasingEquations::cubicEaseInOut);
    add(analogClock);

    btnHourUp.setXY(34, 76);
    btnHourUp.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_UP_50_50_00F0FF_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_UP_50_50_000000_SVG_ID));
    btnHourUp.setAction(buttonCallback);
    add(btnHourUp);

    btnHourDown.setXY(34, 126);
    btnHourDown.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_DOWN_50_50_00F0FF_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_DOWN_50_50_000000_SVG_ID));
    btnHourDown.setAction(buttonCallback);
    add(btnHourDown);

    btnMinUp.setXY(84, 76);
    btnMinUp.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_UP_50_50_00F0FF_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_UP_50_50_000000_SVG_ID));
    btnMinUp.setAction(buttonCallback);
    add(btnMinUp);

    btnMinDown.setXY(84, 126);
    btnMinDown.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_DOWN_50_50_00F0FF_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_DOWN_50_50_000000_SVG_ID));
    btnMinDown.setAction(buttonCallback);
    add(btnMinDown);

    btnSecUp.setXY(134, 76);
    btnSecUp.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_UP_50_50_00F0FF_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_UP_50_50_000000_SVG_ID));
    btnSecUp.setAction(buttonCallback);
    add(btnSecUp);

    btnSecDown.setXY(134, 126);
    btnSecDown.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_DOWN_50_50_00F0FF_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_HARDWARE_KEYBOARD_ARROW_DOWN_50_50_000000_SVG_ID));
    btnSecDown.setAction(buttonCallback);
    add(btnSecDown);

    btnPlay.setXY(84, 176);
    btnPlay.setBitmaps(touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_AV_PLAY_CIRCLE_OUTLINE_50_50_00F0FF_SVG_ID), touchgfx::Bitmap(BITMAP_ICON_THEME_IMAGES_AV_PAUSE_CIRCLE_OUTLINE_50_50_00F0FF_SVG_ID));
    btnPlay.setAction(buttonCallback);
    add(btnPlay);

    btnNex.setXY(435, 226);
    btnNex.setBitmaps(touchgfx::Bitmap(BITMAP_NEXT_BUTTON_RELEASED_ID), touchgfx::Bitmap(BITMAP_NEXT_BUTTON_PRESSED_ID));
    btnNex.setAction(buttonCallback);
    add(btnNex);
}

MainViewBase::~MainViewBase()
{

}

void MainViewBase::setupScreen()
{

}

void MainViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &btnHourUp)
    {
        //HourUp
        //When btnHourUp clicked call virtual function
        //Call buttonHourUpClicked
        buttonHourUpClicked();
    }
    if (&src == &btnHourDown)
    {
        //HourDown
        //When btnHourDown clicked call virtual function
        //Call buttonHourDownClicked
        buttonHourDownClicked();
    }
    if (&src == &btnSecUp)
    {
        //SecondUp
        //When btnSecUp clicked call virtual function
        //Call buttonSecondUpClicked
        buttonSecondUpClicked();
    }
    if (&src == &btnSecDown)
    {
        //SecondDown
        //When btnSecDown clicked call virtual function
        //Call buttonSecondDownClicked
        buttonSecondDownClicked();
    }
    if (&src == &btnPlay)
    {
        //PausePlay
        //When btnPlay clicked call virtual function
        //Call buttonPausePlayClicked
        buttonPausePlayClicked();
    }
    if (&src == &btnNex)
    {
        //ChangeScreen
        //When btnNex clicked change screen to Screen1
        //Go to Screen1 with screen transition towards East
        application().gotoScreen1ScreenSlideTransitionEast();
    }
    if (&src == &btnMinUp)
    {
        //MinuteUp
        //When btnMinUp clicked call virtual function
        //Call buttonMinuteUpClicked
        buttonMinuteUpClicked();
    }
    if (&src == &btnMinDown)
    {
        //MinuteDown
        //When btnMinDown clicked call virtual function
        //Call buttonMinuteDownClicked
        buttonMinuteDownClicked();
    }
}
