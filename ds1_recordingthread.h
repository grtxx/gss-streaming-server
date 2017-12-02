//---------------------------------------------------------------------------

#ifndef ds1_recordingthreadH
#define ds1_recordingthreadH

#include <windows.h>
#include <classes.hpp>
#include <mmsystem.h>
#include <math.h>
#include <io.h>
#include "c:/DXSDK/Include/dsound.h"
#include "circbuffer.h"

#ifndef bufferbytes_defined
#define bufferbytes_defined
const int bufferbytes=64*1024;
const int bufferhalf=bufferbytes/2;
#endif

class tRecordThread : public TThread
{
public:
        IDirectSoundCapture *DS;
        IDirectSoundCaptureBuffer *captureBuffer;
        int rfile;
        bool eof;
        short leftvol, rightvol;
        tCircularBuffer *cBuffer;
        HANDLE endEvent;
        __fastcall tRecordThread(IDirectSoundCapture *ADS, int channels, int samplerate, int resolution);
        void __fastcall Execute();
};

//---------------------------------------------------------------------------
#endif
