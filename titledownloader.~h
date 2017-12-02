//---------------------------------------------------------------------------

#ifndef titledownloaderH
#define titledownloaderH
//---------------------------------------------------------------------------

#include <gsockets.h>
#include <classes.hpp>
#include <stdio.h>

class cTitleDownloader : public TThread
{
public:
        AnsiString titleURL;
        AnsiString currentTitle;
        AnsiString statusText;
        AnsiString titleSubsts;
        int interval;
        long statusCode;
        bool eof;
        __fastcall cTitleDownloader();
        void __fastcall Execute();
};

#endif
