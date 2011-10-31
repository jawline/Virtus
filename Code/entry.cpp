#include <Core/GEngine.h>
#include <3DViewer/ViewerApplication.h>


/**
 * @brief Entry point of the program, loads the base application and starts the engine loop
 */

int main()
{
    srand( time(0) );

    //Get a instance of the engine
    GEngine* Engine = GEngine::getInstance();

    //Check if a valid instance was created
    if (Engine == 0) {
    	printf("ERROR: Engine failed to be created");
    	return -1;
    }

    //Check if it's in a error
    if (Engine->isInErrorState() == true)
    {
        printf("ERROR: Engine failed to initialize with details %s\n", Engine->errorDetails());
        return -1;
    }


    GApplication* App = new ModelViewer();

    if (Engine->loadApplication(App) == false)
    {

        printf("Unable to load application, reason %s\n", Engine->errorDetails());
        return -1;

    }

    int Result = Engine->gameLoop();

    if (Engine->isInErrorState() == true)
    {
        printf("ERROR: Engine failed to run with details %s\n", Engine->errorDetails());
        return 0;
    }

    Engine->unloadApplication();

    delete GRenderer::getRenderer();
    delete Engine;
    delete App;

    return Result;
}
