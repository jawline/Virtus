#include <Core/GEngine.h>
#include <3DViewer/ViewerApplication.h>


/**
 * @brief Entry point of the program, loads the base application and starts the engine loop
 */

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
