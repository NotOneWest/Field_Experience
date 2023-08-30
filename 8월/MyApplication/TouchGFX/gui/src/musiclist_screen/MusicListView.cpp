#include <gui/musiclist_screen/MusicListView.hpp>
#include <touchgfx/Color.hpp>
#include "fatfs.h"
#include <string.h>

extern char files[30][256];
extern int file_num;

MusicListView::MusicListView() :
		musicListItemSelectedCallback(this, &MusicListView::musicListItemSelectedHandler)
{
}

void MusicListView::setupScreen()
{
	musicList.setNumberOfItems(file_num);
	musicList.setItemSelectedCallback(musicListItemSelectedCallback);

    MusicListViewBase::setupScreen();
}

void MusicListView::tearDownScreen()
{
    MusicListViewBase::tearDownScreen();
}

void MusicListView::musicListUpdateItem(CustomListElement& item, int16_t itemIndex)
{
    item.setupListElement(itemIndex);
}

void MusicListView::musicListItemSelectedHandler(int16_t itemSelected)
{

    selectedVal.invalidate();

    if(itemSelected >=0 && itemSelected < file_num){
    	Unicode::UnicodeChar buffer[SELECTEDVAL_SIZE];
		Unicode::strncpy(buffer, files[itemSelected], SELECTEDVAL_SIZE);
		Unicode::snprintf(selectedValBuffer, SELECTEDVAL_SIZE, "%s", buffer);
    }

	selectedVal.resizeToCurrentText(); selectedVal.invalidate();

	presenter->musicSelected(itemSelected);
}
























