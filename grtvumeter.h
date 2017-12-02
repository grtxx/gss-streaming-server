//---------------------------------------------------------------------------

#ifndef grtvumeterH
#define grtvumeterH

#include "controls.hpp"
#include "graphics.hpp"


class VUMeter : public TWinControl
{
public:
        TControlCanvas *Canvas;
        int pgl, pgr, opgl, opgr;
        int imax;
        int showkind;
        int logaritmic;
        float uspeed, dspeed;
        double leftdata, rightdata;
        int dbstep;
        int mindb;
        int bitpersample;
        int lpeak, rpeak;
        int peaktime;
        bool horizontal;
        __fastcall VUMeter(TComponent *ow);
        __fastcall Refresh();
        __fastcall SetLEDs(int left, int right);
        __fastcall SetDATA(int left, int right);
        __fastcall Free();
        void __fastcall WndProc(TMessage &Message);
};

//---------------------------------------------------------------------------
#endif
