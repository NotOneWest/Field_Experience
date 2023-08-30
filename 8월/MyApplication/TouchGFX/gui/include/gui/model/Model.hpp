#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/hal/types.hpp>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener) { modelListener = listener; }

    void tick();

    void saveTick(int16_t saveTick) { tickCounter = saveTick; }
    void saveHour(int16_t saveHour) { hour = saveHour; }
	void saveMinute(int16_t saveMinute) { minute = saveMinute; }
	void saveSecond(int16_t saveSecond) { second = saveSecond; }

	int16_t getTick() { return tickCounter; }
	int16_t getHour() { return hour; }
	int16_t getMinute() { return minute; }
	int16_t getSecond() { return second; }

	void SetmusicSelected(int itemSelected);
	int GetMusic(void);

protected:
    ModelListener* modelListener;
    int16_t tickCounter, hour,  minute, second;
    int MusicTitle;
};

#endif // MODEL_HPP
