#include "FontMap.h"
#include "../GRender.h"

FontMap::FontMap() {
	PointSize = 0;

	//Optimize this to use a single VertexSet for the whole engine? Or will one per map be efficient enough..
	drawBuffer = GRenderer::getRenderer()->getGraphicsAPI()->newVertexSet();

	//Setup the initial vertex set state so taht the render function can just manipulate the existing vertices and texture coordinates,
	float texCoords[] = { 0, 1,
                          0, 0,
                          1, 0,
                          1, 1
                        };

    float vertices[] = { 0, 0, 0,
                         0, 0, 0,
                         0, 0, 0,
                         0, 0, 0
                       };

    drawBuffer->setVertices(vertices, 4);
    drawBuffer->setTextureCoordinates(texCoords, 4);
}

void FontMap::Render(int Char, float X, float Y)
{

    int OffsetX = CharList[Char].OffsetX;
    int OffsetY = -CharList[Char].OffsetY;

    FontMap::Render(Char, X, Y, OffsetX, OffsetY, CharList[Char].Width, CharList[Char].Height);

}

void FontMap::Render(int Char, float X, float Y, float offsetX, float offsetY, float Width, float Height)
{

    if(Char <= 0 || Char >= 255) return;

    if (CharList[Char].charTexture != 0)
    {
        CharList[Char].charTexture->makeActive();
    }

    //Get a pointer to the drawBuffers vertices to manipulate
    float* vertices = drawBuffer->getVertices();

    //Set the drawBuffers vertices
    vertices[0] = X + offsetX;
    vertices[1] = Y + Height + offsetY;
    vertices[2] = 0;

    vertices[3] = X + offsetX;
    vertices[4] = Y + offsetY;
    vertices[5] = 0;

    vertices[6] = X + Width + offsetX;
    vertices[7] = Y + offsetY;
    vertices[8] = 0;

    vertices[9] = X + Width + offsetX;
    vertices[10] = Y + Height + offsetY;
    vertices[11] = 0;

    //Send it to be drawn
    drawBuffer->Draw(0, 4, QUADS);

}

void FontMap::Cleanup() {

    GRenderer::getRenderer()->getRenderLog()->writeToLogFile("Cleaning up FontMap\n");

    for (int i = 0; i < 255; i++)
    {

        if (CharList[i].charTexture != 0)
        {
            delete CharList[i].charTexture;
        }

        CharList[i].charTexture = 0;

    }

    if (drawBuffer != 0)
        delete drawBuffer;

    drawBuffer = 0;
}

FontChar* FontMap::GetChar(int Char) {
	if(Char <= 0 || Char >= 255) return NULL;

    return &CharList[Char];
}
