//---------------------------------------------------------------------------


#pragma hdrstop

#include "ds1_encoderthread.h"


//---------------------------------------------------------------------------

__fastcall tEncoderThread::tEncoderThread(tCircularBuffer *pbuffer): TThread(true)
{
  pcmBuffer = NULL;
  eof = false;
  pcmBuffer=pbuffer;
  inbuffer_event=CreateEvent(NULL, true, false, NULL);
  outbuffer_event=CreateEvent(NULL, true, false, NULL);

//  FreeOnTerminate=true;
  bitBuffer = new tCircularBuffer(256*1024);
  bitBuffer->out_event_id=outbuffer_event;
  pcmBuffer->in_event_id=inbuffer_event;
  endEvent=NULL;
}

bool __fastcall tEncoderThread::initEncoder(tEncoderEnum enctype, int samplerate, int channels, int bitrate, AnsiString filename)
{
  encoderParams.encodingengine=enctype;
  encoderParams.samplerate=samplerate;
  encoderParams.channels=channels;
  encoderParams.bitrate=bitrate;
  outFileName=filename;
  if ( ENCODER_AAC == encoderParams.encodingengine ) return InitAAC();
  if ( ENCODER_RAW == encoderParams.encodingengine ) return InitRAW();
  if ( ENCODER_MP3 == encoderParams.encodingengine ) return InitMP3();
}

void __fastcall tEncoderThread::EncodeMP3()
{
  while (pcmBuffer->nowInBuffer>(MP3Params.dwInputBufferLength*2) && (! eof))
  {
    pcmBuffer->getData((char*)MP3Params.InputBuffer, MP3Params.dwInputBufferLength*2);
    {
      DWORD dwOutBufLen=0;
      if ( beEncodeChunk(MP3Params.hStream,
                         MP3Params.dwInputBufferLength,
                         MP3Params.InputBuffer,
                         (BYTE*)MP3Params.Mp3Buffer,
                         &dwOutBufLen)==BE_ERR_SUCCESSFUL )
      {
        bitBuffer->insertData(MP3Params.Mp3Buffer, dwOutBufLen);
      }
    }
  }
}

bool __fastcall tEncoderThread::InitMP3()
{
  MP3Params.mp3config.format.mp3.dwSampleRate=encoderParams.samplerate;
  MP3Params.mp3config.format.mp3.byMode=(encoderParams.channels==2 ? BE_MP3_MODE_STEREO : BE_MP3_MODE_MONO);
  MP3Params.mp3config.format.mp3.wBitrate=encoderParams.bitrate;
  MP3Params.mp3config.format.mp3.bCopyright=false;
  MP3Params.mp3config.format.mp3.bCRC=false;
  MP3Params.mp3config.format.mp3.bOriginal=false;
  MP3Params.mp3config.format.mp3.bPrivate=false;

  beInitStream(&MP3Params.mp3config, &MP3Params.dwInputBufferLength, &MP3Params.dwOutputBufferLength, &MP3Params.hStream);
  MP3Params.InputBuffer=new short[MP3Params.dwInputBufferLength];
  MP3Params.Mp3Buffer=new char[MP3Params.dwOutputBufferLength];
  return true;
}

bool __fastcall tEncoderThread::DeInitMP3()
{
  DWORD dwOutBufLen=0;
  beDeinitStream(MP3Params.hStream,(BYTE*)MP3Params.Mp3Buffer,&dwOutBufLen);
  beCloseStream(MP3Params.hStream);
  bitBuffer->insertData(MP3Params.Mp3Buffer, dwOutBufLen);
  delete MP3Params.Mp3Buffer;
  delete MP3Params.InputBuffer;
  return true;
}

bool __fastcall tEncoderThread::InitAAC()
{
  faacEncConfiguration *aacformat;

  AACParams.hEncoder = faacEncOpen(encoderParams.samplerate,
                                   encoderParams.channels,
                                   &AACParams.inputsamples,
                                   &AACParams.outputsize);
  AACParams.channels=encoderParams.channels;
  aacformat=faacEncGetCurrentConfiguration(AACParams.hEncoder);
  aacformat->aacObjectType=LOW;
  aacformat->mpegVersion=MPEG4;
  aacformat->useTns=1;
  aacformat->useLfe=0;
  aacformat->allowMidside=0;
  aacformat->bitRate=(encoderParams.bitrate/encoderParams.channels)*1000;
  aacformat->bandWidth=0;
  aacformat->quantqual=1000;
  aacformat->outputFormat=1;
  aacformat->inputFormat = FAAC_INPUT_16BIT;
  return faacEncSetConfiguration(AACParams.hEncoder, aacformat);
}

bool __fastcall tEncoderThread::DeInitAAC()
{
        int byteswritten;
        char buffer[65536];
        try
        {
          byteswritten=faacEncEncode(AACParams.hEncoder,
                                     NULL,
                                     0,
                                     (unsigned char*)buffer,
                                     AACParams.outputsize);
          bitBuffer->insertData(buffer,AACParams.outputsize);
        }
        catch (...)
        {
        }
        faacEncClose(AACParams.hEncoder);
        return true;
}

void __fastcall tEncoderThread::EncodeAAC()
{
  while (pcmBuffer->nowInBuffer>AACParams.inputsamples*AACParams.channels)
  {
    char *inbuffer;
    char *outbuffer;
    int byteswritten=0;
    inbuffer=new char[AACParams.inputsamples*AACParams.channels];
    outbuffer=new char[AACParams.outputsize];
    pcmBuffer->getData(inbuffer,AACParams.inputsamples*AACParams.channels);
    try
    {
      byteswritten=faacEncEncode(AACParams.hEncoder,
                                 (int *)inbuffer,
                                 AACParams.inputsamples,
                                 (unsigned char*)outbuffer,
                                 AACParams.outputsize);
    }
    catch (...)
    {
    }
    if ( byteswritten!=0 )
    {
      bitBuffer->insertData(outbuffer, byteswritten);
    }
    delete inbuffer;
    delete outbuffer;
  }
}

void __fastcall tEncoderThread::EncodeRAW()
{
        char *buffer;
        buffer=new char[768];
        while (pcmBuffer->nowInBuffer>=768)
        {
                int i=(pcmBuffer->nowInBuffer>768 ? 768 : pcmBuffer->nowInBuffer);
                pcmBuffer->getData(buffer,i);
                bitBuffer->insertData(buffer,i);
        }
        delete buffer;
}

bool __fastcall tEncoderThread::InitRAW()
{
        return true;
}

bool __fastcall tEncoderThread::DeInitRAW()
{
        return true;
}

__fastcall tEncoderThread::~tEncoderThread()
{
}

void __fastcall tEncoderThread::Execute()
{
  HANDLE handles[2];
  while ( ! eof )
  {
    handles[0]=inbuffer_event;
    handles[1]=outbuffer_event;
    WaitForMultipleObjects(2, &handles[0], false, 1000);
    ResetEvent(inbuffer_event);
    ResetEvent(outbuffer_event);

    if (pcmBuffer!=NULL)
    {
      if ( ENCODER_AAC == encoderParams.encodingengine ) EncodeAAC();
      if ( ENCODER_RAW == encoderParams.encodingengine ) EncodeRAW();
      if ( ENCODER_MP3 == encoderParams.encodingengine ) EncodeMP3();
    }
  }
  if ( ENCODER_AAC == encoderParams.encodingengine ) DeInitAAC();
  if ( ENCODER_RAW == encoderParams.encodingengine ) DeInitRAW();
  if ( ENCODER_MP3 == encoderParams.encodingengine ) DeInitMP3();
  delete bitBuffer;
  if (endEvent!=NULL) SetEvent(endEvent);
}

#pragma package(smart_init)
