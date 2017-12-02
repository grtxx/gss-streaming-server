//---------------------------------------------------------------------------

#ifndef gsocketsH
#define gsocketsH

#include "winsock2.h"
#include "sysutils.hpp"

void fillsadata(sockaddr *sa, char ipaddr[15], int port);
void fillsadata(sockaddr *sa, AnsiString ipaddr, int port);
void getsadata(sockaddr sa, AnsiString &ipaddr, int &port);
AnsiString getHostIP(AnsiString host);
int sendStringLn(int socket, AnsiString s);
int sendStringw32Ln(int socket, AnsiString s);
AnsiString socketReadLn(int socket);

//---------------------------------------------------------------------------
#endif
 