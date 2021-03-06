//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2020 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#include "MultitouchInputDevice.h"
#include "CursorEvent.h"
#include "Player.h"
#include "AVGNode.h"
#include "TouchStatus.h"

#include "../base/Logger.h"
#include "../base/ObjectCounter.h"
#include "../base/Exception.h"
#include "../base/ConfigMgr.h"

using namespace std;

namespace avg {

MultitouchInputDevice::MultitouchInputDevice(const DivNodePtr& pEventReceiverNode)
    : InputDevice("MultitouchInputDevice", pEventReceiverNode)
{
    if (pEventReceiverNode) {
        m_TouchOffset = IntPoint(0,0);
        m_TouchArea = pEventReceiverNode->getSize();
    } else {
        m_TouchArea = ConfigMgr::get()->getSizeOption("touch", "area");
        if (m_TouchArea.x == 0) {
            m_TouchArea = Player::get()->getScreenResolution();
        }
        m_TouchOffset = ConfigMgr::get()->getSizeOption("touch", "offset");
    }
    m_pMutex = MutexPtr(new boost::mutex);
}

MultitouchInputDevice::~MultitouchInputDevice()
{
}

vector<EventPtr> MultitouchInputDevice::pollEvents()
{
    lock_guard lock(*m_pMutex);

    vector<EventPtr> events;
    vector<TouchStatusPtr>::iterator it;
//    cerr << "--------poll---------" << endl;
    for (it = m_Touches.begin(); it != m_Touches.end(); ) {
//        cerr << (*it)->getID() << " ";
        CursorEventPtr pEvent = (*it)->pollEvent();
        if (pEvent) {
            events.push_back(pEvent);
            if (pEvent->getType() == Event::CURSOR_UP) {
                it = m_Touches.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
//    cerr << endl;
    return events;
}

int MultitouchInputDevice::getNumTouches() const
{
    return m_TouchIDMap.size();
}

TouchStatusPtr MultitouchInputDevice::getTouchStatus(int id)
{
    map<int, TouchStatusPtr>::iterator it = m_TouchIDMap.find(id);
    if (it == m_TouchIDMap.end()) {
        return TouchStatusPtr();
    } else {
        return it->second;
    }
}

void MultitouchInputDevice::addTouchStatus(int id, CursorEventPtr pInitialEvent)
{
    TouchStatusPtr pTouchStatus(new TouchStatus(pInitialEvent));
    m_TouchIDMap[id] = pTouchStatus;
    m_Touches.push_back(pTouchStatus);
}

void MultitouchInputDevice::removeTouchStatus(int id)
{
    unsigned numRemoved = m_TouchIDMap.erase(id);
    AVG_ASSERT(numRemoved == 1);
}

const MultitouchInputDevice::TouchIDMap& MultitouchInputDevice::getTouchIDMap() const
{
    return m_TouchIDMap;
}

glm::vec2 MultitouchInputDevice::getTouchArea() const
{
    return m_TouchArea;
}

IntPoint MultitouchInputDevice::getScreenPos(const glm::vec2& pos) const
{
    return IntPoint(int(pos.x * m_TouchArea.x + m_TouchOffset.x + 0.5),
            int(pos.y * m_TouchArea.y + m_TouchOffset.y) + 0.5);
}

boost::mutex& MultitouchInputDevice::getMutex()
{
    return *m_pMutex;
}

int MultitouchInputDevice::getNextContactID()
{
    static int lastID = 0;
    return lastID++;
}

}
