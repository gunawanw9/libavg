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

#ifndef _BlurFXNode_H_
#define _BlurFXNode_H_

#include "../api.h"

#include "FXNode.h"

#include <boost/shared_ptr.hpp>

namespace avg {

class GPUBlurFilter;
typedef boost::shared_ptr<GPUBlurFilter> GPUBlurFilterPtr;

class AVG_API BlurFXNode: public FXNode {
public:
    BlurFXNode(float radius=1.f);
    virtual ~BlurFXNode();

    void connect();
    virtual void disconnect();

    void setRadius(float stdDev);
    float getRadius() const;

private:
    virtual GPUFilterPtr createFilter(const IntPoint& size);

    GPUBlurFilterPtr m_pFilter;

    float m_StdDev;
};

typedef boost::shared_ptr<BlurFXNode> BlurFXNodePtr;

}

#endif

