
//---------------------------------------------------------------------------

#ifndef gss_form1H
#define gss_form1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <io.h>

#include <mmsystem.h>
#include "c:/DXSDK/Include/dsound.h"

#include <ds1_recordingthread.h>
#include <ds1_encoderthread.h>
#include <ds1_decoderthread.h>
#include <ds1_playingthread.h>
#include <wfx.h>
#include <wwwserver.h>
#include <grtvumeter.h>
#include <titledownloader.h>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Registry.hpp>

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TButton *Button7;
        TTimer *voltimer;
        TButton *Button1;
        TStatusBar *StatusBar1;
        TMainMenu *MainMenu1;
        TMenuItem *Fjl1;
        TMenuItem *Kilps1;
        TMenuItem *savesettings;
        TCheckBox *CheckBox1;
        TTimer *statustimer;
        TPageControl *controls;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TTabSheet *TabSheet4;
        TTabSheet *TabSheet5;
        TLabel *Label10;
        TEdit *port;
        TUpDown *ud_port;
        TLabel *Label6;
        TEdit *mdconnector;
        TLabel *Label7;
        TEdit *mdinterval;
        TUpDown *ud_mdinterval;
        TLabel *Label8;
        TLabel *Label11;
        TEdit *streamname;
        TLabel *Label12;
        TEdit *streamurl;
        TCheckBox *streampublic;
        TLabel *Label4;
        TComboBox *bitrate;
        TLabel *Label5;
        TListBox *dspmodules;
        TButton *Button5;
        TButton *Button6;
        TButton *Button3;
        TLabel *Label1;
        TComboBox *soundcard;
        TLabel *Label2;
        TComboBox *khz;
        TLabel *Label3;
        TComboBox *channels;
        TLabel *Label13;
        TEdit *streamgenre;
        TComboBox *dsps;
        TStaticText *StaticText1;
        TTabSheet *TabSheet6;
        TMemo *Memo1;
        TLabel *Label9;
        TEdit *titleSubsts;
        TTabSheet *TabSheet7;
        TComboBox *dsps2;
        TListBox *dsp2modules;
        TButton *Button2;
        TButton *Button4;
        TButton *Button8;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall voltimerTimer(TObject *Sender);
        void __fastcall Kilps1Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall dspsChange(TObject *Sender);
        void __fastcall statustimerTimer(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall savesettingsClick(TObject *Sender);
        void __fastcall mdconnectorChange(TObject *Sender);
        void __fastcall dsps2Change(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations

        IDirectSoundCapture *DSCap;
        tRecordThread *RT;
        tEncoderThread *ET;

        IDirectSound *DSPlay;
        tBufferPlayThread *PT;
        tDecoderThread *DT;
        tWFXThread *WF;
        tWFXThread *WF2;        
        tWWWServersocket *WS;
        cTitleDownloader *TD;

        tCircularBuffer *cBuffer;
        tCircularBuffer *fxoutBuffer;
        HANDLE endEvent;

        VUMeter *VU;

        LPGUID sCardGUIDS[20];

        int paramset;

        __fastcall TForm1(TComponent* Owner);
        void __fastcall EnumDSPs();
        void __fastcall EnumDSPModules();
        void __fastcall EnumDSP2Modules();        
        void __fastcall LoadSettings();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
