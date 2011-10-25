#include "ResourceMonitor.h"
#include <stdio.h>

void ResourceMonitor::f_freeEntity(void* m_Entity)
{

}

void* ResourceMonitor::f_loadEntity(std::string Name)
{
    return 0;
}

void ResourceMonitor::f_freeIndex(unsigned int i)
{
    resource_monitor_entry Entry = m_resourceList[i];

    //Store a pointer to the entity
    void* Entity = Entry.m_Entity;

    //Remove it from the list
    m_resourceList.erase(m_resourceList.begin() + i);

    //Free the entity
    f_freeEntity(Entity);

    return;
}

void ResourceMonitor::f_addEntity(std::string Filename, void* Entity)
{
    //Create a new resource entry
    resource_monitor_entry Entry;
    Entry.m_ResourceName = Filename;
    Entry.m_boundCount = 1;

    //Create the memory for the actual resource unit
    Entry.m_Entity = Entity;

    //Add it to the resources list
    m_resourceList.push_back(Entry);

    return;
}

void* ResourceMonitor::rmGetResource(std::string Resource)
{
    for (unsigned int i = 0; i < m_resourceList.size(); i++)
    {
        if (m_resourceList[i].m_ResourceName.compare(Resource) == 0)
        {
            //Its the right resource
            m_resourceList[i].m_boundCount++;
            return m_resourceList[i].m_Entity;
        }
    }

    return f_loadEntity(Resource); //Not loaded, load it
}

void ResourceMonitor::rmReleaseResource(void* Resource)
{
    for (unsigned int i = 0; i < m_resourceList.size(); i++)
    {
        if (m_resourceList[i].m_Entity == Resource)
        {
            m_resourceList[i].m_boundCount--;

            if (m_resourceList[i].m_boundCount == 0)
            {
                printf("[RESOURCE_DEBUG] Entity 0x%lx removed from memory[/RESOURCE_DEBUG]\n", (unsigned long) Resource);
                f_freeIndex(i);
            }

            return;
        }
    }
}

ResourceMonitor::ResourceMonitor()
{

}

ResourceMonitor::~ResourceMonitor()
{
}
