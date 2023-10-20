#include <gui/main_screen/MainView.hpp>

MainView::MainView() :
    tickCounter(0)
{
}

void MainView::setupScreen()
{
    MainViewBase::setupScreen();
    digitalHours = digitalClock.getCurrentHour();
    digitalMinutes = digitalClock.getCurrentMinute();
    digitalSeconds = digitalClock.getCurrentSecond();

    analogHours = analogClock.getCurrentHour();
    analogMinutes = analogClock.getCurrentMinute();
    analogSeconds = analogClock.getCurrentSecond();
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}

void MainView::buttonHourUpClicked(){
	if (++digitalHours >= 24) digitalHours = 0;
	if (++analogHours >= 24) analogHours = 0;
}

void MainView::buttonHourDownClicked()
{
	if (--digitalHours < 0) digitalHours = 23;
	if (--analogHours < 0) analogHours = 23;
}

void MainView::buttonMinuteUpClicked()
{
	if (++digitalMinutes >= 60){
        digitalMinutes = 0;
        if (++digitalHours >= 24) digitalHours = 0;
    }
    
    if (++analogMinutes >= 60){
        analogMinutes = 0;
        if (++analogHours >= 24) analogHours = 0;
    }
}

void MainView::buttonMinuteDownClicked()
{
    if (--digitalMinutes < 0){
        digitalMinutes = 59;
        if (--digitalHours < 0) digitalHours = 23;
    }
    
    if (--analogMinutes < 0){
        analogMinutes = 59;
        if (--analogHours < 0) analogHours = 23;
    }
}

void MainView::buttonSecondUpClicked(){
	if (++digitalSeconds >= 60){
        digitalSeconds = 0;
        if (++digitalMinutes >= 60){
            digitalMinutes = 0;
            if (++digitalHours >= 24) digitalHours = 0;
        }
    }

    if (++analogSeconds >= 60){
        analogSeconds = 0;
        if (++analogMinutes >= 60){
            analogMinutes = 0;
            if (++analogHours >= 24) analogHours = 0;
        }
    }
}

void MainView::buttonSecondDownClicked()
{
	if (--digitalSeconds < 0) {
        digitalSeconds = 59;
        if (--digitalMinutes < 0) {
	        digitalMinutes = 59;
	        if (--digitalHours < 0) digitalHours = 23;
	    }
    }

    if (--analogSeconds < 0){
        analogSeconds = 59;
        if (--analogMinutes < 0){
	        analogMinutes = 59;
	        if (--analogHours < 0) analogHours = 23;
	    }
    }
}

void MainView::buttonPausePlayClicked()
{
	if(btnPlay.getState()) flag=1;
	else flag=0;
}

void MainView::handleTickEvent()
{
    if(flag){
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
	}
	// Update the clocks
    digitalClock.setTime24Hour(digitalHours, digitalMinutes, digitalSeconds);
    analogClock.setTime24Hour(analogHours, analogMinutes, analogSeconds);
}
