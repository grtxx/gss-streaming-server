//---------------------------------------------------------------------------


#pragma hdrstop

#include "ds1_recordingthread.h"

//---------------------------------------------------------------------------


__fastcall tRecordThread::tRecordThread(IDirectSoundCapture *ADS,  int samplerate, int channels, int resolution): TThread(true)
{
  DS=ADS;
  _DSCBUFFERDESC BufDesc;
  HResult hr;
  WAVEFORMATEX WaveEx;

//  FreeOnTerminate=true;
  ZeroMemory( &WaveEx, sizeof(WaveEx) );
  ZeroMemory( &WaveEx, sizeof(WaveEx) );
  WaveEx.wFormatTag=WAVE_FORMAT_PCM;
  WaveEx.nChannels=channels;
  WaveEx.nSamplesPerSec=samplerate;
  WaveEx.wBitsPerSample=resolution;
  WaveEx.nAvgBytesPerSec=WaveEx.nSamplesPerSec*WaveEx.wBitsPerSample/8*WaveEx.nChannels;
  WaveEx.nBlockAlign=WaveEx.wBitsPerSample/8*WaveEx.nChannels;

  ZeroMemory( &BufDesc, sizeof(BufDesc) );
  BufDesc.dwBufferBytes=bufferbytes;
  BufDesc.dwFlags= 0;
  BufDesc.lpwfxFormat=&WaveEx;
  BufDesc.dwSize=sizeof(BufDesc);

  if FAILED( DS->CreateCaptureBuffer(&BufDesc, &captureBuffer, NULL) )
  {
    MessageBox(0, "Cannot create directsound capture buffer\n\rNincs DS CaptureBuffer", "Error!", 16);
  }
  endEvent=NULL;

  cBuffer=new tCircularBuffer(1024*512);
}

void __fastcall tRecordThread::Execute()
{
  unsigned long playcursor, readcursor;
  short *ptr1, *ptr2;
  char *ptrx;
  unsigned long bytes1, bytes2;
  unsigned long i;
  short maxleft, maxright;
  int leftleds, rightleds;
  long lastreadcursor = 0;

  eof=false;
  captureBuffer->Start(DSCBSTART_LOOPING);
  do
  {
    captureBuffer->GetCurrentPosition(&playcursor, &readcursor);

    // Updating current volume parameters
    captureBuffer->Lock((readcursor>1000 ? readcursor-1000 : readcursor+bufferbytes-1000),1000,(void**)&ptr1,&bytes1,(void**)&ptr2,&bytes2,0);
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
    captureBuffer->Unlock((void*)ptr1,bytes1,(void*)ptr2,bytes2);

    captureBuffer->GetCurrentPosition(&playcursor, &readcursor);
    // Downoloading samples to cBuffer
    if ( ((playcursor>=bufferhalf) && (readcursor>=bufferhalf) && (lastreadcursor==0)) ||
         ((playcursor< bufferhalf) && (readcursor< bufferhalf) && (lastreadcursor==bufferhalf)) )
    {
      captureBuffer->Lock(lastreadcursor,bufferhalf,(void**)&ptr1,&bytes1,(void**)&ptr2,&bytes2,0);
      {
        cBuffer->insertData((char*)ptr1, bytes1);
        cBuffer->insertData((char*)ptr2, bytes2);
      }
      captureBuffer->Unlock((void*)ptr1,bytes1,(void*)ptr2,bytes2);
      lastreadcursor=(lastreadcursor==0 ? bufferhalf : 0);
    }
    Sleep(20);
  } while (! eof);
  captureBuffer->Stop();
  captureBuffer->Release();
  leftvol=0;
  rightvol=0;
  if (endEvent!=NULL) SetEvent(endEvent);
}

#pragma package(smart_init)
