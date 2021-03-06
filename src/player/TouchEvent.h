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
//  Original author of this file is igor@c-base.org
//

#ifndef _TouchEvent_H_
#define _TouchEvent_H_

#include "../api.h"
#include "CursorEvent.h"

#include "../base/GLMHelper.h"

#include <math.h>
#include <boost/weak_ptr.hpp>

namespace avg {

class TouchEvent;
typedef boost::shared_ptr<class TouchEvent> TouchEventPtr;
typedef boost::weak_ptr<class TouchEvent> TouchEventWeakPtr;

class AVG_API TouchEvent: public CursorEvent 
{
    public:
        TouchEvent(int id, Type eventType, const IntPoint& pos, Source source, 
                const glm::vec2& speed, float orientation, float area, 
                float eccentricity, glm::vec2 majorAxis, glm::vec2 minorAxis);
        TouchEvent(int id, Type eventType, const IntPoint& pos, Source source,
                const glm::vec2& speed=glm::vec2(0, 0));
        virtual ~TouchEvent();
        virtual CursorEventPtr copy() const;

        float getOrientation() const;
        float getArea() const;
        const glm::vec2 & getCenter() const;
        float getEccentricity() const;
        const glm::vec2 & getMajorAxis() const;
        const glm::vec2 & getMinorAxis() const;

        float getHandOrientation() const;

        void addRelatedEvent(TouchEventPtr pEvent);
        std::vector<TouchEventPtr> getRelatedEvents() const;

        virtual void trace();

    private:
        float m_Orientation;
        float m_Area;
        glm::vec2 m_Center;
        float m_Eccentricity;
        glm::vec2 m_MajorAxis;
        glm::vec2 m_MinorAxis;
        std::vector<TouchEventWeakPtr> m_RelatedEvents;
        bool m_bHasHandOrientation;
        float m_HandOrientation; 
};

}
#endif
