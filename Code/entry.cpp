#include <Core/GEngine.h>
#include "3DViewer/ViewerApplication.h"
#include "Render/GRender.h"

#include <Filesystem/Filesystem.h>
#include <Filesystem/LocalFolder.h>
#include <Filesystem/LocalFile.h>
#include <LogWriter/LogWriter.h>


int main()
{
    srand( time(0) );

    GEngine* Engine = GEngine::getInstance();

    if (Engine->isInErrorState() == true)
    {
        printf("ERROR: Engine failed to initialize with details %s\n", Engine->errorDetails());
    }

    GApplication* App = new ModelViewer();

    if (Engine->loadApplication(App) == false)
    {
        printf("Unable to load application, reason %s\n", Engine->errorDetails());
    }

    int Result = Engine->gameLoop();

    if (Engine->isInErrorState() == true)
    {
        printf("ERROR: Engine failed to run with details %s\n", Engine->errorDetails());
    }

    delete GRenderer::getRenderer();
    delete Engine;
    delete App;

    return Result;
}
