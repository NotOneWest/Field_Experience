#ifndef MUSICVIEW_HPP
#define MUSICVIEW_HPP

#include <gui_generated/music_screen/MusicViewBase.hpp>
#include <gui/music_screen/MusicPresenter.hpp>

class MusicView : public MusicViewBase
{
public:
    MusicView();
    virtual ~MusicView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void SetMusicItem(int16_t itemSelected);
    virtual void playMusic();
    virtual void btnNextClicked();
	virtual void btnPrevClicked();

	virtual void handleTickEvent();

protected:
	bool increase = false;
};

#endif // MUSICVIEW_HPP
