//---------------------------------------------------------------------------


#pragma hdrstop

#include "gsockets.h"

AnsiString getHostIP(AnsiString host)
{
  hostent *he = gethostbyname(host.c_str());
  AnsiString x;
  if (he!=NULL)
  {
    char *chrs=he->h_addr_list[0];
	x = IntToStr( (unsigned char)(chrs[0]) )+"."+
		IntToStr( (unsigned char)(chrs[1]) )+"."+
		IntToStr( (unsigned char)(chrs[2]) )+"."+
		IntToStr( (unsigned char)(chrs[3]) );
  } else {
    x="";
  }
  return x;
}

void fillsadata(sockaddr *sa, AnsiString ipaddr, int port)
{
  ZeroMemory(sa,sizeof(sa));
  sa->sa_family=AF_INET;
  sa->sa_data[0]=port / 256;
  sa->sa_data[1]=port % 256;
  {
	int i;
	int ip[5];
	int cnt=4;
	int multi=1;
	ZeroMemory(ip,sizeof(ip));
	for (i=ipaddr.Length();i>=1;i--)
	{
	  if (46==ipaddr[i])
	  {
		multi=1;
		cnt--;
	  } else {
		ip[cnt]+=multi*((int)(ipaddr[i])-48);
		multi*=10;
	  }
	}
	for (i=1;i<5;i++) sa->sa_data[1+i]=ip[i];
  }
}

void fillsadata(sockaddr *sa, char ipaddr[15], int port)
{
  ZeroMemory(sa,sizeof(sa));
  sa->sa_family=AF_INET;
  sa->sa_data[0]=port / 256;
  sa->sa_data[1]=port % 256;
  {
    int i;
    int ip[5];
    int cnt=4;
    int multi=1;
    ZeroMemory(ip,sizeof(ip));
    for (i=strlen(ipaddr)-1;i>=0;i--)
    {
      if (46==ipaddr[i])
      {
        multi=1;
        cnt--;
      } else {
        ip[cnt]+=multi*((int)(ipaddr[i])-48);
        multi*=10;
      }
    }
    for (i=1;i<5;i++) sa->sa_data[1+i]=ip[i];
  }
}

void getsadata(sockaddr sa, AnsiString &ipaddr, int &port)
{
  sa.sa_family=AF_INET;
  port=sa.sa_data[0]*256+sa.sa_data[1];
  ipaddr="";
  ipaddr=IntToStr(sa.sa_data[2])+"."+IntToStr(sa.sa_data[3])+"."+IntToStr(sa.sa_data[4])+"."+IntToStr(sa.sa_data[5]);
}

AnsiString socketReadLn(int socket)
{
  AnsiString s="";
  char c;
  int cnt;
  do
  {
    cnt=recv(socket, &c, 1, 0);
    if (cnt==1 && c!=10 && c!=13) s=s+c;
  } while (c!=10 && c!=13 && cnt==1);
  if (c==13) recv(socket, &c, 1, 0);
  return s;
}

int sendStringLn(int socket, AnsiString s)
{
  s=s+"\n";
  return send(socket, s.c_str(), s.Length(), 0);
}

int sendStringw32Ln(int socket, AnsiString s)
{
  s=s+"\r\n";
  return send(socket, s.c_str(), s.Length(), 0);
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
