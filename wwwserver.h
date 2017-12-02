//---------------------------------------------------------------------------

#ifndef wwwserverH
#define wwwserverH

#include <gsockets.h>
#include <classes.hpp>
#include <circbuffer.h>
#include <stdio.h>
#include <titledownloader.h>

class tWWWServersocket : public TThread
{
public:
        WORD port;
        int serverSocket;
        bool eof;
        tCircularBuffer *inBuffer;
        long Clients;
        AnsiString StreamName;
        AnsiString StreamURL;
        AnsiString StreamGenre;
        bool StreamPublic;
        int StreamBitRate;
        cTitleDownloader *Titles;
        HANDLE endEvent;
//        __fastcall ~tWWWServersocket();
        __fastcall tWWWServersocket(int listenport);
        void __fastcall Execute();
};

class tWWWClientSocket : public TThread
{
public:
        int socket;
        AnsiString HTTPHeaders[100];
        int headercount;
        tWWWServersocket *WS;
        __fastcall tWWWClientSocket(int clientsocket, tWWWServersocket *AWS);
        __fastcall HTTPGET();
        AnsiString getheader(AnsiString headername);
        void __fastcall Execute();
};

//---------------------------------------------------------------------------
#endif
