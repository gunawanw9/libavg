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

#ifndef _FilterIntensity_H_
#define _FilterIntensity_H_

#include "../api.h"
#include "Filter.h"

namespace avg {

class AVG_API FilterIntensity : public Filter
{
public:
    FilterIntensity(int offset, float factor);
    virtual ~FilterIntensity();
    virtual void applyInPlace(BitmapPtr pBmp) ;

private:
    int m_Offset;
    float m_Factor;
};

}  // namespace

#endif

