//---------------------------------------------------------------------------

#ifndef circbufferH
#define circbufferH
//---------------------------------------------------------------------------

#include <windows.h>

class tCircularBuffer;
class tBufReader;

class tBufReader
{
public:
        long bufferStart;
        long nowInBuffer;
        tCircularBuffer *parent;
        __fastcall tBufReader();
        int getData(char *data, int count);
        tBufReader *nextreader;
        HANDLE in_event_id;
};

class tCircularBuffer
{
public:
        char *buffer;
        long bufferSize;
        long bufferStart, bufferEnd;
        long nowInBuffer;
        HANDLE in_event_id;
        HANDLE out_event_id;
        HANDLE hSemafor;
        tBufReader *FirstReader;
        __fastcall tCircularBuffer();
        __fastcall tCircularBuffer(int bufsize);
        __fastcall ~tCircularBuffer();
        __fastcall initBuffer(int bufsize);
        __fastcall clear();
        __fastcall registerReader(tBufReader *R);
        __fastcall unRegisterReader(tBufReader *R);
        int insertData(char *data, int count);
        int getData(char *data, int count);
        int peekData(char *data, int count);
        int advance(int count);
};

#endif
