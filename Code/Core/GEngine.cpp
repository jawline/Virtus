#include "GEngine.h"
#include <Input/Postbox.h>
#include <stdio.h>
#include <Input/Keys.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "./Render/Interface/InterfaceLayer.h"
#include "Configure/ConfigurationNumber.h"
#include "Configure/ConfigurationString.h"

const char* defaultCfgFile = "./engine.cfg";
const unsigned int defaultWindowWidth = 800;
const unsigned int defaultWindowHeight = 600;
const char* defaultWindowTitle = "Virtus Base Window";

const char* GEngine::TAG = "Engine Core";

GEngine::GEngine() :
	m_fpsCalculator() {
	m_engineCfg = 0;
	m_eventManager = 0;

	m_errorState = 0;
	m_errorDetails = 0;

	m_canvasWidth = 0;
	m_canvasHeight = 0;

	m_threadHive = 0;
	m_engineLog = 0;

	m_inputBindings = 0;
}

GEngine::~GEngine() {

	uninitialize();

}

void GEngine::initializeInputBindings() {

	m_inputBindings = new InputBinding[input_event_end];

	for (unsigned int i = 0; i < input_event_end; ++i) {

		InputBinding * l_CurInput = &m_inputBindings[i];
		l_CurInput->Keys = new EventBinding[KEYS_END];

	}

}

void GEngine::freeInputBindings() {

	if (m_inputBindings == 0)
		return;
	for (unsigned int i = 0; i < input_event_end; ++i) {

		InputBinding* l_curInput = &m_inputBindings[i];

		if (l_curInput != 0 && l_curInput->Keys != 0)
			delete[] l_curInput->Keys;

	}

	delete[] m_inputBindings;
}

bool GEngine::initialize() //Not much initialization to do yet =)
{
	//A buffer for the log messages
	char logBuffer[512];

	//Create the engine configuration
	//TODO: Check the log exists if not create it etc
	m_engineCfg = new ConfigurationInstance(
			static_cast<File*> (Filesystem::evaluateChild(
					Filesystem::getRoot(), "Saved/engine.cfg")));

	//Create a log for the engines core
	m_engineLog = new LogWriter(LogWriter::getPathToLogFile("Core"));

	//Get the graphicsAPI
	const char* graphicsAPIString = "OpenGL2";

	//Get a node to the configuration graphics API string
	ConfigurationNode* apiNode = m_engineCfg->getNode("Graphics/GraphicsAPI");

	//If the graphics API is specified as a string in the Cfg use that instead
	if (apiNode != 0 && apiNode->getType() == configurationString) {

		ConfigurationString* apiString =
				static_cast<ConfigurationString*> (apiNode);
		graphicsAPIString = apiString->getStringData();

	}

	//Initialize the games renderer. this function must be called before doing anything with the renderer
	if (GRenderer::getRenderer()->initialize(defaultWindowWidth,
			defaultWindowHeight, graphicsAPIString) == false) {

		//Unable to init the renderer so set the error state
		setErrorDetails(
				"Unable to get a instance for the specified graphics API");

		return false;
	}

	//Bind input events from the renderers window to our input postbox
	GRenderer::getRenderer()->getWindow()->setInputPostbox(&m_inputPostbox);

	//Create variables for the window height and width and set them to the defaults
	unsigned int renderWindowWidth = defaultWindowWidth;
	unsigned int renderWindowHeight = defaultWindowHeight;

	//Get a load of nodes for the engine settings
	ConfigurationNode* widthNode =
			m_engineCfg->getNode("Graphics/Window/Width");
	ConfigurationNode* heightNode = m_engineCfg->getNode(
			"Graphics/Window/Height");
	ConfigurationNode* titleNode =
			m_engineCfg->getNode("Graphics/Window/Title");
	ConfigurationNode* fullscreenNode = m_engineCfg->getNode(
			"Graphics/Window/Fullscreen");
	ConfigurationNode* defaultFontNode = m_engineCfg->getNode(
			"Graphics/Fonts/defaultFont");
	ConfigurationNode* numberOfThreads = m_engineCfg->getNode("Engine/Cores");

	//If the configuration node doesn't exist or isn't a number then create a job engine
	if (numberOfThreads == 0 || numberOfThreads->getType()
			!= configurationNumber) {
		m_threadHive = new BeeHive(1);
	} else {

		//Otherwise use the number of threads configuration node value to create the BeeHive
		m_threadHive
				= new BeeHive(
						(static_cast<ConfigurationNumber*> (numberOfThreads))->getValue());

	}

	//Send a message that the HIVE has been created to the log
	m_engineLog->writeLine(TAG, "Initialized job HIVE");

	//If the width node exists and is a number
	if (widthNode != 0 && widthNode->getType() == configurationNumber) {

		//Set the window width to the width nodes value
		ConfigurationNumber* Number =
				static_cast<ConfigurationNumber*> (widthNode);
		renderWindowWidth = Number->getValue();
	} /* NOTE: if it doesn't exist it will be set to the default value above */

	//If the height node exists and is a number
	if (heightNode != 0 && heightNode->getType() == configurationNumber) {

		//Set the window height to the height nodes value
		ConfigurationNumber* Number =
				static_cast<ConfigurationNumber*> (heightNode);
		renderWindowHeight = Number->getValue();

	} /* NOTE: As above if  it doesn't exist it will be set to default */

	//Print this info to the log
	stringstream widthStrm;
	widthStrm << renderWindowWidth;

	m_engineLog->writeLine(TAG, string("Set width to ") + widthStrm.str());

	stringstream heightStrm;
	heightStrm << renderWindowHeight;

	m_engineLog->writeLine(TAG, string("Set height to ") + heightStrm.str());

	//Set the window dimensions to the variables
	GRenderer::getRenderer()->getWindow()->setDimensions(renderWindowWidth,
			renderWindowHeight);

	string title = defaultWindowTitle;

	//If the window has a title node and it is a configuration string
	if (titleNode != 0 && titleNode->getType() == configurationString) {

		ConfigurationString* string =
				static_cast<ConfigurationString*> (titleNode);

		if (string->getStringData() != 0) {

			//Set the window title to the configuration node value
			title = string->getStringData();
		}

	} /* NOTE: Otherwise set to default */

	//Print this info to the log
	m_engineLog->writeLine(TAG, string("Setting window title to ") + title);

	GRenderer::getRenderer()->getWindow()->setTitle(title.c_str());

	//If the window is supposed to be fullscreen hit that
	if (fullscreenNode != 0 && fullscreenNode->getType() == configurationNumber) {

		ConfigurationNumber* fullscreenValue =
				static_cast<ConfigurationNumber*> (fullscreenNode);

		//If the value is set to 1 (True)
		if (fullscreenValue->getValue() == 1) {

			//Fullscreen the window
			GRenderer::getRenderer()->getWindow()->setFullscreen(true);

			//State this in the log
			m_engineLog->writeLine(TAG, "Using fullscreen if supported");

		} else {

			//Unfullscreenit
			GRenderer::getRenderer()->getWindow()->setFullscreen(false);

		}

	}

	//If the default font node is set in the config and is a file path
	if (defaultFontNode != 0 && defaultFontNode->getType()
			== configurationString) {

		//Set the defalt font renderer to load the specified font
		ConfigurationString* string =
				static_cast<ConfigurationString*> (defaultFontNode);
		GRenderer::getRenderer()->getInterfaceManager()->getDefaultTextRenderer()->Load(
				(File*) Filesystem::evaluateChild(Filesystem::getRoot(),
						string->getStringData()));

	}

	m_inputBindings = 0;
	initializeInputBindings();

	m_GameActive = false;

	m_eventManager = new EventManager();

	if (!m_eventManager) {
		setErrorDetails("Couldn't create a new event manager");

		return false;
	}

	m_engineLog->writeLine(TAG, "Initialized event manager");

	m_GamePaused = false;

	m_currentApplication = 0;

	m_canvasWidth = renderWindowWidth;
	m_canvasHeight = renderWindowHeight;

	m_engineLog->writeLine(TAG, "Intiailized engine");

	return true;
}

void GEngine::uninitialize() {

	freeInputBindings();

	if (m_eventManager != 0) {
		m_engineLog->writeLine(TAG, "Freeing event manager");

		delete m_eventManager;
		m_eventManager = 0;
	}

	if (m_engineCfg != 0) {
		m_engineLog->writeLine(TAG, "Freeing the configuration instance");

		delete m_engineCfg;
		m_engineCfg = 0;
	}

	//Kill the thread hive
	if (m_threadHive != 0) {
		m_engineLog->writeLine(TAG, "Killing the HIVE");

		delete m_threadHive;
		m_threadHive = 0;
	}

	//Write that the engine has been uninitialized to the log
	m_engineLog->writeLine(TAG, "Engine uninitialized");

	//Then destroy the log
	if (m_engineLog != 0) {
		delete m_engineLog;
		m_engineLog = 0;
	}

}

void GEngine::bindEvent(unsigned int Eve, input_event_ids id, key_ids key) {
	if (id > input_event_end)
		return;
	if (key > KEYS_END)
		return;

	m_inputBindings[id].Keys[key].Events.push_back(Eve);
}

void GEngine::f_sleep() {

	if (m_fpsCalculator.getFPS() > 1000) {

		//usleep(m_fpsCalculator.getFPS() * 2);

	}

}

void GEngine::sendEventManagerMessage(Input_Event iEvent) {

	//Get the binding related to that event ID
	InputBinding* inputBinding = &m_inputBindings[iEvent.Event_ID];

	//Trigger the event bound to that key
	if (iEvent.Event_Data.size() != 0) {

		//Find the event binding for that key
		EventBinding* eventBinding = &inputBinding->Keys[iEvent.Event_Data[0]];

		//Find the list of events to fire from the event binding
		std::vector<unsigned int>* eventsToFire = &eventBinding->Events;

		//For each of those events
		for (unsigned int i = 0; i < eventsToFire->size(); i++) {

			//Trigger the event within the event manager
			m_eventManager->triggerEvent(eventsToFire->at(i),
					iEvent.Event_Data, iEvent.Event_Flags);

		}

	}
}

void GEngine::processEvent(Input_Event event) {

	//If the Event ID is a mousepress scale the inputs to the right canvas width (As the canvas width may differ from the renderer width
	switch (event.Event_ID) {

	case input_event_button_one:
	case input_event_button_two: {
		float eventX = event.Event_Data[1];
		float eventY = event.Event_Data[2];

		eventX = eventX / GRenderer::getRenderer()->getWindow()->getWidth();
		eventX = eventX * m_canvasWidth;

		eventY = eventY / GRenderer::getRenderer()->getWindow()->getHeight();
		eventY = eventY * m_canvasHeight;

		event.Event_Data[1] = eventX;
		event.Event_Data[2] = eventY;

		break;
	}
		;
	}

	//The next few lines send it through the interface manager. if the interface manager uses it then don't send it to the registered listener, otherwise send it to the registered listener


	//If the interface manager returns true on ProcessEvent
	if (GRenderer::getRenderer()->getInterfaceManager()->processEvent(event)
			== true) {

		//This event has been used by the interface manager, don't send it to the event manager


	} else {

		//Send it to the event manager because the interfacemanager
		sendEventManagerMessage(event);
	}
}

void GEngine::doFrameUpdate(float secondsPassed) {

	if (!m_GamePaused) {

		if (m_physicsActive) {

			//Process all physics
			//TODO: REVISE PHYSICS - m_gamePhysics->Update(secondsPassed);

		}

		//Update all the rendering bits
		GRenderer::getRenderer()->update(secondsPassed);

		//Update the application
		if (m_currentApplication != 0) {

			m_currentApplication->updateApplication(secondsPassed);

		}

	}

}

int GEngine::gameLoop() {

	if (m_errorState == true) {
		setErrorDetails(
				"Cannot enter game loop when the engine is in a error state");
		return -1;
	}

	m_GameActive = true;
	m_physicsActive = true;

	m_clock.initialize();
	float secondsPassed;

	while (m_GameActive) {

		secondsPassed = m_clock.calculateElapsedTime();

		//Alert to the FPS calculator of a new frame
		m_fpsCalculator.frame(secondsPassed);

		//Run through the postbox and trigger events
		while (m_inputPostbox.HasNext()) {

			processEvent(m_inputPostbox.ReadNext());

		}

		doFrameUpdate(secondsPassed);

		//Rendering Step

		GRenderer::getRenderer()->renderScene(m_canvasWidth, m_canvasHeight);

		//End of rendering step

		//Sleep
		f_sleep();
	}

	return 0;
}

bool GEngine::loadApplication(GApplication* App) {

	//If the engine is in a error state then can't load a application
	if (m_errorState == true) {
		setErrorDetails(
				"Cannot load application if a error occured prior to load call");
		return false;
	}

	//If there is currently a application loaded return false
	if (m_currentApplication != 0) {
		return false;
	}

	if (App->onLoad(this) == false) {
		return false;
	}

	m_currentApplication = App;

	return true;
}

bool GEngine::unloadApplication() {

	if (m_currentApplication == 0) {
		return true;
	}

	m_currentApplication->onUnload();

	m_currentApplication = 0;

	return true;
}

void GEngine::setErrorDetails(const char* str) {
	if (m_errorDetails != 0)
		delete[] m_errorDetails;

	char* newErrorDetails = new char[strlen(str) + 1];
	m_errorDetails = newErrorDetails;

	strncpy(newErrorDetails, str, strlen(str) + 1);

	m_engineLog->writeLine( TAG, string("Error details set as ") + m_errorDetails);

	m_errorState = true;
}

static GEngine* m_gameEngineInstance = 0;

GEngine* GEngine::getInstance() {

	if (m_gameEngineInstance == 0) {

		m_gameEngineInstance = new GEngine();
		m_gameEngineInstance->initialize();

	}

	return m_gameEngineInstance;
}
