#ifndef _POSTBOX_DEF_H_
#define _POSTBOX_DEF_H_
#include <queue>

/* Warning - Correct usage is to check the HasNext bool before peaking or reading. */

using namespace std;

template <class T>
class Postbox
{
private:
    queue<T> m_PostboxQueue;

public:

    void AddQueue(T Event)
    {
        m_PostboxQueue.push(Event);
    }

    T ReadNext()
    {
        T Ret = m_PostboxQueue.front();
        m_PostboxQueue.pop();
        return Ret;
    }

    T PeakNext()
    {
        if (!m_PostboxQueue.empty())
        {
            return m_PostboxQueue.front();
        }
    }

    bool HasNext()
    {
        if (m_PostboxQueue.empty()) return false;
        return true;
    }

};

#endif //_POSTBOX_DEF_H_