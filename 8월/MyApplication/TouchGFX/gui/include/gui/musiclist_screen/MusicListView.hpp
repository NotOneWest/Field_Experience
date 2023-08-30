#ifndef MUSICLISTVIEW_HPP
#define MUSICLISTVIEW_HPP

#include <gui_generated/musiclist_screen/MusicListViewBase.hpp>
#include <gui/musiclist_screen/MusicListPresenter.hpp>
#include <gui/containers/CustomListElement.hpp>

class MusicListView : public MusicListViewBase
{
public:
    MusicListView();
    virtual ~MusicListView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void musicListUpdateItem(CustomListElement& item, int16_t itemIndex);


protected:

	Callback<MusicListView, int16_t> musicListItemSelectedCallback;
	void musicListItemSelectedHandler(int16_t itemSelected);
};

#endif // MUSICLISTVIEW_HPP
