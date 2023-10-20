#ifndef CUSTOMLISTELEMENT_HPP
#define CUSTOMLISTELEMENT_HPP

#include <gui_generated/containers/CustomListElementBase.hpp>
#include <touchgfx/widgets/Button.hpp>

class CustomListElement : public CustomListElementBase
{
public:
    CustomListElement();
    virtual ~CustomListElement() {}

    virtual void initialize();

    void setupListElement(int16_t index);

protected:

};

#endif // CUSTOMLISTELEMENT_HPP
