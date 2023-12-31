/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SCREEN1VIEWBASE_HPP
#define SCREEN1VIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TiledImage.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Gauge.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

class Screen1ViewBase : public touchgfx::View<Screen1Presenter>
{
public:
    Screen1ViewBase();
    virtual ~Screen1ViewBase();
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void newGaugeValue()
    {
        // Override and implement this function in Screen1
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::TiledImage background;
    touchgfx::Button btnNext;
    touchgfx::Gauge gauge1;
    touchgfx::Image icon;
    touchgfx::TextAreaWithOneWildcard readoutText;

    /*
     * Wildcard Buffers
     */
    static const uint16_t READOUTTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar readoutTextBuffer[READOUTTEXT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Screen1ViewBase, const touchgfx::AbstractProgressIndicator&> gaugeValueSetCallback;
    touchgfx::Callback<Screen1ViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void gaugeValueSetCallbackHandler(const touchgfx::AbstractProgressIndicator& src);
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // SCREEN1VIEWBASE_HPP
