#include <gui/containers/CustomListElement.hpp>

extern char files[30][256];

CustomListElement::CustomListElement()
{

}

void CustomListElement::initialize()
{
    CustomListElementBase::initialize();
}

void CustomListElement::setupListElement(int16_t index)
{
	Unicode::UnicodeChar buffer[TITLE_SIZE];
	Unicode::strncpy(buffer, files[index], TITLE_SIZE);
	Unicode::snprintf(titleBuffer, TITLE_SIZE, "%s", buffer);
	title.resizeToCurrentTextWithAlignment(); title.invalidate();
}
