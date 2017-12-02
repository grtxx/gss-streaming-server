//---------------------------------------------------------------------------


#pragma hdrstop

#include "wfx.h"

__fastcall tWFXThread::~tWFXThread()
{
}

__fastcall tWFXThread::tWFXThread(tCircularBuffer *pbuffer): TThread(true)
{
  eof = false;
  channels=2;
  srate=44100;
  
  fxinBuffer=pbuffer;
  fxoutBuffer = new tCircularBuffer(256*1024);

  inbuffer_event=CreateEvent(NULL, true, false, NULL);
  outbuffer_event=CreateEvent(NULL, true, false, NULL);

  fxinBuffer->in_event_id=inbuffer_event;
  fxoutBuffer->out_event_id=outbuffer_event;
  endEvent=NULL;
  DSPSemafor=CreateEvent(NULL,false,false,NULL);
  SetEvent(DSPSemafor);
}

int __fastcall tWFXThread::DSPinit(AnsiString dllname)
{
  int (__stdcall *DSPGetHeader)();
  int retval=0;

  WaitForSingleObject(DSPSemafor,1000);
  ResetEvent(DSPSemafor);
  try
  {
    if (DSPM!=NULL)
    {
      SetEvent(DSPSemafor);
      DSPquit();
      WaitForSingleObject(DSPSemafor,1000);
      ResetEvent(DSPSemafor);
    }
    dspLib=LoadLibrary(dllname.c_str());
    DSPGetHeader=NULL;
    DSPGetHeader=GetProcAddress(dspLib,"winampDSPGetHeader2");
    DSPH=(winampDSPHeader*)DSPGetHeader();
  }
  catch (...)
  {
    retval=-1;
  }
  SetEvent(DSPSemafor);
  return retval;
}

int __fastcall tWFXThread::DSPinitModule(int modulenumber, HANDLE aHandle)
{
  int retval = 0;
  WaitForSingleObject(DSPSemafor,1000);
  ResetEvent(DSPSemafor);
  if (DSPH!=NULL)
  {
    try
    {
      DSPM=DSPH->getModule(modulenumber);
    }
    catch (...)
    {
      DSPM=NULL;
    }
    if (DSPM!=NULL)
    {
      DSPM->userData=NULL;
      DSPM->hwndParent=aHandle;
      DSPM->hDllInstance=dspLib;
      try
      {
        retval=DSPM->Init(DSPM);
      }
      catch (...)
      {
        retval=-1;
      }
    } else {
      SetEvent(DSPSemafor);
      DSPquit();
    }
  }
  SetEvent(DSPSemafor);
  return retval;
}

int __fastcall tWFXThread::DSPquit()
{
  WaitForSingleObject(DSPSemafor,1000);
  ResetEvent(DSPSemafor);
  try
  {
    if (DSPM!=NULL)
    {
      DSPM->Quit(DSPM);
      DSPM=NULL;
    }
    if (DSPH!=NULL)
    {
      DSPH=NULL;
      FreeLibrary(dspLib);
    }
  }
  catch (...)
  {
  }
  SetEvent(DSPSemafor);
}

int __fastcall tWFXThread::DSPconfig()
{
  WaitForSingleObject(DSPSemafor,1000);
  ResetEvent(DSPSemafor);
  try
  {
    DSPM->Config(DSPM);
  }
  catch (...)
  {
  }
  SetEvent(DSPSemafor);
  return 0;
}

void __fastcall tWFXThread::Execute()
{
  HANDLE handles[2];
  char buffer[64*1024];
  int c;
  int count;

  while ( ! eof )
  {
    handles[0]=inbuffer_event;
    handles[1]=outbuffer_event;
    WaitForMultipleObjects(2, &handles[0], false, 1000);
    ResetEvent(inbuffer_event);
    ResetEvent(outbuffer_event);
    if ((fxoutBuffer->nowInBuffer*100/fxoutBuffer->bufferSize)<70 )
    {
      if (fxinBuffer!=NULL)
      {
        while (fxinBuffer->nowInBuffer>8*1024 && fxoutBuffer->nowInBuffer<fxoutBuffer->bufferSize-32768 && (! eof))
        {
          c=fxinBuffer->nowInBuffer;
          if (c>4*1024) c=4*1024;
          if (c+fxoutBuffer->nowInBuffer>fxoutBuffer->bufferSize) c=fxoutBuffer->bufferSize-fxoutBuffer->nowInBuffer;
          fxinBuffer->getData(buffer,c);
          c=c/(2*channels);
          count=c;
          WaitForSingleObject(DSPSemafor,1000);
          ResetEvent(DSPSemafor);
          try
          {
            if (DSPM!=NULL)
            {
              count=DSPM->ModifySamples(DSPM,(short*)&buffer,c,16,channels,srate);
            }
          }
          catch (...)
          {
          }
          SetEvent(DSPSemafor);
          fxoutBuffer->insertData(buffer,count*2*channels);
        }
      }
    }
  }
  DSPquit();
  delete fxoutBuffer;
  if (endEvent!=NULL) SetEvent(endEvent);
  CloseHandle(DSPSemafor);
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
