#ifndef MAIN_VIEW_HPP
#define MAIN_VIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>

class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    
    virtual void buttonHourUpClicked();
    virtual void buttonHourDownClicked();
    virtual void buttonMinuteUpClicked();
    virtual void buttonMinuteDownClicked();
    virtual void buttonSecondUpClicked();
    virtual void buttonSecondDownClicked();
    virtual void buttonPausePlayClicked();

    virtual void handleTickEvent();
protected:
    int tickCounter;
    int analogHours;
    int analogMinutes;
    int analogSeconds;
    int digitalHours;
    int digitalMinutes;
    int digitalSeconds;
    int flag=0;
};

#endif // MAIN_VIEW_HPP
