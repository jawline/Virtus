#ifndef _GAME_RENDERING_LAYER_DEF_H_
#define _GAME_RENDERING_LAYER_DEF_H_

class GRenderer;

class GRenderLayer
{
public:

    virtual void draw(GRenderer* renderer, int width, int height) { }
    virtual void update(float seconds) { }

    virtual int getID()
    {
        return -1;
    }

    virtual ~GRenderLayer() { }

};

#endif //_GAME_RENDERING_LAYER_DEF_H_
