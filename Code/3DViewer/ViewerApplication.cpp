#include "ViewerApplication.h"
#include <Core/GEngine.h>
#include "EventsList.h"
#include "../Render/3D/3DRenderLayer.h"
#include "../Render/Interface/Button.h"
#include "../Render/Interface/Checkbox.h"
#include "UserInterface.h"
#include "../Render/3D/Plane.h"
#include "../Render/3D/WorldNode.h"
#include <Render/3D/VoxJect.h>
#include <math.h>

const float viewerRotateSpeed = 50.0f;
const float viewerMoveSpeed = 1000.0f;

void toggleWireframeFn(Button* Btn)
{

    GRenderer::getRenderer()->setRenderWireframeValue(!GRenderer::getRenderer()->getRenderWireframeValue());

}

bool ModelViewer::onLoad(GEngine* Engine) {

    printf("Modelviewer: Loaded into engine\n");

    m_Forward = m_Backward = m_Left = m_Right = m_Up = m_Down = false;

    m_worldNode = new WorldNode("Data/skys/dome_3.tga", 9000);

    TestNode* Test = new TestNode();
    Test->load("Data/Models/test.3ds");
    SceneNode* TestTerrain = new TerrainNode();

    Test->getPosition()->setX(12);
    Test->getPosition()->setZ(0);
    Test->getPosition()->setY(50);

    m_worldNode->addChildNode(Test);

    Test = new TestNode();
    Test->load("Data/Models/test.3ds");
    Test->getPosition()->setX(256);
    Test->getPosition()->setZ(256);
    Test->getPosition()->setY(50);
    m_worldNode->addChildNode(Test);

    Test = new TestNode();
    Test->load("Data/Models/test.3ds");
    Test->getPosition()->setX(512);
    Test->getPosition()->setY(50);
    Test->getPosition()->setZ(512);
    m_worldNode->addChildNode(Test);


    m_worldNode->addChildNode(TestTerrain);

    ThreeDRenderLayer* Layer = (ThreeDRenderLayer*) GRenderer::getRenderer()->getLayer(Layer_3D);
    Layer->setWorldNode(m_worldNode);

    GRenderer::getRenderer()->get3DLayer()->getActiveCamera()->move(-256, -20, -256);

    Engine->getEventManager()->setNumberOfEvents(EVENTS_END);

    ForwardHandle = new PressedEventHandler(&m_Forward);
    ForwardReleaseHandle = new ReleasedEventHandler(&m_Forward);

    Engine->bindEvent(EVENT_MOVE_FORWARD, input_event_keypressed, KEY_W);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_FORWARD, ForwardHandle);

    Engine->bindEvent(EVENT_MOVE_FORWARD_STOP, input_event_keyreleased, KEY_W);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_FORWARD_STOP, ForwardReleaseHandle);

    LeftHandle = new PressedEventHandler(&m_Left);
    LeftReleaseHandle = new ReleasedEventHandler(&m_Left);

    Engine->bindEvent(EVENT_ROTATE_LEFT, input_event_keypressed, KEY_A);
    Engine->getEventManager()->registerEventCallback(EVENT_ROTATE_LEFT, LeftHandle);

    Engine->bindEvent(EVENT_ROTATE_LEFT_STOP, input_event_keyreleased, KEY_A);
    Engine->getEventManager()->registerEventCallback(EVENT_ROTATE_LEFT_STOP, LeftReleaseHandle);

    RightHandle = new PressedEventHandler(&m_Right);
    RightReleaseHandle = new ReleasedEventHandler(&m_Right);

    Engine->bindEvent(EVENT_ROTATE_RIGHT, input_event_keypressed, KEY_D);
    Engine->getEventManager()->registerEventCallback(EVENT_ROTATE_RIGHT, RightHandle);

    Engine->bindEvent(EVENT_ROTATE_RIGHT_STOP, input_event_keyreleased, KEY_D);
    Engine->getEventManager()->registerEventCallback(EVENT_ROTATE_RIGHT_STOP, RightReleaseHandle);

    BackwardHandle = new PressedEventHandler(&m_Backward);
    BackwardReleaseHandle = new ReleasedEventHandler(&m_Backward);

    Engine->bindEvent(EVENT_MOVE_BACKWARD, input_event_keypressed, KEY_S);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_BACKWARD, BackwardHandle);

    Engine->bindEvent(EVENT_MOVE_BACKWARD_STOP, input_event_keyreleased, KEY_S);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_BACKWARD_STOP, BackwardReleaseHandle);

    UpHandle = new PressedEventHandler(&m_Up);
    UpReleaseHandle = new ReleasedEventHandler(&m_Up);

    Engine->bindEvent(EVENT_MOVE_UP, input_event_keypressed, KEY_E);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_UP, UpHandle);

    Engine->bindEvent(EVENT_MOVE_UP_STOP, input_event_keyreleased, KEY_E);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_UP_STOP, UpReleaseHandle);

    DownHandle = new PressedEventHandler(&m_Down);
    DownReleaseHandle = new ReleasedEventHandler(&m_Down);

    Engine->bindEvent(EVENT_MOVE_DOWN, input_event_keypressed, KEY_Q);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_DOWN, DownHandle);

    Engine->bindEvent(EVENT_MOVE_DOWN_STOP, input_event_keyreleased, KEY_Q);
    Engine->getEventManager()->registerEventCallback(EVENT_MOVE_DOWN_STOP, DownReleaseHandle);

    m_toggleWireframeHandler = new ToggleWireframeHandler();

    Engine->bindEvent(EVENT_WIREFRAME_TOGGLE, input_event_keyreleased, KEY_K);
    Engine->getEventManager()->registerEventCallback(EVENT_WIREFRAME_TOGGLE, m_toggleWireframeHandler);

    windowCloseHandler = new CloseHandler();
    Engine->bindEvent(EVENT_CLOSE, input_event_window, WINDOW_KILL_REQUEST);
    Engine->getEventManager()->registerEventCallback(EVENT_CLOSE, windowCloseHandler);

    m_viewerInterface = new ViewerUserInterface();
    ((InterfaceLayer*)GRenderer::getRenderer()->getLayer(Layer_Interface))->AddElement(m_viewerInterface);


    m_gameEngine = Engine;

    Color textClr = Color(1, 1, 1);
    Color bgNormal = Color(0, 0, 1);
    Color bgOver = Color(1, 0, 0);
    Color bgDown = Color(0, 1, 0);

    Button* Btn  = new Button(10, GEngine::getInstance()->getCanvasHeight() - 50, 300, 50, "Toggle Wireframe", "Really, its pretty cool", "Enjoy", textClr, bgNormal, bgOver, bgDown);
    Btn->setOnMouseUp(toggleWireframeFn);
    GRenderer::getRenderer()->getInterfaceManager()->AddElement(Btn);

    m_wireframeButton = Btn;

    return true;
}

bool ModelViewer::onUnload() {

    ThreeDRenderLayer* Layer = (ThreeDRenderLayer*) GRenderer::getRenderer()->getLayer(Layer_3D);

/*    Layer->RemoveNode(Test);
    Layer->RemoveNode(TestTerrain);
    */

    Layer->setWorldNode(0);
    delete m_worldNode;

    //UNBIND EVENTS
    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_FORWARD, ForwardHandle);
    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_FORWARD_STOP, ForwardReleaseHandle);

    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_BACKWARD, BackwardHandle);
    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_BACKWARD_STOP, BackwardReleaseHandle);

    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_ROTATE_LEFT, LeftHandle);
    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_ROTATE_LEFT_STOP, LeftReleaseHandle);

    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_ROTATE_RIGHT, RightHandle);
    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_ROTATE_RIGHT_STOP, RightReleaseHandle);

    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_UP, UpHandle);
    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_UP_STOP, UpReleaseHandle);

    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_DOWN, DownHandle);
    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_MOVE_DOWN_STOP, DownReleaseHandle);

    m_gameEngine->getEventManager()->unregisterEventCallback(EVENT_WIREFRAME_TOGGLE, m_toggleWireframeHandler);

    //Delete event hooks
    delete ForwardHandle;
    delete ForwardReleaseHandle;

    ForwardHandle = 0;
    ForwardReleaseHandle = 0;

    delete BackwardHandle;
    delete BackwardReleaseHandle;

    BackwardHandle = 0;
    BackwardReleaseHandle = 0;

    delete LeftHandle;
    delete LeftReleaseHandle;

    LeftHandle = 0;
    LeftReleaseHandle = 0;

    delete RightHandle;
    delete RightReleaseHandle;

    RightHandle = 0;
    RightReleaseHandle = 0;

    delete UpHandle;
    delete UpReleaseHandle;

    UpHandle = 0;
    UpReleaseHandle = 0;

    delete DownHandle;
    delete DownReleaseHandle;

    DownHandle = 0;
    DownReleaseHandle = 0;

    delete m_toggleWireframeHandler;

    m_toggleWireframeHandler = 0;

    GRenderer::getRenderer()->getInterfaceManager()->RemoveElement(m_viewerInterface);
    GRenderer::getRenderer()->getInterfaceManager()->RemoveElement(m_wireframeButton);

    delete m_viewerInterface;
    delete m_wireframeButton;

    printf("Modelviewer: unloaded from engine\n");

    return true;
}

void ModelViewer::updateApplication(float seconds) {

    //  TestTerrain->m_rotation.y += 60 * seconds;
    ThreeDRenderLayer* Layer = (ThreeDRenderLayer*) GRenderer::getRenderer()->getLayer(Layer_3D);
    //Layer->getActiveCamera()->rotate(0, 10*seconds, 0);

    if (m_Left == true) {
        Layer->getActiveCamera()->rotate(0, -viewerRotateSpeed * seconds, 0);
    }

    if (m_Right == true) {
        Layer->getActiveCamera()->rotate(0, viewerRotateSpeed * seconds, 0);
    }

    if (m_Up == true) {
        Layer->getActiveCamera()->rotate(viewerRotateSpeed * seconds, 0, 0);
    }

    if (m_Down == true) {
        Layer->getActiveCamera()->rotate(-viewerRotateSpeed * seconds, 0, 0);
    }

    if (m_Forward == true) {

        /*
        float Value = viewerMoveSpeed;
        Value = Value * cos(Layer->getActiveCamera()->getRotation().getY() * M_PI / 180);

        float Value2 = viewerMoveSpeed;
        Value2 = Value2 * sin(Layer->getActiveCamera()->getRotation().getY() * M_PI / 180);

        float Value3 = viewerMoveSpeed;
        Value3 = Value3 * sin(Layer->getActiveCamera()->getRotation().getY() * M_PI / 180);

        Layer->getActiveCamera()->move(-Value2 * seconds, Value3 * seconds, Value * seconds);
        */

        float xIncrement = viewerMoveSpeed * sin (Layer->getActiveCamera()->getRotation().getY() * M_PI / 180) * seconds;
        float yIncrement = viewerMoveSpeed * sin (Layer->getActiveCamera()->getRotation().getX() * M_PI / 180) * seconds;
        float zIncrement = viewerMoveSpeed * cos (Layer->getActiveCamera()->getRotation().getY() * M_PI / 180) * seconds;

        Layer->getActiveCamera()->move(-xIncrement, yIncrement, zIncrement);


    }

    if (m_Backward == true) {

        /*
        float Value = viewerMoveSpeed;
        Value = Value * cos(Layer->getActiveCamera()->getRotation().getY() * M_PI / 180);

        float Value2 = viewerMoveSpeed;
        Value2 = Value2 * sin(Layer->getActiveCamera()->getRotation().getY() * M_PI / 180);

        float Value3 = viewerMoveSpeed;
        Value3 = Value3 * sin(Layer->getActiveCamera()->getRotation().getY() * M_PI / 180);

        Layer->getActiveCamera()->move(Value2 * seconds, -Value3 * seconds, -Value * seconds);
        */


        float xIncrement = viewerMoveSpeed * sin (Layer->getActiveCamera()->getRotation().getY() * M_PI / 180) * seconds;
        float yIncrement = viewerMoveSpeed * sin (Layer->getActiveCamera()->getRotation().getX() * M_PI / 180) * seconds;
        float zIncrement = viewerMoveSpeed * cos (Layer->getActiveCamera()->getRotation().getY() * M_PI / 180) * seconds;

        Layer->getActiveCamera()->move(xIncrement, -yIncrement, -zIncrement);


    }
}
