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
//  Original author of this file is Nick Hebner (hebnern@gmail.com).
//

#include "ArgBase.h"

#include "../base/Exception.h"

#include <stdlib.h>

using namespace std;

namespace avg {

ArgBase::ArgBase(string sName, bool bRequired, ptrdiff_t memberOffset)
    : m_sName(sName),
      m_bRequired(bRequired),
      m_MemberOffset(memberOffset)
{
    m_bDefault = true;
}

ArgBase::~ArgBase()
{
}
    
string ArgBase::getName() const
{
    return m_sName;
}

bool ArgBase::isDefault() const
{
    return m_bDefault;
}

bool ArgBase::isRequired() const
{
    return m_bRequired;
}

ptrdiff_t ArgBase::getMemberOffset() const
{
    return m_MemberOffset;
}

}
