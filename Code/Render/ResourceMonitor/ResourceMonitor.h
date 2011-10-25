#ifndef _RESOURCE_MONITOR_DEF_H_
#define _RESOURCE_MONITOR_DEF_H_

#include <vector>
#include <iostream>

/*
    This class contains a abstract resource monitor used to keep track of resources loaded into the system
    Used by classes such as the texture monitor
    Used as a template for most other resource monitors in the engine
*/

struct resource_monitor_entry
{
    /* The abstract class stores all entities as void* the resource monitor uses the template to typecast them back */
    void* m_Entity;

    /* Simple reference count system for now */
    unsigned int m_boundCount;

    /* Name used to identify the resource */
    std::string m_ResourceName;
};

class ResourceMonitor
{
protected:

    std::vector<resource_monitor_entry> m_resourceList;

    /* Free the entity specified. Overload to perform specific free calls (Like typecast or calling specific destroy function on it)*/

    virtual void f_freeEntity(void* m_Entity);

    /* Virtual function, loads specified resource, overload with specific load routine */
    virtual void* f_loadEntity(std::string Name);

    /* Free's the entity at the index specified (Removes it from the list and calls f_freeEntity on its data if there are no other binds) */

    void f_freeIndex(unsigned int i);

    /* Adds a entity to the resource list */

    void f_addEntity(std::string Filename, void* Entity);

    /* Functions to get and free entitys, protected because they should always be called from other functions within the child class */

    void* rmGetResource(std::string Resource);
    void rmReleaseResource(void* Resource);

public:
    ResourceMonitor();
    virtual ~ResourceMonitor();

};

#endif //_RESOURCE_MONITOR_DEF_H_
