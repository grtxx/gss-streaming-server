//---------------------------------------------------------------------------

#ifndef ds1_decoderthreadH
#define ds1_decoderthreadH
//---------------------------------------------------------------------------

#include <windows.h>
#include <classes.hpp>
#include "stdio.h"
#include "circbuffer.h"
#include "frame.h"
#include "faad.h"

#ifndef ENCODERENUM_DEFINED
#define ENCODERENUM_DEFINED
typedef enum { ENCODER_RAW, ENCODER_AAC, ENCODER_MP3 } tEncoderEnum;
#endif

typedef struct
{
        faacDecHandle hDecoder;
        faacDecConfiguration *Config;
        faacDecFrameInfo FrameInfo;
        bool firsttime;
} tAACDecParams;

typedef struct
{
        tEncoderEnum decodingengine;
} tDecoderParams;

class tDecoderThread : public TThread
{
private:
        tAACDecParams AACParams;
        tDecoderParams DecoderParams;
        HANDLE inbuffer_event;
        HANDLE outbuffer_event;
public:
        bool eof;
        tCircularBuffer *pcmBuffer;
        tCircularBuffer *bitBuffer;
        __fastcall tDecoderThread(tCircularBuffer *pbuffer);
        void __fastcall Execute();
        void __fastcall DecodeAAC();
        bool __fastcall InitAAC();
        bool __fastcall DeInitAAC();        
        void __fastcall DecodeRAW();
        bool __fastcall InitRAW();
        bool __fastcall DeInitRAW();
        bool __fastcall initDecoder(tEncoderEnum enctype);
};


#endif
