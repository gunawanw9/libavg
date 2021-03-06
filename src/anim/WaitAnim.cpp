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

#include "WaitAnim.h"

#include "../player/Player.h"

using namespace boost;
using namespace boost::python;
using namespace std;

namespace avg {

WaitAnim::WaitAnim(long long duration, const object& startCallback, 
        const object& stopCallback)
    : Anim(startCallback, stopCallback),
      m_Duration(duration)
{
}

WaitAnim::~WaitAnim()
{
}
    
void WaitAnim::start(bool bKeepAttr)
{
    m_pThis = dynamic_pointer_cast<WaitAnim>(shared_from_this());
    Anim::start();
    m_StartTime = Player::get()->getFrameTime();
}

void WaitAnim::abort()
{
    setStopped();
    m_pThis = WaitAnimPtr();
}
    
bool WaitAnim::step()
{
    assert(isRunning());
    if (m_Duration != -1 && Player::get()->getFrameTime()-m_StartTime > m_Duration) {
        setStopped();
        m_pThis = WaitAnimPtr();
        return true;
    } else {
        return false;
    }

}

}
