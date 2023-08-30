#include <gui/main_screen/MainView.hpp>

MainView::MainView()
{

}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();

    tickCounter = presenter->getTick();
	analogHours = presenter->getHour();
	analogMinutes = presenter->getMinute();
	analogSeconds = presenter->getSecond();

	analogClock.setTime24Hour(analogHours, analogMinutes, analogSeconds);
}

void MainView::tearDownScreen()
{
	presenter->saveTick(tickCounter);
	presenter->saveHour(analogHours);
	presenter->saveMinute(analogMinutes);
	presenter->saveSecond(analogSeconds);

    MainViewBase::tearDownScreen();
}

void MainView::handleTickEvent()
{
	tickCounter++;
	if (tickCounter % 60 == 0)
	{
		if (++analogSeconds >= 60)
		{
			analogSeconds = 0;
			if (++analogMinutes >= 60)
			{
				analogMinutes = 0;
				if (++analogHours >= 24)
				{
					analogHours = 0;
				}
			}
		}
	}

    analogClock.setTime24Hour(analogHours, analogMinutes, analogSeconds);
}
