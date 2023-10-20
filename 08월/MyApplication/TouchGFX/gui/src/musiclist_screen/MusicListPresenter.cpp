#include <gui/musiclist_screen/MusicListView.hpp>
#include <gui/musiclist_screen/MusicListPresenter.hpp>

MusicListPresenter::MusicListPresenter(MusicListView& v)
    : view(v)
{

}

void MusicListPresenter::activate()
{

}

void MusicListPresenter::deactivate()
{

}

void MusicListPresenter::musicSelected(int16_t itemSelected)
{
	model->SetmusicSelected(itemSelected);
}
