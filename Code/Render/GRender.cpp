#include "GRender.h"
#include "Interface/InterfaceLayer.h"
#include "3D/3DRenderLayer.h"
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>

GRenderer* renderInstance = 0;

GRenderer::GRenderer()
{
    m_gameRenderLog = 0;
    m_renderAPI = 0;
    m_textureMonitor = 0;

    for (int i = 0; i < Layers_Max; i++)
        m_renderLayers[i] = 0;

}

GRenderer::~GRenderer()
{
    printf("GRender: Uninitializing\n");
    uninitialize();
}

GRenderer* GRenderer::getRenderer()
{
    if (renderInstance == 0)
    {
        renderInstance = new GRenderer();
    }

    return renderInstance;
}

bool GRenderer::initialize(unsigned int w, unsigned int h, const char* renderAPI)
{
    uninitialize();


    //Create a new render log
    m_gameRenderLog = new RenderLog("./render.log");


    printf("GRender: Getting API instance\n");

    //Get a API instance
    m_renderAPI = GraphicsAPI::getAPIInstance(renderAPI);

    printf("GRender: Attempted to get API instance\n");

    //Test the render API was got succesfully
    if (m_renderAPI == 0)
    {

        //If there was no gainable API instance bottle out

        char Buffer[512];
        sprintf(Buffer, "GRender: Unable to get a render context for graphics API %s API invalid or not supported by the system\n", renderAPI);

        m_gameRenderLog->writeToLogFile(Buffer);

        uninitialize();
        return false;
    }
    else
    {

        char Buffer[512];
        sprintf(Buffer, "GRender: Render API summary: %s\n", m_renderAPI->getAPISummary());

        //If you got a API instance write a summary to the log
        m_gameRenderLog->writeToLogFile(Buffer);

    }

    RenderWindow* gameWindow = m_renderAPI->getRenderWindow();

    if (gameWindow->create("Engine", w, h, this) == false)
    {
        m_gameRenderLog->writeToLogFile("GRender: Unable to create window");
        uninitialize();
        return false;
    }

    if (m_renderAPI->initialize() == false)
    {
        m_gameRenderLog->writeToLogFile("GRender: Render API failed to initialize");
        uninitialize();

        return false;
    }

    //Create a texture monitor
    m_textureMonitor = new TextureMonitor();

    //Set default wireframe values
    m_shouldRenderWireframe = false;


    //Set the layers
    setLayer(Layer_Interface, new InterfaceLayer());
    setLayer(Layer_3D, new ThreeDRenderLayer());

    printf("GRender: Done initializing the renderer\n");


    return true;
}

void GRenderer::uninitialize()
{

    //Free all the layers
    for (unsigned int i = 0; i < Layers_Max; ++i)
    {

        if (m_renderLayers[i] != 0)
            delete m_renderLayers[i];

        m_renderLayers[i] = 0;
    }

    //Free the texture monitor (And any textures it has loaded);
    if (m_textureMonitor != 0)
    {
        delete m_textureMonitor;
        m_textureMonitor = 0;
    }

    //Next delete the render API (And hopefully all the windows/GL context resources it was using, hehe)
    if (m_renderAPI != 0)
    {
        delete m_renderAPI;
        m_renderAPI = 0;
    }



    //Delete the log last just in case something wants to write data while it dies
    if (m_gameRenderLog != 0)
    {
        delete m_gameRenderLog;
        m_gameRenderLog = 0;
    }


    return;
}

void GRenderer::begin(int wx, int wy)
{
    getWindow()->update();

    getGraphicsAPI()->setupViewport(0, 0, m_renderAPI->getRenderWindow()->getWidth(), m_renderAPI->getRenderWindow()->getHeight());
    getGraphicsAPI()->clearScreen();
   // getGraphicsAPI()->setupViewport(0, 0, 512, 512);

}

bool GRenderer::getRenderWireframeValue()
{

    return m_shouldRenderWireframe;
}

void GRenderer::setRenderWireframeValue(bool val)
{
    m_shouldRenderWireframe = val;
}

unsigned int numFramesAccumed = 0;

void GRenderer::renderScene(int wx, int wy)
{

    begin(wx, wy);

    getLayer(Layer_3D)->draw(this, wx, wy);


    m_renderAPI->setupFor2D(wx, wy);

    m_renderAPI->enableBlending();

    m_renderAPI->disableBlending();
    m_renderAPI->clearActiveTexture();

    getLayer(Layer_Interface)->draw(this, wx, wy);






    end(wx, wy);
}

void GRenderer::end(int wx, int wy)
{

    if (m_renderAPI != 0)
    {
        m_renderAPI->clearErrors();
    }

    if (getWindow() != 0)
    {
        getWindow()->finalFrame();
    }


}

void GRenderer::update(float seconds)
{
    for (unsigned int i = 0; i < Layers_Max; i++)
    {

        if (GRenderer::m_renderLayers[i])
        {

            GRenderer::m_renderLayers[i]->update(seconds);

        }

    }
}
