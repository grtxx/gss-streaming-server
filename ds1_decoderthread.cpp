//---------------------------------------------------------------------------


#pragma hdrstop

#include "ds1_decoderthread.h"

//---------------------------------------------------------------------------

__fastcall tDecoderThread::tDecoderThread(tCircularBuffer *pbuffer): TThread(true)
{
  eof = false;
  bitBuffer=pbuffer;
  pcmBuffer = new tCircularBuffer(96*1024);

  inbuffer_event=CreateEvent(NULL, true, false, NULL);
  outbuffer_event=CreateEvent(NULL, true, false, NULL);

  bitBuffer->in_event_id=inbuffer_event;
  pcmBuffer->out_event_id=outbuffer_event;
}

bool __fastcall tDecoderThread::InitAAC()
{
  AACParams.hDecoder=faacDecOpen();
  AACParams.Config=faacDecGetCurrentConfiguration(AACParams.hDecoder);
  AACParams.Config->defSampleRate = 44100;
  AACParams.Config->outputFormat = FAAD_FMT_16BIT;
  AACParams.Config->dontUpSampleImplicitSBR = 1;
  faacDecSetConfiguration(AACParams.hDecoder, AACParams.Config);
  AACParams.firsttime=true;
}

bool __fastcall tDecoderThread::DeInitAAC()
{
  faacDecClose(AACParams.hDecoder);
  return true;
}

void __fastcall tDecoderThread::DecodeAAC()
{
  char *buffer=new char[16384];
  void *outBuffer;
  long bufsize;
  unsigned long smr;
  unsigned char chn;
  if ( (bitBuffer->nowInBuffer>1024) )
  {
    bitBuffer->peekData(buffer, 1024);
    if (AACParams.firsttime)
    {
      faacDecInit(AACParams.hDecoder, (char*)buffer, 1024, &smr, &chn);
      AACParams.firsttime=false;
    } else {
      outBuffer = faacDecDecode(AACParams.hDecoder, &AACParams.FrameInfo, buffer, 1024);
      bitBuffer->advance(AACParams.FrameInfo.bytesconsumed);
    }
    pcmBuffer->insertData((char*)outBuffer, AACParams.FrameInfo.samples*2);
    outBuffer=NULL;
  }
  delete buffer;  
}

bool __fastcall tDecoderThread::InitRAW()
{
  return true;
}

bool __fastcall tDecoderThread::DeInitRAW()
{
  return true;
}

void __fastcall tDecoderThread::DecodeRAW()
{
        char *buffer;
        buffer=new char[768];
        while ( (bitBuffer->nowInBuffer>0) )
        {
                int i=(bitBuffer->nowInBuffer>768 ? 768 : bitBuffer->nowInBuffer);
                bitBuffer->getData(buffer,i);
                pcmBuffer->insertData(buffer,i);
        }
        delete buffer;
}

bool __fastcall tDecoderThread::initDecoder(tEncoderEnum enctype)
{
    DecoderParams.decodingengine = enctype;
    if ( ENCODER_AAC == DecoderParams.decodingengine ) InitAAC();
    if ( ENCODER_RAW == DecoderParams.decodingengine ) InitRAW();
}

void __fastcall tDecoderThread::Execute()
{
  HANDLE handles[2];
  while ( ! eof )
  {
    handles[0]=inbuffer_event;
    handles[1]=outbuffer_event;
    WaitForMultipleObjects(2, &handles[0], false, 10);
    ResetEvent(inbuffer_event);
    ResetEvent(outbuffer_event);
    if ((pcmBuffer->nowInBuffer*100/pcmBuffer->bufferSize)<70 )
    {
      if ( ENCODER_AAC == DecoderParams.decodingengine ) DecodeAAC();
      if ( ENCODER_RAW == DecoderParams.decodingengine ) DecodeRAW();
    }
  }
  if ( ENCODER_AAC == DecoderParams.decodingengine ) DeInitAAC();
  if ( ENCODER_RAW == DecoderParams.decodingengine ) DeInitRAW();
  delete pcmBuffer;
}

#pragma package(smart_init)
