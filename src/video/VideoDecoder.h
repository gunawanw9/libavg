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

#ifndef _VideoDecoder_H_
#define _VideoDecoder_H_

#include "../api.h"
#include "../avgconfigwrapper.h"

#include "VideoInfo.h"

#include "../graphics/PixelFormat.h"

#include "WrapFFMpeg.h"

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

namespace avg {

class Bitmap;
typedef boost::shared_ptr<Bitmap> BitmapPtr;
struct AudioParams;

enum FrameAvailableCode {
    FA_NEW_FRAME, FA_USE_LAST_FRAME, FA_STILL_DECODING
};

enum StreamSelect {
    SS_AUDIO, SS_VIDEO, SS_DEFAULT
};

class AVG_API VideoDecoder
{
    public:
        enum DecoderState {CLOSED, OPENED, DECODING};
        VideoDecoder();
        virtual ~VideoDecoder();
        virtual void open(const std::string& sFilename, bool bEnableSound);
        virtual void startDecoding(bool bDeliverYCbCr, const AudioParams* pAP);
        virtual void close();
        virtual DecoderState getState() const;
        VideoInfo getVideoInfo() const;
        PixelFormat getPixelFormat() const;
        IntPoint getSize() const;
        float getStreamFPS() const;

        virtual void seek(float destTime) = 0;
        virtual void loop() = 0;
        virtual int getCurFrame() const = 0;
        virtual int getNumFramesQueued() const = 0;
        virtual float getCurTime() const = 0;
        virtual float getFPS() const = 0;
        virtual void setFPS(float fps) = 0;

        virtual FrameAvailableCode getRenderedBmp(BitmapPtr& pBmp, float timeWanted);
        virtual FrameAvailableCode getRenderedBmps(std::vector<BitmapPtr>& pBmps,
                float timeWanted) = 0;
        virtual bool isEOF() const = 0;
        virtual void throwAwayFrame(float timeWanted) = 0;

        // Prevents different decoder instances from executing open/close simultaneously
        static boost::mutex s_OpenMutex;

    protected:
        int getNumFrames() const;
        AVFormatContext* getFormatContext();
        AVCodecContext const * getCodecContext() const;
        AVCodecContext * getCodecContext();
        void allocFrameBmps(std::vector<BitmapPtr>& pBmps);

        int getVStreamIndex() const;
        AVStream* getVideoStream() const;
        int getAStreamIndex() const;
        AVStream* getAudioStream() const;

    private:
        void initVideoSupport();
        int openCodec(int streamIndex);
        float getDuration(StreamSelect streamSelect) const;
        PixelFormat calcPixelFormat(bool bUseYCbCr);
        std::string getStreamPF() const;

        DecoderState m_State;
        AVFormatContext * m_pFormatContext;
        std::string m_sFilename;

        // Video
        int m_VStreamIndex;
        AVStream * m_pVStream;
        PixelFormat m_PF;
        IntPoint m_Size;
        
        // Audio
        int m_AStreamIndex;
        AVStream * m_pAStream;
        
        static bool s_bInitialized;
};

typedef boost::shared_ptr<VideoDecoder> VideoDecoderPtr;

void avcodecError(const std::string& sFilename, int err);

}
#endif 

