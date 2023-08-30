#ifndef CLOCKPRESENTER_HPP
#define CLOCKPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ClockView;

class ClockPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ClockPresenter(ClockView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~ClockPresenter() {};

    void saveTick(int16_t tickCounter)
	{
    	model -> saveTick(tickCounter);
	}

	void saveHour(int16_t analogHours)
	{
		model -> saveHour(analogHours);
	}

	void saveMinute(int16_t analogMinutes)
	{
		model -> saveMinute(analogMinutes);
	}

	void saveSecond(int16_t analogSeconds)
	{
		model -> saveSecond(analogSeconds);
	}

	int16_t getTick()
	{
		return model -> getTick();
	}

	int16_t getHour()
	{
		return model -> getHour();
	}

	int16_t getMinute()
	{
		return model -> getMinute();
	}

	int16_t getSecond()
	{
		return model -> getSecond();
	}

private:
    ClockPresenter();

    ClockView& view;
};

#endif // CLOCKPRESENTER_HPP
