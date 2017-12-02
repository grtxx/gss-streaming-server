//---------------------------------------------------------------------------


#pragma hdrstop
#include "ds1_playingthread.h"

//------------------------------------------------------------------------------

__fastcall tBufferPlayThread::tBufferPlayThread(IDirectSound *ADS, int samplerate, int channels, int resolution, tCircularBuffer *buf) : TThread( true )
{
  DS=ADS;
  _DSBUFFERDESC BufDesc;
  HResult hr;
  WAVEFORMATEX WaveEx;
  IDirectSoundNotify *DN;

  cBuffer=buf;
  minbuffer=cBuffer->bufferSize/2;
 
  ZeroMemory( &WaveEx, sizeof(WaveEx) );
  WaveEx.wFormatTag=WAVE_FORMAT_PCM;
  WaveEx.nChannels=channels;
  WaveEx.nSamplesPerSec=samplerate;
  WaveEx.wBitsPerSample=resolution;
  WaveEx.nAvgBytesPerSec=WaveEx.nSamplesPerSec*WaveEx.wBitsPerSample/8*WaveEx.nChannels;
  WaveEx.nBlockAlign=WaveEx.wBitsPerSample/8*WaveEx.nChannels;

  ZeroMemory( &BufDesc, sizeof(BufDesc) );
  BufDesc.dwBufferBytes=bufferbytes;
  BufDesc.dwFlags= DSBCAPS_CTRLFREQUENCY | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY;
  BufDesc.lpwfxFormat=&WaveEx;
  BufDesc.dwSize=sizeof(BufDesc);

  DS->CreateSoundBuffer(&BufDesc, &SecBuf, NULL);
  ds_event=0;
  if ( SUCCEEDED( SecBuf->QueryInterface(IID_IDirectSoundNotify, (void**)&DN) ) )
  {
    _DSBPOSITIONNOTIFY Position[2];

    ds_event=CreateEvent(NULL, true, false, NULL);

    Position[0].dwOffset=0;
    Position[0].hEventNotify=ds_event;

    Position[1].dwOffset=bufferhalf;
    Position[1].hEventNotify=ds_event;

    DN->SetNotificationPositions(2, &Position[0]);
    DN->Release();
  }
  idealfreq=samplerate;
  idealbuffersize=minbuffer;
  freqcontrol=false;
  lastfreqchange=0;
  freqinterval=300;
  currentfreq=idealfreq;
}

int bposadd(int base, int increment, int bufsize)
{
  return (base+increment<=bufsize ? base+increment : base+increment-bufsize);
}

void __fastcall tBufferPlayThread::Execute()
{
  unsigned long playcursor, writecursor;
  short *ptr1, *ptr2;
  char *ptrx;
  unsigned long bytes1, bytes2;
  unsigned long i;
  short maxleft, maxright;
  int leftleds, rightleds;
  long lastwrite = 0;
  bool bufferingmode = false;
  HANDLE event_id, event_id2;

  event_id = CreateEvent(NULL, true, false, NULL);

  SecBuf->Play(0,0,DSBPLAY_LOOPING);

  eof=false;
  do
  {
    SecBuf->GetCurrentPosition(&playcursor, &writecursor);
    // Updating current volume parameters
    SecBuf->Lock((playcursor>1000 ? playcursor-1000 : playcursor+bufferbytes-1000),1000,(void**)&ptr1,&bytes1,(void**)&ptr2,&bytes2,0);
    leftvol=0;
    rightvol=0;
    for (i=0; i<bytes1/4; i++)
    {
      if ( leftvol <abs(ptr1[i*2+0]) ) leftvol= abs(ptr1[i*2+0]);
      if ( rightvol<abs(ptr1[i*2+1]) ) rightvol=abs(ptr1[i*2+1]);
    }
    for (i=0; i<bytes2/4; i++)
    {
      if ( leftvol <abs(ptr2[i*2+0]) ) leftvol= abs(ptr2[i*2+0]);
      if ( rightvol<abs(ptr2[i*2+1]) ) rightvol=abs(ptr2[i*2+1]);
    }
    SecBuf->Unlock((void*)ptr1,bytes1,(void*)ptr2,bytes2);

    // Uploading samples
    SecBuf->GetCurrentPosition(&playcursor, &writecursor);
    if ( ((playcursor>=bufferhalf) && (writecursor>=bufferhalf) && (lastwrite==0)) ||
         ((playcursor< bufferhalf) && (writecursor< bufferhalf) && (lastwrite==bufferhalf)) )
    {
      SecBuf->Lock(lastwrite,
                   bufferhalf,
                   (void**)&ptr1,
                   &bytes1,
                   (void**)&ptr2,
                   &bytes2,0);
      if ( cBuffer->nowInBuffer>(bytes1+bytes2)*1 && (!bufferingmode))
      {
        cBuffer->getData((char*)ptr1, bytes1);
        cBuffer->getData((char*)ptr2, bytes2);
      } else {
        ZeroMemory(ptr1, bytes1);
        ZeroMemory(ptr2, bytes2);
        if (!bufferingmode) underruns++;
        bufferingmode=(cBuffer->nowInBuffer<minbuffer && cBuffer->nowInBuffer<cBuffer->bufferSize/2);
      }
      SecBuf->Unlock((void*)ptr1,bytes1,(void*)ptr2,bytes2);
      lastwrite=(lastwrite==0 ? bufferhalf : 0);
    }
    // Frequency control
    if (freqcontrol)
    {
       float nwinbuf=0;
       {
            int i;

            for (i=100;i>0;i--) bufavg[i]=bufavg[i-1];
            bufavg[0]=cBuffer->nowInBuffer;

            for (i=0;i<100;i++) nwinbuf+=bufavg[i];
            nwinbuf=nwinbuf/100;
       }
       if (lastfreqchange<DateTimeToUnix(Now()))
       {
         currentfreq=(nwinbuf-idealbuffersize)*freqinterval/idealbuffersize+idealfreq;
         SecBuf->SetFrequency(currentfreq);
         lastfreqchange=DateTimeToUnix(Now());
       }
    }

    if ( 0 == ds_event )
    {
      Sleep(5);
    } else {
      WaitForSingleObject( ds_event, 5 );
      ResetEvent( ds_event );
    }
  } while (! eof);
  SecBuf->Stop();
  SecBuf->Release();
  leftvol=0;
  rightvol=0;
  if (endEvent!=NULL) SetEvent(endEvent);  
}

#pragma package(smart_init)
