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

#include "GPUBandpassFilter.h"
#include "Bitmap.h"
#include "ShaderRegistry.h"
#include "OGLShader.h"
#include "FBO.h"
#include "GLContextManager.h"
#include "GLTexture.h"

#include "../base/ObjectCounter.h"
#include "../base/Exception.h"

#include <iostream>

#define SHADERID "bandpass"

using namespace std;

namespace avg {

GPUBandpassFilter::GPUBandpassFilter(const IntPoint& size, PixelFormat pfSrc, 
        float min, float max, float postScale, bool bInvert, bool bStandalone)
    : GPUFilter(pfSrc, B8G8R8A8, bStandalone, SHADERID),
      m_PostScale(postScale),
      m_bInvert(bInvert),
      m_MinFilter(size, pfSrc, R32G32B32A32F, min, true, false, true),
      m_MaxFilter(size, pfSrc, R32G32B32A32F, max, true, false, true)
{
    ObjectCounter::get()->incRef(&typeid(*this));
    
    GLContext::getCurrent()->ensureFullShaders("GPUBandpassFilter");

    setDimensions(size);
    GLContextManager* pCM = GLContextManager::get();
    m_pMinTexParam = pCM->createShaderParam<int>(SHADERID, "u_MinTex");
    m_pMaxTexParam = pCM->createShaderParam<int>(SHADERID, "u_MaxTex");
    m_pPostScaleParam = pCM->createShaderParam<float>(SHADERID, "u_PostScale");
    m_pInvertParam = pCM->createShaderParam<int>(SHADERID, "u_bInvert");
}

GPUBandpassFilter::~GPUBandpassFilter()
{
    ObjectCounter::get()->decRef(&typeid(*this));
}

void GPUBandpassFilter::applyOnGPU(GLContext* pContext, GLTexturePtr pSrcTex)
{
    m_MinFilter.apply(pContext, pSrcTex);
    m_MaxFilter.apply(pContext, pSrcTex);

    getFBO(pContext)->activate();
    getShader()->activate();
    m_pMinTexParam->set(pContext, 0);
    m_pMaxTexParam->set(pContext, 1);
    m_pPostScaleParam->set(pContext, float(m_PostScale));
    m_pInvertParam->set(pContext, m_bInvert);
    m_MaxFilter.getDestTex(pContext)->activate(WrapMode(), GL_TEXTURE1);
    draw(pContext, m_MinFilter.getDestTex(pContext), WrapMode());
}

}
