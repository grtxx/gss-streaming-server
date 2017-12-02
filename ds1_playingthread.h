//---------------------------------------------------------------------------

#ifndef ds1_playingthreadH
#define ds1_playingthreadH
//---------------------------------------------------------------------------

#include <windows.h>
#include <classes.hpp>
#include <mmsystem.h>
#include <math.h>
#include <io.h>
#include "c:\DXSDK\Include\dsound.h"
#include "circbuffer.h"
#include "dateutils.hpp"

#ifndef bufferbytes_defined
#define bufferbytes_defined
const int bufferbytes=32*1024;
const int bufferhalf=bufferbytes/2;
#endif

class tBufferPlayThread : public TThread
{
protected:
        long bufavg[1000];
public:
        IDirectSound *DS;
        IDirectSoundBuffer *SecBuf;
        short leftvol, rightvol;
        tCircularBuffer *cBuffer;
        HANDLE endEvent;
        HANDLE ds_event;
        bool eof;
        int minbuffer;
        long underruns;
        bool freqcontrol;
        long currentfreq;
        long freqinterval;
        long lastfreqchange;
        long idealbuffersize;
        long idealfreq;
        void __fastcall Execute();
        __fastcall tBufferPlayThread(IDirectSound *ADS, int samplerate, int channels, int resolution, tCircularBuffer *buf);
};

#endif

