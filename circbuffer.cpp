//---------------------------------------------------------------------------


#pragma hdrstop

#include "circbuffer.h"
#include "winbase.h"

__fastcall tBufReader::tBufReader()
{
  parent=NULL;
  nowInBuffer=0;
  in_event_id=0;
  nextreader=NULL;
}

int tBufReader::getData(char *data, int count)
{
  long i = 0;
  long pos = 0;

  if (parent!=NULL)
  {
    WaitForSingleObject(parent->hSemafor,1000);
    ResetEvent(parent->hSemafor);
    while (count>0)
    {
      i=(bufferStart+count<parent->bufferSize ? count : parent->bufferSize-bufferStart);
      CopyMemory((void*)&data[pos], (void*)&parent->buffer[bufferStart], i);   nowInBuffer=nowInBuffer-i;
      pos=pos+i;
      count=count-i;
      bufferStart=bufferStart+i;
      if (bufferStart>=parent->bufferSize) bufferStart=0;
    }
    SetEvent(parent->hSemafor);
    return 0;
  } else return -1;
}


//-------------------------------------------------------------------

__fastcall tCircularBuffer::tCircularBuffer(int bufsize)
{
  hSemafor=CreateEvent(NULL,true,false,NULL);
  SetEvent(hSemafor);
  initBuffer(bufsize);
  FirstReader=NULL;
}

__fastcall tCircularBuffer::tCircularBuffer()
{
  hSemafor=CreateEvent(NULL,true,false,NULL);
  SetEvent(hSemafor);
  initBuffer(65536);
  FirstReader=NULL;
}

__fastcall tCircularBuffer::~tCircularBuffer()
{
  delete buffer;
  CloseHandle(hSemafor);
  if (in_event_id!=0)  CloseHandle(in_event_id);
  if (out_event_id!=0) CloseHandle(out_event_id);
}

__fastcall tCircularBuffer::initBuffer(int bufsize)
{
  buffer=new char[bufsize];
  bufferSize=bufsize;
  bufferStart=0;
  bufferEnd=0;
  nowInBuffer=0;
  ZeroMemory(buffer, bufsize);
  in_event_id = 0;
  out_event_id = 0;
  return 0;
}

int tCircularBuffer::insertData(char *data, int count)
{
  long i = 0;
  long pos = 0;
  tBufReader *P;

  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);
  while (count>0)
  {
	i=(bufferEnd+count<bufferSize ? count : bufferSize-bufferEnd);
	CopyMemory((void*)&buffer[bufferEnd], (void*)&data[pos], i);   nowInBuffer=nowInBuffer+i;
	P=FirstReader;
	while (P!=NULL)
	{
	  P->nowInBuffer=P->nowInBuffer+i;
	  if (P->in_event_id!=0)  PulseEvent(P->in_event_id);
	  P=P->nextreader;
	}
	pos=pos+i;
	count=count-i;
	bufferEnd=bufferEnd+i;
	if (bufferEnd>=bufferSize) bufferEnd=0;
  }
  SetEvent(hSemafor);
  if (in_event_id!=0)  PulseEvent(in_event_id);
  return 0;
}

int tCircularBuffer::peekData(char *data, int count)
{
  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);
  long pos = bufferStart;
  long nIB= nowInBuffer;
  SetEvent(hSemafor);
  getData(data, count);
  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);

  bufferStart=pos;
  nowInBuffer=nIB;
  SetEvent(hSemafor);
  return 0;
}

int tCircularBuffer::advance(int count)
{
  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);

  bufferStart=(bufferStart+count) % bufferSize;
  nowInBuffer=(nowInBuffer>count ? nowInBuffer-count : 0);

  SetEvent(hSemafor);
  if (out_event_id!=0)  PulseEvent(out_event_id);
  return 0;
}

__fastcall tCircularBuffer::clear()
{
  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);

  nowInBuffer=0;
  bufferStart=0;
  bufferEnd=0;

  SetEvent(hSemafor);
  return 0;
}

__fastcall tCircularBuffer::registerReader(tBufReader *R)
{
  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);

  R->nextreader=FirstReader;
  FirstReader=R;
  R->bufferStart=bufferEnd-bufferSize/2;
  if (R->bufferStart<0) R->bufferStart+=bufferSize;
  R->nowInBuffer=bufferSize/2;
  R->parent=this;

  SetEvent(hSemafor);
  return 0;
}

__fastcall tCircularBuffer::unRegisterReader(tBufReader *R)
{
  tBufReader *P;
  tBufReader *Q=NULL;

  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);

  P=FirstReader;
  while (P!=NULL)
  {
    if (R==P)
    {
      if (Q==NULL) { FirstReader=P->nextreader; }
              else { Q->nextreader=P->nextreader; }
      R->parent=NULL;
    }
    Q=P;
    P=P->nextreader;
  }

  SetEvent(hSemafor);
  return 0;  
}

int tCircularBuffer::getData(char *data, int count)
{
  long i = 0;
  long pos = 0;

  WaitForSingleObject(hSemafor,1000);
  ResetEvent(hSemafor);

  {
    while (count>0)
    {
      i=(bufferStart+count<bufferSize ? count : bufferSize-bufferStart);
      CopyMemory((void*)&data[pos], (void*)&buffer[bufferStart], i);   nowInBuffer=nowInBuffer-i;
      pos=pos+i;
      count=count-i;
      bufferStart=bufferStart+i;
      if (bufferStart>=bufferSize) bufferStart=0;
    }
  }

  SetEvent(hSemafor);
  if (out_event_id!=0)  PulseEvent(out_event_id);
  return 0;
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
