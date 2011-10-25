#ifndef _APPLICATION_DEF_H_
#define _APPLICATION_DEF_H_

class GEngine;

/**
 * @brief Abstraction of a application that runs on the engine
 *
 */
class GApplication
{
private:

public:

    virtual ~GApplication() {}

    /**
     * @brief Called the first time the app is loaded, initialisation should be done here
     *
     */
    virtual bool onLoad(GEngine* Engine) = 0;

    /**
     * @brief Called when the app is unloaded, deinitilization should be done here
     */
    virtual bool onUnload() = 0;

    /**
     * @brief Called once per frame with the time that has elapsed since the last frame in seconds
     */
    virtual void updateApplication(float seconds) = 0;
};

#endif //_APPLICATION_DEF_H_
