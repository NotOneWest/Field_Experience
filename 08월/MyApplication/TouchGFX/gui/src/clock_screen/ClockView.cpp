#include <gui/clock_screen/ClockView.hpp>

ClockView::ClockView()
{

}

void ClockView::setupScreen()
{
    ClockViewBase::setupScreen();

    tickCounter = presenter->getTick();
    digitalHours = presenter->getHour();
    digitalMinutes = presenter->getMinute();
    digitalSeconds = presenter->getSecond();

    digitalClock.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);
}

void ClockView::tearDownScreen()
{
	presenter->saveTick(tickCounter);
	presenter->saveHour(digitalHours);
	presenter->saveMinute(digitalMinutes);
	presenter->saveSecond(digitalSeconds);

    ClockViewBase::tearDownScreen();
}

void ClockView::btnHourUpClicked()
{
    Hours = (Hours + 1) % 24; // Keep new value in range 0..23
    Unicode::snprintf(hourNumBuffer, HOURNUM_SIZE, "%02d", Hours);
    hourNum.invalidate();
}

void ClockView::btnHourDownClicked()
{
	Hours = (Hours + 24 - 1) % 24; // Keep new value in range 0..23
    Unicode::snprintf(hourNumBuffer, HOURNUM_SIZE, "%02d", Hours);
    hourNum.invalidate();
}

void ClockView::btnMinUpClicked()
{
    Minutes = (Minutes + 1) % 60; // Keep new value in range 0..59
    Unicode::snprintf(minNumBuffer, MINNUM_SIZE, "%02d", Minutes);
    minNum.invalidate();
}

void ClockView::btnMinDownClicked()
{
    Minutes = (Minutes + 60 - 1) % 60; // Keep new value in range 0..59
    Unicode::snprintf(minNumBuffer, MINNUM_SIZE, "%02d", Minutes);
    minNum.invalidate();
}

void ClockView::btnSecUpClicked()
{
    Seconds = (Seconds + 1) % 60; // Keep new value in range 0..59
    Unicode::snprintf(secNumBuffer, SECNUM_SIZE, "%02d", Seconds);
    secNum.invalidate();
}

void ClockView::btnSecDownClicked()
{
    Seconds = (Seconds + 60 - 1) % 60; // Keep new value in range 0..59
    Unicode::snprintf(secNumBuffer, SECNUM_SIZE, "%02d", Seconds);
    secNum.invalidate();
}

void ClockView::btnSaveTimeClicked()
{
	digitalHours = Hours; digitalMinutes = Minutes; digitalSeconds = Seconds;
	Hours=0; Minutes=0; Seconds=0;
}

void ClockView::handleTickEvent()
{
	tickCounter++;
	if (tickCounter % 60 == 0)
	{
		if (++digitalSeconds >= 60)
		{
			digitalSeconds = 0;
			if (++digitalMinutes >= 60)
			{
				digitalMinutes = 0;
				if (++digitalHours >= 24)
				{
					digitalHours = 0;
				}
			}
		}
	}
	// Update the clocks
    digitalClock.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);
}
