#ifndef _IMAGE_ELEMENT_DEF_H_
#define _IMAGE_ELEMENT_DEF_H_
#include "IElement.h"
#include "../TextureMonitor/TextureMonitor.h"

/* Simple UI element that displays a image */

class ImageElement : public IElement
{
private:
    TextureMonitor* m_tMonitor;
    Texture* m_dispTexture;

public:
    ImageElement(float x, float y, float width, float height, const char* Filename, TextureMonitor* EMon);
    ~ImageElement();

};

#endif //_IMAGE_ELEMENT_DEF_H_
