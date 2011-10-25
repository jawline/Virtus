#include "GraphicsAPI.h"
#include <string.h>
#include <stdio.h>
#include "GL2/GLAPI.h"

GraphicsAPI* GraphicsAPI::getAPIInstance(const char* Name)
{

    if (strcmp(Name, "OpenGL2") == 0)
    {
        printf("Returning new OpenGL2 API instance\n");
        return new GL2GraphicsAPI();
    }

    printf("Unable to get specified graphics API\n");
    return 0;
}
