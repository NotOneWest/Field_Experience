#ifndef MAINPRESENTER_HPP
#define MAINPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainView;

class MainPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MainPresenter(MainView& v);

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

    virtual ~MainPresenter() {};

    void saveTick(int16_t tickCounter)
	{
		model -> saveTick(tickCounter);
	}

	void saveHour(int16_t digitalHours)
	{
		model -> saveHour(digitalHours);
	}

	void saveMinute(int16_t digitalMinutes)
	{
		model -> saveMinute(digitalMinutes);
	}

	void saveSecond(int16_t digitalSeconds)
	{
		model -> saveSecond(digitalSeconds);
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
    MainPresenter();

    MainView& view;
};

#endif // MAINPRESENTER_HPP
