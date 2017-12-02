//---------------------------------------------------------------------------


#pragma hdrstop

#include "grtvumeter.h"
#include <math.h>

//---------------------------------------------------------------------------

__fastcall VUMeter::VUMeter(TComponent *ow) : TWinControl(ow)
{
        TWinControl::TWinControl(ow);
        Canvas=new TControlCanvas();
        Canvas->Control=this;
        imax=0;
        showkind=0;
        logaritmic=0;
        uspeed=0.6;
        dspeed=0.4;
        mindb=-60;
        dbstep=3;
        bitpersample=16;
        lpeak=0;
        rpeak=0;
        peaktime=40;
}

__fastcall VUMeter::Refresh()
{
        int i,j;
        int barwidth=0;
        Graphics::TBitmap *B;
        B=new Graphics::TBitmap();
        B->Width=Width;
        B->Height=Height;

        B->Canvas->Pen->Color=(TColor)0x000000;
        B->Canvas->Brush->Color=(TColor)0x000000;
        B->Canvas->Rectangle(0,0,B->Width,B->Height);

        imax=B->Height-18;
        opgl=floor(pgl*imax/(1 << (bitpersample-1)));
        opgr=floor(pgr*imax/(1 << (bitpersample-1)));
        switch (showkind)
        {
          case 0: barwidth=B->Width/2-5; break;
          case 1: barwidth=B->Width/2-34; break;
          case 2: barwidth=B->Width/2-8; break;
        }
        if (barwidth<2) barwidth=2;
        for (i=imax;i>=0;i=i-2)
        {
                TColor ncolor;
                TColor ucolor;
                ncolor=(TColor)0x003300;
                ucolor=(TColor)0x00ff00;
                if (i*100/imax>50) { ncolor=(TColor)0x003333; ucolor=(TColor)0x00ffff; }
                if (i*100/imax>90) { ncolor=(TColor)0x000033; ucolor=(TColor)0x0000ff; }

                B->Canvas->Pen->Color=((opgl>i) ? ucolor : ncolor );
                B->Canvas->MoveTo(3,B->Height-3-i);
                B->Canvas->LineTo(3+barwidth,B->Height-3-i);

                B->Canvas->Pen->Color=((opgr>i) ? ucolor : ncolor );
                B->Canvas->MoveTo(B->Width-3-barwidth,B->Height-3-i);
                B->Canvas->LineTo(B->Width-3,B->Height-3-i);
        }
        B->Canvas->Brush->Style=bsSolid;
        if (abs(pgl)>32760) lpeak=peaktime;
        B->Canvas->Brush->Color=((lpeak>0) ? 0x0000ff : 0x000033);
        lpeak=(lpeak>0 ? lpeak-1 : 0);
        B->Canvas->Pen->Color=B->Canvas->Brush->Color;
        B->Canvas->Rectangle(3,3,barwidth+3, B->Height-3-imax-2);

        B->Canvas->Brush->Style=bsSolid;
        if (abs(pgr)>32760) rpeak=peaktime;
        B->Canvas->Brush->Color=((rpeak>0) ? 0x0000ff : 0x000033);
        rpeak=(rpeak>0 ? rpeak-1 : 0);
        B->Canvas->Pen->Color=B->Canvas->Brush->Color;
        B->Canvas->Rectangle(B->Width-3-barwidth,3,B->Width-3, B->Height-3-imax-2);

        switch (showkind)
        {
          case 0:
                break;
          case 1:
                {
                        double f;
                        AnsiString s;
                        int lpos = -100000;
                        for (i=0;i>=mindb;i-=dbstep)
                        {
                                f=i;
                                f=exp(f/10)*(1 << (bitpersample-1));
                                if (logaritmic>0)
                                {
                                        f=f/(1 << (bitpersample-1))*logaritmic;
                                        f=(log10(f+1))/log10(logaritmic+1)*(1 << (bitpersample-1));
                                }
                                f=B->Height-6-f/(1 << (bitpersample-1))*imax;
                                if (lpos+B->Canvas->TextHeight(s)<f+3-B->Canvas->TextHeight(s)/2)
                                {
                                        lpos=f+3-B->Canvas->TextHeight(s)/2;
                                        B->Canvas->Pen->Color=0xffffff;
                                        B->Canvas->MoveTo(21,f+3);          B->Canvas->LineTo(25,f+3);
                                        B->Canvas->MoveTo(B->Width-21,f+3); B->Canvas->LineTo(B->Width-26,f+3);
                                        B->Canvas->Font->Color=0x00a0ff;
                                        B->Canvas->Brush->Style=bsClear;
                                        s=IntToStr(i);
                                        B->Canvas->TextOutA(B->Width/2-B->Canvas->TextWidth(s)/2,f+3-B->Canvas->TextHeight(s)/2,s);
                                }
                        }
                        s="";
                }
                break;
          case 2:
                {
                        double f;
                        int lpos = -100000;
                        for (i=0;i>=mindb;i-=dbstep)
                        {
                                f=i;
                                f=exp(f/10)*(1 << (bitpersample-1));
                                if (logaritmic>0)
                                {
                                        f=f/(1 << (bitpersample-1))*logaritmic;
                                        f=(log10(f+1))/log10(logaritmic+1)*(1 << (bitpersample-1));
                                }
                                f=B->Height-6-f/(1 << (bitpersample-1))*imax;
                                B->Canvas->Pen->Color=0xffffff;
                                B->Canvas->MoveTo(B->Width/2-1,f+3);
                                B->Canvas->LineTo(B->Width/2+1,f+3);
                        }
                }
                break;
        }

        Canvas->Draw(0, 0, B);
        B->Free();
}

__fastcall VUMeter::SetLEDs(int left, int right)
{
        leftdata=left;
        rightdata=right;
        if ((left*imax/(1 << (bitpersample-1)))!=opgl || (right*imax/(1 << (bitpersample-1)))!=opgr || imax==0)
        {
                pgl=left;
                pgr=right;
                Refresh();
        }
}

__fastcall VUMeter::SetDATA(int left, int right)
{
        leftdata=left;
        rightdata=right;
        if (logaritmic==0)
        {
          pgl=left;
          pgr=right;
        } else {

                double f1;
                if (left!=0)
                {
                        f1=left;  f1=f1/(1 << (bitpersample-1))*logaritmic;
                        f1=(f1>0 ? (log10(f1+1))/log10(logaritmic+1)*(1 << (bitpersample-1)) : 0);
                        pgl=f1;
                } else {
                        pgl=left;
                }
                if (right!=0)
                {
                        f1=right; f1=f1/(1 << (bitpersample-1))*logaritmic;
                        f1=(f1>0 ? (log10(f1+1))/log10(logaritmic+1)*(1 << (bitpersample-1)) : 0);
                        pgr=f1;
                } else {
                        pgr=right;
                }
        }
        Refresh();
}

__fastcall VUMeter::Free()
{
        Canvas->Free();
        TWinControl::Free();
}

void __fastcall VUMeter::WndProc(TMessage &Message)
{
        if (Message.Msg==WM_PAINT) Refresh();
        TWinControl::WndProc(Message);

}

#pragma package(smart_init)

