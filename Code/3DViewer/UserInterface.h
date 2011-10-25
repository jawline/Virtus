#ifndef _VIEWER_USER_INTERFACE_DEF_H_
#define _VIEWER_USER_INTERFACE_DEF_H_
#include <Render/Interface/SText.h>
#include <Render/Interface/InterfaceLayer.h>
#include <Render/Color.h>
#include <Render/Interface/Button.h>
#include <sstream>

class ViewerUserInterface : public IElement
{

public:

    ViewerUserInterface();
    void draw(InterfaceLayer* IL);
    bool handleInput(Input_Event e) { return false; }

};

#endif
