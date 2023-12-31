/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/CustomListElementBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <images/BitmapDatabase.hpp>

CustomListElementBase::CustomListElementBase()
{
    setWidth(320);
    setHeight(40);
    box.setPosition(0, 0, 320, 40);
    box.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    add(box);

    title.setPosition(0, 6, 320, 25);
    title.setColor(touchgfx::Color::getColorFromRGB(22, 25, 29));
    title.setLinespacing(0);
    Unicode::snprintf(titleBuffer, TITLE_SIZE, "%s", touchgfx::TypedText(T___SINGLEUSE_FHWH).getText());
    title.setWildcard(titleBuffer);
    title.setTypedText(touchgfx::TypedText(T___SINGLEUSE_TFL1));
    add(title);

    divider.setXY(0, 36);
    divider.setBitmap(touchgfx::Bitmap(BITMAP_DIVIDER_ID));
    add(divider);
}

CustomListElementBase::~CustomListElementBase()
{

}

void CustomListElementBase::initialize()
{

}
