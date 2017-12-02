object Form1: TForm1
  Left = 259
  Top = 258
  BorderStyle = bsSingle
  Caption = 'GRT'#39's Streaming Server'
  ClientHeight = 290
  ClientWidth = 569
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Button7: TButton
    Left = 8
    Top = 248
    Width = 73
    Height = 21
    Caption = 'Start'
    TabOrder = 0
    OnClick = Button7Click
  end
  object Button1: TButton
    Left = 88
    Top = 248
    Width = 73
    Height = 21
    Caption = 'Stop'
    TabOrder = 1
    OnClick = Button1Click
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 271
    Width = 569
    Height = 19
    Panels = <
      item
        Width = 120
      end
      item
        Width = 250
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object CheckBox1: TCheckBox
    Left = 456
    Top = 250
    Width = 17
    Height = 17
    TabOrder = 3
    OnClick = CheckBox1Click
  end
  object controls: TPageControl
    Left = 8
    Top = 8
    Width = 465
    Height = 233
    ActivePage = TabSheet6
    MultiLine = True
    TabIndex = 6
    TabOrder = 4
    object TabSheet1: TTabSheet
      Caption = 'Hangk'#225'rtya'
      object Label1: TLabel
        Left = 8
        Top = 20
        Width = 65
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Hangk'#225'rtya:'
      end
      object Label2: TLabel
        Left = 8
        Top = 44
        Width = 65
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Sample rate:'
      end
      object Label3: TLabel
        Left = 8
        Top = 68
        Width = 65
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Csatorn'#225'k:'
      end
      object soundcard: TComboBox
        Left = 80
        Top = 16
        Width = 369
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
      end
      object khz: TComboBox
        Left = 80
        Top = 40
        Width = 105
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 1
        Text = '44100'
        Items.Strings = (
          '32000'
          '44100'
          '48000')
      end
      object channels: TComboBox
        Left = 80
        Top = 64
        Width = 105
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 1
        TabOrder = 2
        Text = 'Stereo'
        Items.Strings = (
          'Mono'
          'Stereo')
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'DSP'
      ImageIndex = 1
      object dspmodules: TListBox
        Left = 8
        Top = 112
        Width = 353
        Height = 73
        ItemHeight = 13
        TabOrder = 0
      end
      object Button5: TButton
        Left = 368
        Top = 112
        Width = 81
        Height = 21
        Caption = 'DSP Start'
        TabOrder = 1
        OnClick = Button5Click
      end
      object Button6: TButton
        Left = 368
        Top = 136
        Width = 81
        Height = 21
        Caption = 'DSP Stop'
        TabOrder = 2
        OnClick = Button6Click
      end
      object Button3: TButton
        Left = 368
        Top = 160
        Width = 81
        Height = 21
        Caption = 'DSP be'#225'll'#237't'#225'sa'
        TabOrder = 3
        OnClick = Button3Click
      end
      object dsps: TComboBox
        Left = 8
        Top = 8
        Width = 441
        Height = 97
        Style = csSimple
        ItemHeight = 13
        TabOrder = 4
        Text = 'dsps'
        OnChange = dspsChange
      end
    end
    object TabSheet7: TTabSheet
      Caption = 'DSP2'
      ImageIndex = 6
      object dsps2: TComboBox
        Left = 8
        Top = 8
        Width = 441
        Height = 97
        Style = csSimple
        ItemHeight = 13
        TabOrder = 0
        OnChange = dsps2Change
      end
      object dsp2modules: TListBox
        Left = 8
        Top = 112
        Width = 353
        Height = 73
        ItemHeight = 13
        TabOrder = 1
      end
      object Button2: TButton
        Left = 368
        Top = 112
        Width = 81
        Height = 21
        Caption = 'DSP Start'
        TabOrder = 2
        OnClick = Button2Click
      end
      object Button4: TButton
        Left = 368
        Top = 136
        Width = 81
        Height = 21
        Caption = 'DSP Stop'
        TabOrder = 3
        OnClick = Button4Click
      end
      object Button8: TButton
        Left = 368
        Top = 160
        Width = 81
        Height = 21
        Caption = 'DSP be'#225'll'#237't'#225'sa'
        TabOrder = 4
        OnClick = Button8Click
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'MP3 Enk'#243'dol'#225's'
      ImageIndex = 2
      object Label4: TLabel
        Left = 8
        Top = 20
        Width = 105
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'MP3 bitrate:'
      end
      object Label5: TLabel
        Left = 216
        Top = 20
        Width = 105
        Height = 13
        AutoSize = False
        Caption = 'Kbps'
      end
      object bitrate: TComboBox
        Left = 120
        Top = 16
        Width = 89
        Height = 21
        Style = csDropDownList
        ItemHeight = 13
        ItemIndex = 6
        TabOrder = 0
        Text = '128'
        Items.Strings = (
          '32'
          '48'
          '64'
          '80'
          '96'
          '112'
          '128'
          '160'
          '192'
          '256'
          '320')
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Metadata'
      ImageIndex = 3
      object Label6: TLabel
        Left = 8
        Top = 28
        Width = 113
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'C'#237'm URL vagy f'#225'jln'#233'v:'
      end
      object Label7: TLabel
        Left = 8
        Top = 52
        Width = 113
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Let'#246'lt'#233'si gyakoris'#225'g:'
      end
      object Label8: TLabel
        Left = 192
        Top = 52
        Width = 105
        Height = 13
        AutoSize = False
        Caption = 'm'#225'sodperc'
      end
      object Label11: TLabel
        Left = 8
        Top = 108
        Width = 113
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Stream neve:'
      end
      object Label12: TLabel
        Left = 8
        Top = 132
        Width = 113
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Stream URL:'
      end
      object Label13: TLabel
        Left = 8
        Top = 156
        Width = 113
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'St'#237'lus:'
      end
      object Label9: TLabel
        Left = 8
        Top = 76
        Width = 113
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Sz'#246'veg cser'#233'k:'
      end
      object mdconnector: TEdit
        Left = 128
        Top = 24
        Width = 321
        Height = 21
        TabOrder = 0
        Text = 'http://main.pecstv.hu/mediterran/title.tv'
        OnChange = mdconnectorChange
      end
      object mdinterval: TEdit
        Left = 128
        Top = 48
        Width = 41
        Height = 21
        TabOrder = 1
        Text = '5'
      end
      object ud_mdinterval: TUpDown
        Left = 169
        Top = 48
        Width = 16
        Height = 21
        Associate = mdinterval
        Min = 1
        Max = 600
        Position = 5
        TabOrder = 2
        Wrap = False
      end
      object streamname: TEdit
        Left = 128
        Top = 104
        Width = 321
        Height = 21
        TabOrder = 3
        Text = 'Mediterr'#225'n R'#225'di'#243
      end
      object streamurl: TEdit
        Left = 128
        Top = 128
        Width = 321
        Height = 21
        TabOrder = 4
        Text = 'http://www.mediterranradio.hu'
      end
      object streampublic: TCheckBox
        Left = 128
        Top = 176
        Width = 97
        Height = 17
        Caption = 'Publikus'
        Checked = True
        State = cbChecked
        TabOrder = 5
      end
      object streamgenre: TEdit
        Left = 128
        Top = 152
        Width = 321
        Height = 21
        TabOrder = 6
        Text = 'Pop Top 40, 80'#39's'
      end
      object titleSubsts: TEdit
        Left = 128
        Top = 72
        Width = 321
        Height = 21
        TabOrder = 7
        Text = 'INFO,Mediterr'#225'n R'#225'di'#243';REKLAM,Mediterr'#225'n R'#225'di'#243
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'Webszerver'
      ImageIndex = 4
      object Label10: TLabel
        Left = 80
        Top = 12
        Width = 33
        Height = 13
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Port:'
      end
      object port: TEdit
        Left = 120
        Top = 8
        Width = 49
        Height = 21
        TabOrder = 0
        Text = '8000'
      end
      object ud_port: TUpDown
        Left = 169
        Top = 8
        Width = 16
        Height = 21
        Associate = port
        Min = 80
        Max = 32000
        Position = 8000
        TabOrder = 1
        Thousands = False
        Wrap = False
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'N'#233'vjegy'
      ImageIndex = 5
      object Memo1: TMemo
        Left = 16
        Top = 16
        Width = 305
        Height = 153
        BorderStyle = bsNone
        Color = clBtnFace
        Ctl3D = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        Lines.Strings = (
          'Ezt a programot Gerebenics Andor '#237'rta.'
          ''
          'A program a Winamp DSP plugin SDK, '
          'a Lame MP3 enk'#243'der (www.mp3dev.org), '
          'a LibFaac '#233's a LibFaad2  felhaszn'#225'l'#225's'#225'val '
          'k'#233'sz'#252'lt.'
          ''
          'email: grt2@freemail.hu'
          'msn: grtxx@hotmail.com'
          ''
          '2008.06.14.')
        ParentCtl3D = False
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
      end
    end
  end
  object StaticText1: TStaticText
    Left = 168
    Top = 248
    Width = 21
    Height = 21
    Alignment = taCenter
    AutoSize = False
    BevelInner = bvNone
    BevelKind = bkSoft
    TabOrder = 5
  end
  object voltimer: TTimer
    Enabled = False
    Interval = 20
    OnTimer = voltimerTimer
    Left = 480
    Top = 40
  end
  object MainMenu1: TMainMenu
    Left = 480
    Top = 8
    object Fjl1: TMenuItem
      Caption = 'F'#225'jl'
      object Kilps1: TMenuItem
        Caption = 'Kil'#233'p'#233's'
        OnClick = Kilps1Click
      end
    end
    object savesettings: TMenuItem
      Caption = 'Be'#225'll'#237't'#225'sok ment'#233'se'
      OnClick = savesettingsClick
    end
  end
  object statustimer: TTimer
    Interval = 500
    OnTimer = statustimerTimer
    Left = 480
    Top = 72
  end
end
