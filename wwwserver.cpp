//---------------------------------------------------------------------------


#pragma hdrstop

#include "wwwserver.h"

//---------------------------------------------------------------------------

__fastcall tWWWClientSocket::tWWWClientSocket(int clientsocket, tWWWServersocket *AWS): TThread(true)
{
  socket=clientsocket;
  WS=AWS;
  FreeOnTerminate=true;
}

AnsiString tWWWClientSocket::getheader(AnsiString headername)
{
  int i=0;
  AnsiString s1,s2;
  s1=headername.UpperCase();
  for (i=0;i<=headercount;i++)
  {
    s2=HTTPHeaders[i].SubString(0,HTTPHeaders[i].Pos(":")-1).UpperCase().Trim();
    if ( s1==s2 )
    {
      s1="";
      s2="";
      return HTTPHeaders[i].SubString(HTTPHeaders[i].Pos(":")+1,500).Trim();
    }
  }
}

__fastcall tWWWClientSocket::HTTPGET()
{
  AnsiString path;
  FILE *f;
  int count;
  bool eofc=false;
  bool metadata=false;
  AnsiString oldmetadata="????";
  path=HTTPHeaders[0].SubString(5,255);
  if (path=="/ HTTP/1.0" || getheader("User-agent").LowerCase().Pos("player")>0 || getheader("Icy-metadata")=="1")  // Stream content
  {
    try
    {
      tBufReader *Reader = new tBufReader();
      long count=0;
      char buffer[128*1024];
      char c;
      if (WS->inBuffer!=NULL)
      {
        WS->inBuffer->registerReader(Reader);
        Reader->in_event_id=CreateEvent(NULL,true,false,NULL);

        if ( getheader("Icy-metadata")=="1") metadata=true;
        switch (metadata)
        {
        case false: sendStringLn(socket, "HTTP/1.0 200 OK");
                    break;
        case true:  sendStringLn(socket, "ICY 200 OK");
                    break;
        }
        sendStringLn(socket, "Content-type: audio/mpeg");
        if (metadata)
        {
          sendStringLn(socket, "Icy-metaint: 32768");
          sendStringLn(socket, "Icy-Metadata: 1");
          sendStringLn(socket, "icy-name: "+WS->StreamName);
          sendStringLn(socket, "icy-url: "+WS->StreamURL);
          sendStringLn(socket, "icy-genre: "+WS->StreamGenre);
          sendStringLn(socket, "icy-br: "+IntToStr(WS->StreamBitRate));
          c=(WS->StreamPublic ? 49 : 48);
          sendStringLn(socket, "icy-pub: "+(char)c);
        }
        sendStringLn(socket, "");
        do
        {
          WaitForSingleObject(Reader->in_event_id,10000);
          ResetEvent(Reader->in_event_id);
          while (Reader->nowInBuffer>32768)
          {
            Reader->getData(buffer, 32768);
            count=send(socket, buffer, 32768, 0);

            if (metadata)
            {
              char buffer[4096];
              int bufferlen;
              int i;
              AnsiString metadata;
              metadata="StreamTitle='"+WS->Titles->currentTitle+"';StreamUrl='';";
              if (metadata!=oldmetadata)
              {
                for (i=1;i<=metadata.Length();i++) { buffer[i]=metadata[i]; }
                bufferlen=metadata.Length()+1;
                buffer[bufferlen-1]=0;
                while (bufferlen % 16!=0)
                {
                  bufferlen++;
                  buffer[bufferlen]=0;
                }
                buffer[0]=((byte)bufferlen)/16;
                send(socket,buffer,bufferlen+1, 0);
                oldmetadata=metadata;
                metadata="";
              } else {
                buffer[0]=0;
                send(socket,buffer, 1, 0);
              }
            }
          }
        } while (count>0 && WS->eof==false);
        WS->inBuffer->unRegisterReader(Reader);
        CloseHandle(Reader->in_event_id);
        delete Reader;
      } else {
        sendStringLn(socket, "HTTP/1.0 404 Service not available");
        sendStringLn(socket, "");
      }
    }
    catch (...)
    {
      sendStringLn(socket, "HTTP/1.0 403 Service not available");
      sendStringLn(socket, "");
    }
  } else {
    Priority=tpIdle;
    path="www"+path.SubString(0,path.Pos(" ")-1);
    if (path=="www/") path="www/index.html";
    if (path.Pos("..")!=0) path="www/";
    f=fopen(path.c_str(), "rb");
    if (f==NULL)
    {
      sendStringLn(socket, "HTTP/1.1 404 NOT FOUND");
      sendStringLn(socket, "");
      sendStringLn(socket, "I don't like you");
    } else {
      sendStringLn(socket, "HTTP/1.0 200 OK");
      fseek(f,0,2);
      sendStringLn(socket, "Content-length: "+IntToStr(ftell(f)));
      fseek(f,0,0);
      sendStringLn(socket, "");
      char buffer[16384];
      while (! eofc)
      {
        count=fread(&buffer, 1, 16384, f);
        count=count-send(socket, buffer, count, 0);
        eofc=((count!=0) || feof(f));
      }
    }
    fclose(f);
  }
}

void __fastcall tWWWClientSocket::Execute()
{
  AnsiString s;
  bool hEnd = false;
  char c;
  headercount=0;
  bool ok;

  WS->Clients++;
  while (! hEnd)
  {
     hEnd=recv(socket,&c,1,0)!=1;
     if (headercount>50) hEnd=true;
     if (c!=13 && c!=10) { s=s+c; } else
     {
       if (c==10)
       {
         if (s=="")
         {
           hEnd=true;
         } else {
           HTTPHeaders[headercount++]=s;
         }
         s="";
       }
     }
  }
  if (HTTPHeaders[0].SubString(0,4).UpperCase()=="GET ")
  {
    ok=true;
    HTTPGET();
  }
  if (!ok)
  {
    send(socket, "HTTP/1.1 403 Forbidden\r\n\r\n", 23, 0);
    send(socket, "I DON'T LIKE YOU\r\n\r\n", 20, 0);
  }
  {
    BYTE i = 1;
    setsockopt(socket, AF_INET, SO_LINGER, (char*)&i, 1);
  }
  shutdown(socket,SD_SEND);
  Sleep(100);
  closesocket(socket);
  {
    int i;
    for (i=0;i<100;i++) HTTPHeaders[i]="";
  }
  WS->Clients--;
}

void __fastcall tWWWServersocket::Execute()
{
  sockaddr sa_address;
  serverSocket=socket(AF_INET,SOCK_STREAM,0);
  fillsadata(&sa_address,"0.0.0.0",port);
  if (0!=bind(serverSocket, &sa_address, sizeof(sa_address)) )
  {
    Beep();
    eof=true;
  }
  if (0!=listen(serverSocket,0) ) eof=true;
  {
    unsigned long i = 1;
    ioctlsocket(serverSocket, FIONBIO, &i);
  }
  do
  {
    tWWWClientSocket *client;
    int clientsocket;
    int sasize=sizeof(sa_address);
    clientsocket = accept(serverSocket, &sa_address, &sasize);
    if ((clientsocket>0) && (! eof))
    {
      {
        unsigned long i = 0;
        ioctlsocket(clientsocket, FIONBIO, &i);
      }
      client=new tWWWClientSocket(clientsocket, this);
      client->Resume();
    } else {
      if (clientsocket>0)
      {
        shutdown(clientsocket, SD_SEND);
        closesocket(clientsocket);
      }
    }
    Sleep(40);
  } while (! eof);
  shutdown(serverSocket, SD_SEND);
  closesocket(serverSocket);
  while (Clients!=0)
  {
    Sleep(40);
  }
  if (endEvent!=NULL) SetEvent(endEvent);
}

__fastcall tWWWServersocket::tWWWServersocket(int listenport): TThread(true)
{
    WSAData WSAD;
    eof=false;
    port=listenport;
    WSAStartup(2,&WSAD);
    Clients=0;
    Priority=tpIdle;
    endEvent=NULL;
}

#pragma package(smart_init)
