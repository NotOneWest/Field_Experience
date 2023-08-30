#ifndef CLOCKVIEW_HPP
#define CLOCKVIEW_HPP

#include <gui_generated/clock_screen/ClockViewBase.hpp>
#include <gui/clock_screen/ClockPresenter.hpp>

class ClockView : public ClockViewBase
{
public:
    ClockView();
    virtual ~ClockView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void btnHourUpClicked();
	virtual void btnHourDownClicked();
	virtual void btnMinUpClicked();
	virtual void btnMinDownClicked();
	virtual void btnSecUpClicked();
	virtual void btnSecDownClicked();
	virtual void btnSaveTimeClicked();

    virtual void handleTickEvent();
protected:
    int16_t tickCounter;
    int16_t Hours=0, Minutes=0, Seconds=0;
	int16_t digitalHours;
	int16_t digitalMinutes;
	int16_t digitalSeconds;
};

#endif // CLOCKVIEW_HPP
