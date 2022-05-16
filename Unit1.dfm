object Form1: TForm1
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Star Crusher'
  ClientHeight = 120
  ClientWidth = 208
  Color = clWhite
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  PixelsPerInch = 96
  TextHeight = 13
  object Image1: TImage
    Left = 8
    Top = 8
    Width = 97
    Height = 97
    OnMouseDown = Image1MouseDown
    OnMouseMove = Image1MouseMove
    OnMouseUp = Image1MouseUp
  end
  object Label1: TLabel
    Left = 184
    Top = 16
    Width = 3
    Height = 13
    OnMouseMove = LabelMouseMove
  end
  object Label2: TLabel
    Left = 184
    Top = 35
    Width = 3
    Height = 13
    OnMouseMove = LabelMouseMove
  end
  object Label3: TLabel
    Left = 184
    Top = 53
    Width = 3
    Height = 13
    OnMouseMove = LabelMouseMove
  end
  object Label4: TLabel
    Left = 184
    Top = 77
    Width = 3
    Height = 13
    OnMouseMove = LabelMouseMove
  end
  object Label5: TLabel
    Left = 184
    Top = 98
    Width = 3
    Height = 13
    OnMouseMove = LabelMouseMove
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 128
    Top = 24
  end
end
