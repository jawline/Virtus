#ifndef _APPLICATION_DEF_H_
#define _APPLICATION_DEF_H_

class GEngine;

class GApplication
{
private:

public:

    virtual ~GApplication() {}

    virtual bool onLoad(GEngine* Engine)
    {

        return false;
    }

    virtual bool onUnload()
    {

        return false;
    }

    virtual void updateApplication(float seconds)
    {

        return;
    }
};

#endif //_APPLICATION_DEF_H_
