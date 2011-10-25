#ifndef _3D_SCENE_NODE_DEF_H_
#define _3D_SCENE_NODE_DEF_H_
#include <Math/Vector.h>
#include "Camera.h"
#include <vector>

class SceneNode
{
protected:

    //The position of the scene node
    point3f_t m_position;

    //The rotation of the scene node
    point3f_t m_rotation;

    //The children of this scene node
    std::vector<SceneNode*> m_childNodes;

public:

    SceneNode()
    {
    }

    virtual ~SceneNode()
    {
        //Get the number of children
        unsigned int numberOfChildren = numChildren();

        //Delete each child
        for (unsigned int i = 0; i < numberOfChildren; ++i)
        {
            delete m_childNodes[i];
        }

        //Clear the children
        m_childNodes.clear();
    }

    void addChildNode(SceneNode* node)
    {
        m_childNodes.push_back(node);
    }

    void removeChildNode(SceneNode* node)
    {
        //Get the number of children
        unsigned int numberOfChildren = numChildren();

        for (unsigned int i = 0; i < numberOfChildren; ++i)
        {
            if (m_childNodes[i] == node)
            {
                m_childNodes.erase(m_childNodes.begin() + i);
                return;
            }

        }

        //If it gets here then the node wasn't found
    }

    virtual void update(float seconds)
    {

        unsigned int nChildren = numChildren();

        for (unsigned int i = 0; i < nChildren; ++i)
        {
            m_childNodes[i]->update(seconds);
        }
    }

    virtual void draw(Camera* cam)
    {
        unsigned int nChildren = numChildren();

        for (unsigned int i = 0; i < nChildren; ++i)
        {
            m_childNodes[i]->draw(cam);
        }
    }

    unsigned int numChildren()
    {
        return m_childNodes.size();
    }

    SceneNode* getChild(unsigned int i)
    {
        return m_childNodes[i];
    }

    point3f_t* getPosition()
    {
        return &m_position;
    }

    point3f_t* getRotation()
    {
        return &m_rotation;
    }
};

#endif //_3D_SCENE_NODE_DEF_H_
