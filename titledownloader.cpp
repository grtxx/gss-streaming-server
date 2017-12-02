//---------------------------------------------------------------------------


#pragma hdrstop

#include "titledownloader.h"

//---------------------------------------------------------------------------

__fastcall cTitleDownloader::cTitleDownloader(): TThread(true)
{
        Priority=tpLowest;
        FreeOnTerminate=true;
        interval=60;
        titleURL="";
        eof=false;
}

void __fastcall cTitleDownloader::Execute()
{
        AnsiString s;
        AnsiString hostname;
        AnsiString uri;
        AnsiString port;
        AnsiString acText;
        sockaddr sadata;
        int sock;
        int currentline = 0;
        WSAData WSAD;
        WSAStartup(2,&WSAD);
        
        while (! eof)
        {
           statusText="";
           if (titleURL!="")
           {
             try
             {
               if (titleURL.SubString(0,7)=="http://")
               {
                 s=titleURL.SubString(8,255);
                 hostname=s.SubString(0,s.Pos("/")-1);
                 port="80";
                 if (hostname.Pos(":")!=0)
                 {
                   port=hostname.SubString(hostname.Pos(":")+1,255);
                   hostname=s.SubString(0,hostname.Pos(":")-1);
                 }
                 uri=s.SubString(s.Pos("/"),255);

                 statusText="HTTP Névfeloldás";
                 fillsadata(&sadata, getHostIP(hostname).c_str(), port.ToInt());
                 sock=socket(AF_INET, SOCK_STREAM, 0);
                 {
                   int i=10000;
                   setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&i, sizeof(int));
                 }
                 statusText="HTTP Kapcsolódás";
                 if ( 0<=connect(sock, &sadata, sizeof(sadata)) )
                 {
                   statusText="HTTP Letöltés";
                   sendStringLn(sock,"GET "+uri+" HTTP/1.1");
                   sendStringLn(sock,"Host: "+hostname);
                   sendStringLn(sock,"");
                   statusText=socketReadLn(sock);
                   if (statusText.SubString(0,4)=="HTTP")
                   {
                     s=statusText.SubString(statusText.Pos(" ")+1,255);
                     s=s.SubString(0,s.Pos(" ")-1);
                     statusCode=s.ToInt();
                     do
                     {
                       s=socketReadLn(sock);
                     } while (s!="");
                     acText=socketReadLn(sock);
                   } else {
                     statusCode=-2;
                   }
                 } else {
                   statusCode=-1;
                   statusText="Kapcsolathívási hiba";
                 }
               }
               if (titleURL.SubString(0,7)=="text://")
               {
                 statusText="OK";
                 acText=titleURL.SubString(8,255);
                 statusCode=200;
               }
               if (titleURL.SubString(0,7)=="file://")
               {
                 FILE *f;
                 AnsiString text = titleURL.SubString(8,255);
                 f=fopen(text.c_str(),"rb");
                 if (f!=NULL)
                 {
                   char c = 255;
                   text="";
                   int aline = 0;
                   while (! feof(f) && aline<=currentline)
                   {
                     if (fread(&c,1,1,f)==1)
                     {
                       if ((c!=13) && (c!=10) && (c!=0))
                       {
                         if (aline==currentline) text=text+c;
                       } else {
                         if (c==10) aline++;
                       }
                     }
                   }
                   currentline++;
                   if (feof(f)) currentline=0;
                   fclose(f);
                   if (text!="")
                   {
                     acText=text;
                   }
                   text="";
                   statusText="OK";
                   statusCode=200;
                 } else {
                   statusText="Nem megnyitható a fájl";
                   statusCode=404;
                 }
               }
             }
             __finally
             {
               closesocket(sock);
             }
           } else {
             statusText="Nincs cím URL";
             currentTitle="";
             statusCode=0;
           }
           if (statusCode==200)
           {
             int i;
             int m=0;
             AnsiString mit="";
             AnsiString mire="";
             for (i=1;i<=titleSubsts.Length();i++)
             {
               if (titleSubsts.SubString(i,1)==",") m=1;
               if (titleSubsts.SubString(i,1)==";")
               {
                 if (mit!="")
                 {
                   if (acText.Pos(mit)!=0)
                   {
                     acText=acText.SubString(0,acText.Pos(mit)-1)+mire+acText.SubString(acText.Pos(mit)+1+mit.Length(),500);
                   }
                 }
                 mit="";
                 mire="";
                 m=0;
               }
               if (titleSubsts.SubString(i,1)!=";" && titleSubsts.SubString(i,1)!=",")
               {
                 switch (m)
                 {
                   case 0:
                      mit+=titleSubsts.SubString(i,1);
                      break;
                   case 1:
                      mire+=titleSubsts.SubString(i,1);
                      break;
                 }
               }
             }
             mit="";
             mire="";


             currentTitle=acText;
           } else {
             currentTitle="";
           }
           Sleep(interval*1000);
        }
}

#pragma package(smart_init)
