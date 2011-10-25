#include "UserInterface.h"
#include <stdio.h>
#include "../Render/GRender.h"
#include <Core/GEngine.h>

ViewerUserInterface::ViewerUserInterface() : IElement(10, 100, 250, 100) {

}

void ViewerUserInterface::draw(InterfaceLayer* IL) {

        GRenderer::getRenderer()->getGraphicsAPI()->enableBlending();

        std::string FPS;

        FPS = "FPS: ";

        std::stringstream Number;
        Number <<  GEngine::getInstance()->getFpsCalculator()->getFPS();

        FPS += Number.str();
        IL->getDefaultTextRenderer()->Render(256, x, y, width, height, FPS);

        GRenderer::getRenderer()->getGraphicsAPI()->disableBlending();

}
