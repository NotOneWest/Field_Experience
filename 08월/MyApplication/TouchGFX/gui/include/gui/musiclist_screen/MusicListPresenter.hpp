#ifndef MUSICLISTPRESENTER_HPP
#define MUSICLISTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MusicListView;

class MusicListPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MusicListPresenter(MusicListView& v);

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

    virtual ~MusicListPresenter() {};

    virtual void musicSelected(int16_t itemSelected);


private:
    MusicListPresenter();

    MusicListView& view;
};

#endif // MUSICLISTPRESENTER_HPP
