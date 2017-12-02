//---------------------------------------------------------------------------

#ifndef wfxH
#define wfxH

#include "circbuffer.h"
#include <windows.h>
#include <classes.hpp>

typedef struct winampDSPModule {
  char *description;		// description
  HWND hwndParent;		// parent window (filled in by calling app)
  HINSTANCE hDllInstance;	// instance handle to this DLL (filled in by calling app)
  void (*Config)(struct winampDSPModule *this_mod);
  int (*Init)(struct winampDSPModule *this_mod);
  int (*ModifySamples)(struct winampDSPModule *this_mod, short int *samples, int numsamples, int bps, int nch, int srate);
  void (*Quit)(struct winampDSPModule *this_mod);
  void *userData;
} winampDSPModule;

typedef struct {
  int version;       // DSP_HDRVER
  char *description; // description of library
  winampDSPModule* (*getModule)(int);	// module retrieval function
} winampDSPHeader;

// exported symbols
typedef winampDSPHeader* (*winampDSPGetHeaderType)();

// header version: 0x20 == 0.20 == winamp 2.0
#define DSP_HDRVER 0x20


class tWFXThread : public TThread
{
private:
        HANDLE inbuffer_event;
        HANDLE outbuffer_event;
public:
        bool eof;
        tCircularBuffer *fxinBuffer;
        tCircularBuffer *fxoutBuffer;
        HANDLE dspLib;
        winampDSPHeader *DSPH;
        winampDSPModule *DSPM;
        int channels;
        int srate;
        HANDLE DSPSemafor;
        HANDLE endEvent;
        __fastcall tWFXThread(tCircularBuffer *pbuffer);
        __fastcall ~tWFXThread();
        int __fastcall DSPinit(AnsiString dllname);
        int __fastcall DSPinitModule(int modulenumber, HANDLE aHandle);
        int __fastcall DSPconfig();
        int __fastcall DSPquit();
        void __fastcall Execute();
};

//---------------------------------------------------------------------------
#endif
