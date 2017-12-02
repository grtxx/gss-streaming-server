//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "gss_form1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

BOOL CALLBACK DSEnumCallback(LPGUID  lpGuid, LPCSTR  lpcstrDescription,
                                     LPCSTR  lpcstrModule, LPVOID  lpContext)
{
  static int i=0;
  AnsiString s;
  s=lpcstrDescription;
  s=s+" (";
  s=s+lpcstrModule+")";
  Form1->soundcard->Items->Add(s);
  Form1->sCardGUIDS[i]=lpGuid;
  i++;
  s="";
}

void __fastcall TForm1::EnumDSPs()
{
  TSearchRec sr;
  HANDLE hLib;
  int (__stdcall *DSPGetHeader)();
  winampDSPHeader *DSPH;
  AnsiString s;

  dsps->Clear();
  FindFirst("dsp\\dsp_*.dll", faAnyFile, sr);
  while (sr.Name!="")
  {
    s="dsp\\"+sr.Name;
    hLib=LoadLibrary(s.c_str());
    DSPGetHeader=GetProcAddress(hLib, "winampDSPGetHeader2");
    DSPH=(winampDSPHeader*)DSPGetHeader();
    s=DSPH->description;
    FreeLibrary(hLib);
    dsps->Items->Add(s+" ("+sr.Name+")");
    dsps2->Items->Add(s+" ("+sr.Name+")");
    sr.Name="";
    FindNext(sr);
  }
  dsps->ItemIndex=0;
  s="";
}

void __fastcall TForm1::EnumDSP2Modules()
{
  dsp2modules->Clear();
  if (dsps2->ItemIndex>=0)
  {
    HANDLE hLib;
    winampDSPHeader *DSPH;
    winampDSPModule *DSPM;
    int (__stdcall *DSPGetHeader)();
    AnsiString s=dsps2->Items->Strings[dsps2->ItemIndex];
    s=s.SubString(s.Pos("(")+1,255);
    s="dsp\\"+s.SubString(0,s.Pos(")")-1);
    hLib=LoadLibrary(s.c_str());
    DSPGetHeader=GetProcAddress(hLib, "winampDSPGetHeader2");
    DSPH=(winampDSPHeader*)DSPGetHeader();
    int i=0;
    while (DSPM=DSPH->getModule(i))
    {
           dsp2modules->Items->Add(DSPM->description);
           i++;
    }
    FreeLibrary(hLib);
    s="";
  }
}

void __fastcall TForm1::EnumDSPModules()
{
  dspmodules->Clear();
  if (dsps->ItemIndex>=0)
  {
    HANDLE hLib;
    winampDSPHeader *DSPH;
    winampDSPModule *DSPM;
    int (__stdcall *DSPGetHeader)();
    AnsiString s=dsps->Items->Strings[dsps->ItemIndex];
    s=s.SubString(s.Pos("(")+1,255);
    s="dsp\\"+s.SubString(0,s.Pos(")")-1);
    hLib=LoadLibrary(s.c_str());
    DSPGetHeader=GetProcAddress(hLib, "winampDSPGetHeader2");
    DSPH=(winampDSPHeader*)DSPGetHeader();
    int i=0;
    while (DSPM=DSPH->getModule(i))
    {
           dspmodules->Items->Add(DSPM->description);
           i++;
    }
    FreeLibrary(hLib);
    s="";
  }
}

void __fastcall TForm1::FormCreate(TObject *Sender)
{
  CoInitialize(NULL);
  VU=new VUMeter(this);
  InsertControl(VU);
  VU->Left=ClientWidth-90;
  VU->Width=90;
  VU->Top=0;
  VU->Height=StatusBar1->Top;
  VU->logaritmic=30;
  VU->showkind=1;
  DirectSoundCaptureEnumerate((LPDSENUMCALLBACK)(&DSEnumCallback), NULL);
  soundcard->ItemIndex=0;
  RT=NULL;
  WF=NULL;
  WF2=NULL;
  ET=NULL;
  WS=NULL;
  EnumDSPs();
  EnumDSPModules();
  endEvent=CreateEvent(NULL,true,false,NULL);

  paramset=0;

  for (int i=1;i<ParamCount();i++)
  {
        if (ParamStr(i).SubString(1,9)=="paramset=") paramset=ParamStr(i).SubString(10,15).ToInt();
  }

  LoadSettings();

  for (int i=1;i<=ParamCount();i++)
  {
        if (ParamStr(i)=="-auto") Button7Click(NULL);
        if (ParamStr(i)=="-dsp") Button5Click(NULL);
        if (ParamStr(i)=="-dsp2" || ParamStr(2)=="-dsp2") Button2Click(NULL);
        if (ParamStr(i)=="-h" || ParamStr(i)=="/?") MessageBox(Handle, "paramset=szám: Használja a megadott paraméterkészletet (így több gss futtatható ugyanazon a gépen)\n\r-auto: Automatikus indulás\n\r-dsp: DSP effekt bekapcsolása (auto módban)\n\r-dsp2: DSP2 effekt bekapcsolása (auto módban)","GSS",64);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
  if (WF!=NULL) WF->DSPconfig();
}

//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
  if (dspmodules->ItemIndex>=0 && WF!=NULL)
  {
    WF->DSPquit();
    AnsiString s=dsps->Items->Strings[dsps->ItemIndex];
    s=s.SubString(s.Pos("(")+1,255);
    s="dsp\\"+s.SubString(0,s.Pos(")")-1);

    WF->DSPinit(s.c_str());
    if (WF->DSPinitModule(dspmodules->ItemIndex, Handle)==-1)
    {
      WF->DSPconfig();
    }
    s="";
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button7Click(TObject *Sender)
{
  GUID *Guid;
  if (DSCap==NULL)
  {
    Guid=sCardGUIDS[soundcard->ItemIndex];
    if (Guid==NULL) Guid=(_GUID*)&DSDEVID_DefaultCapture;
    StatusBar1->Panels->Items[0]->Text=soundcard->Items->Strings[soundcard->ItemIndex];
    if SUCCEEDED( DirectSoundCaptureCreate(Guid, &DSCap, NULL) )
    {
      if (RT==NULL)
      {
        RT=new tRecordThread(DSCap, khz->Items->Strings[khz->ItemIndex].ToInt(), channels->ItemIndex+1, 16);
        RT->Priority=tpTimeCritical;
        RT->endEvent=endEvent;
      }
      if (WF==NULL)
      {
        WF=new tWFXThread(RT->cBuffer);
        WF->channels=channels->ItemIndex+1;
        WF->srate=khz->Items->Strings[khz->ItemIndex].ToInt();
        WF->endEvent=endEvent;
      }
      if (WF2==NULL)
      {
        WF2=new tWFXThread(WF->fxoutBuffer);
        WF2->channels=channels->ItemIndex+1;
        WF2->srate=khz->Items->Strings[khz->ItemIndex].ToInt();
        WF2->endEvent=endEvent;
      }
      if (ET==NULL)
      {
        ET=new tEncoderThread(WF2->fxoutBuffer);
        ET->initEncoder(ENCODER_MP3,
                        khz->Items->Strings[khz->ItemIndex].ToInt(),
                        channels->ItemIndex+1,
                        bitrate->Items->Strings[bitrate->ItemIndex].ToInt(), "");
        ET->endEvent=endEvent;
        if (WS!=NULL) WS->inBuffer=ET->bitBuffer;
      }
      RT->Resume();
      WF->Resume();
      WF2->Resume();
      ET->Resume();
    } else {
      MessageBoxA(Handle,"Hiba","Nincs DSCap",16);
    }
  }

  if (WS==NULL)
  {
    WS=new tWWWServersocket(ud_port->Position);
    WS->endEvent=endEvent;
    if (ET!=NULL) WS->inBuffer=ET->bitBuffer;

    TD=new cTitleDownloader();
    TD->titleURL=mdconnector->Text;
    TD->interval=ud_mdinterval->Position;
    TD->titleSubsts=titleSubsts->Text+";";
    TD->Resume();
  }
  WS->StreamName=streamname->Text;
  WS->StreamURL=streamurl->Text;
  WS->StreamGenre=streamgenre->Text;
  WS->StreamPublic=streampublic->Checked;
  WS->StreamBitRate=bitrate->Items->Strings[bitrate->ItemIndex].ToInt();
  WS->Titles=TD;
  WS->Resume();  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::voltimerTimer(TObject *Sender)
{
  int lv, rv;
  if (RT!=NULL)
  {
    lv=(VU->pgl<RT->leftvol  ? RT->leftvol  : VU->pgl-500);
    rv=(VU->pgr<RT->rightvol ? RT->rightvol : VU->pgr-500);
    VU->SetLEDs(lv, rv);
  } else {
    VU->SetLEDs(0, 0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Kilps1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
  StatusBar1->Panels->Items[0]->Text="WS"; StatusBar1->Refresh();
  if (WS!=NULL)
  {
    WS->eof=true;
    WaitForSingleObject(endEvent,10000);  ResetEvent(endEvent);
    WS=NULL;
    TD->eof=true;
    TD=NULL;
  }
  StatusBar1->Panels->Items[0]->Text="ET"; StatusBar1->Refresh();
  if (ET!=NULL)
  {
    ET->eof=true;
    WaitForSingleObject(endEvent,10000);  ResetEvent(endEvent);
    ET->Free();
    ET=NULL;
  }
  StatusBar1->Panels->Items[0]->Text="WF2"; StatusBar1->Refresh();
  if (WF2!=NULL)
  {
    WF2->DSPquit();
    WF2->eof=true;
    WaitForSingleObject(endEvent,10000);  ResetEvent(endEvent);
    WF2->Free();
    WF2=NULL;
  }
  StatusBar1->Panels->Items[0]->Text="WF"; StatusBar1->Refresh();
  if (WF!=NULL)
  {
    WF->DSPquit();
    WF->eof=true;
    WaitForSingleObject(endEvent,10000);  ResetEvent(endEvent);
    WF->Free();
    WF=NULL;
  }

  StatusBar1->Panels->Items[0]->Text="RT"; StatusBar1->Refresh();
  if (RT!=NULL)
  {
    RT->eof=true;
    WaitForSingleObject(endEvent,10000);  ResetEvent(endEvent);
    RT->Free();
    RT=NULL;
    DSCap->Release();
    DSCap=NULL;
  }
  StatusBar1->Panels->Items[0]->Text="";
}
//---------------------------------------------------------------------------



void __fastcall TForm1::CheckBox1Click(TObject *Sender)
{
  voltimer->Enabled=CheckBox1->Checked;
  if (! voltimer->Enabled)
  {
    VU->SetLEDs(0, 0);
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::dspsChange(TObject *Sender)
{
  EnumDSPModules();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::statustimerTimer(TObject *Sender)
{
  if (WS!=NULL)
  {
    StatusBar1->Panels->Items[2]->Text="CLIENTS:"+IntToStr(WS->Clients);
    StatusBar1->Panels->Items[1]->Text=TD->statusText+": "+TD->currentTitle;
    StaticText1->Color=(StaticText1->Color==0x0000f0 ? 0x000000 : 0x0000f0);
  } else {
    StatusBar1->Panels->Items[2]->Text="";
    StaticText1->Color=0;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
  if (WF!=NULL)  WF->DSPquit();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSettings()
{
  TRegistry *R;
  R=new TRegistry(KEY_READ);
  try
  {
    if ( R->KeyExists("\\Software\\GRT\\GSS\\ParamSet"+IntToStr(paramset)) )
    {
      R->OpenKey("\\Software\\GRT\\GSS\\ParamSet"+IntToStr(paramset), true);
      Left                    = R->ReadInteger("Left");
      Top                     = R->ReadInteger("Top");
      soundcard->ItemIndex    = R->ReadInteger("SoundCard");
      khz->ItemIndex          = R->ReadInteger("SampleRate");
      channels->ItemIndex     = R->ReadInteger("Channels");
      dsps->ItemIndex         = R->ReadInteger("CurrentDSP");
      EnumDSPModules();
      dspmodules->ItemIndex   = R->ReadInteger("CurrentDSPModule");
      dsps2->ItemIndex        = R->ReadInteger("CurrentDSP2");
      EnumDSP2Modules();
      dsp2modules->ItemIndex  = R->ReadInteger("CurrentDSP2Module");
      bitrate->ItemIndex      = R->ReadInteger("Mp3BitRate");
      mdconnector->Text       = R->ReadString ("MetaData_url");
      ud_mdinterval->Position = R->ReadInteger("MetaData_interval");
      streamname->Text        = R->ReadString ("MetaData_streamname");
      streamurl->Text         = R->ReadString ("MetaData_streamurl");
      streamgenre->Text       = R->ReadString ("MetaData_streamgenre");
      streampublic->Checked   = R->ReadBool   ("MetaData_streampublic");
      titleSubsts->Text       = R->ReadString ("MetaData_titleSubsts");
      ud_port->Position       = R->ReadInteger("Server_port");
    }
  }
  __finally
  {
    delete R;
  }
}

void __fastcall TForm1::savesettingsClick(TObject *Sender)
{
  TRegistry *R;
  R=new TRegistry(KEY_WRITE);
  try
  {
    R->OpenKey("\\Software\\GRT\\GSS\\ParamSet"+IntToStr(paramset), true);
    R->WriteInteger("Left", Left);
    R->WriteInteger("Top", Top);
    R->WriteInteger("SoundCard", soundcard->ItemIndex);
    R->WriteInteger("SampleRate", khz->ItemIndex);
    R->WriteInteger("Channels", channels->ItemIndex);
    R->WriteInteger("CurrentDSP", dsps->ItemIndex);
    R->WriteInteger("CurrentDSPModule", dspmodules->ItemIndex);
    R->WriteInteger("CurrentDSP2", dsps2->ItemIndex);
    R->WriteInteger("CurrentDSP2Module", dsp2modules->ItemIndex);
    R->WriteInteger("Mp3BitRate", bitrate->ItemIndex);
    R->WriteString ("MetaData_url", mdconnector->Text);
    R->WriteInteger("MetaData_interval", ud_mdinterval->Position);
    R->WriteString ("MetaData_streamname", streamname->Text);
    R->WriteString ("MetaData_streamurl", streamurl->Text);
    R->WriteString ("MetaData_streamgenre", streamgenre->Text);
    R->WriteString ("MetaData_titleSubsts", titleSubsts->Text);
    R->WriteBool   ("MetaData_streampublic", streampublic->Checked);
    R->WriteInteger("Server_port", ud_port->Position);        
  }
  __finally
  {
    delete R;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::mdconnectorChange(TObject *Sender)
{
  if (TD!=NULL) TD->titleURL=mdconnector->Text;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::dsps2Change(TObject *Sender)
{
  EnumDSP2Modules();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{
  if (dsp2modules->ItemIndex>=0 && WF2!=NULL)
  {
    WF2->DSPquit();
    AnsiString s=dsps2->Items->Strings[dsps2->ItemIndex];
    s=s.SubString(s.Pos("(")+1,255);
    s="dsp\\"+s.SubString(0,s.Pos(")")-1);

    WF2->DSPinit(s.c_str());
    if (WF2->DSPinitModule(dsp2modules->ItemIndex, Handle)==-1)
    {
      WF2->DSPconfig();
    }
    s="";
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  if (WF2!=NULL)  WF2->DSPquit();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
  if (WF2!=NULL) WF2->DSPconfig();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  Button1Click(NULL);
}
//---------------------------------------------------------------------------

