#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

Model::Model() : modelListener(0), tickCounter(0), hour(0), minute(0), second(0)
{

}

void Model::tick()
{

}

void Model::SetmusicSelected(int itemSelected)
{
	MusicTitle = itemSelected;
}

int Model::GetMusic(void)
{
	return MusicTitle;
}
