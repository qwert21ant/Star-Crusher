//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
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

Storage *storage = new Storage();
GameEngine *game = new GameEngine(storage);
Generator *gener = new Generator(storage);
Bitmap* sprites;

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	try{
//	SetCapture(Form1->Handle);

	srand(time(0));

	HBITMAP hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_SPRITES));
	sprites = new Bitmap();
	sprites->GetBitmap()->Handle = hBmp;
	sprites->SetTransparent(true);

	hBmp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BACKGROUND));
	Bitmap* bk = new Bitmap();
	bk->GetBitmap()->Handle = hBmp;
	storage->AddBitmap("Background", bk);

	storage->AddAnimation("Ship", new Animation(sprites, Rect4i::Bounds(8, 0, 84, 72), 4, 0.1));
	storage->AddAnimation("Ship_Boom", new Animation(sprites, Rect4i::Bounds(8, 72, 110, 98), 3, 0.1));
	storage->AddAnimation("Asteroid", new Animation(sprites, Rect4i::Bounds(8, 170, 78, 78), 1, 0.0));
	storage->AddAnimation("Asteroid_Boom", new Animation(sprites, Rect4i::Bounds(86, 170, 126, 126), 3, 0.2));
	storage->AddAnimation("Bullet1", new Animation(sprites, Rect4i::Bounds(8, 296, 4, 10), 1, 0.0));
	storage->AddAnimation("Bullet1_Boom", new Animation(sprites, Rect4i::Bounds(12, 296, 18, 18), 3, 0.4)); //
	storage->AddAnimation("Bullet2", new Animation(sprites, Rect4i::Bounds(8, 314, 38, 38), 4, 0.1));
	storage->AddAnimation("Bullet2_Boom", new Animation(sprites, Rect4i::Bounds(8, 352, 50, 50), 4, 0.1));
	storage->AddAnimation("Enemy1", new Animation(sprites, Rect4i::Bounds(8, 402, 42, 50), 4, 0.1));
	storage->AddAnimation("Enemy1_Boom", new Animation(sprites, Rect4i::Bounds(176, 402, 52, 52), 3, 0.15));
	storage->AddAnimation("Enemy2", new Animation(sprites, Rect4i::Bounds(8, 454, 68, 62), 4, 0.1));
	storage->AddAnimation("Enemy2_Boom", new Animation(sprites, Rect4i::Bounds(8, 516, 98, 72), 4, 0.1));
	storage->AddAnimation("Laser", new Animation(sprites, Rect4i::Bounds(0, 0, 8, 700), 1, 0.0));
	storage->AddAnimation("Cursor", new Animation(sprites, Rect4i::Bounds(208, 352, 38, 38), 1, 0.0, 0.04));

	storage->AddVec2i("Window_Size", {500, 700});
	storage->AddVec2i("Player_Size", {76, 50});
	storage->AddVec2i("Asteroid_Size", {65, 65});
	storage->AddVec2i("Bullet1_Size", {4, 10});
	storage->AddVec2i("Bullet2_Size", {25, 25});
	storage->AddVec2i("Enemy1_Size", {42, 50});
	storage->AddVec2i("Enemy2_Size", {68, 62});
	storage->AddVec2i("Laser_Size", {8, 700});

	storage->AddRange("Asteroid_Scale", {60, 100});
	storage->AddRange("Asteroid_Speed", {6, 10});
	storage->AddRange("Asteroid_RotSpeed", {5, 10});
	storage->AddRange("Asteroid_DevAngle", {-15, 15});


	storage->AddDouble("DEBUG_MODE", 0.0);

	storage->AddDouble("Player_FireSpeed", 0.3);
	storage->AddDouble("Enemy1_FireSpeed", 0.012);
	storage->AddDouble("Enemy2_FireSpeed", 0.005);
	storage->AddDouble("Bullet1_Speed", 20.0);
	storage->AddDouble("Bullet2_Speed", 9.0);

	storage->AddDouble("Background_ScrollSpeed", 0.5);

	storage->AddVec2i("Button_Size", {70, 70});
	storage->AddVec2i("Button1_Pause_Pos", {125, 400});
	storage->AddVec2i("Button2_Pause_Pos", {250, 400});
	storage->AddVec2i("Button3_Pause_Pos", {375, 400});
	storage->AddVec2i("Button1_Menu_Pos", {166, 430});
	storage->AddVec2i("Button3_Menu_Pos", {333, 430});

	storage->AddBitmap("Button1_Up", sprites->Select(Rect4i::Bounds({8, 588}, storage->GetVec2i("Button_Size"))));
	storage->AddBitmap("Button1_Down", sprites->Select(Rect4i::Bounds({78, 588}, storage->GetVec2i("Button_Size"))));
	storage->AddBitmap("Button2_Up", sprites->Select(Rect4i::Bounds({148, 588}, storage->GetVec2i("Button_Size"))));
	storage->AddBitmap("Button2_Down", sprites->Select(Rect4i::Bounds({218, 588}, storage->GetVec2i("Button_Size"))));
	storage->AddBitmap("Button3_Up", sprites->Select(Rect4i::Bounds({288, 588}, storage->GetVec2i("Button_Size"))));
	storage->AddBitmap("Button3_Down", sprites->Select(Rect4i::Bounds({358, 588}, storage->GetVec2i("Button_Size"))));


	storage->AddDouble("Player_Health", 700.0);
    storage->AddDouble("Asteroid_Damage", 100.0);
	storage->AddDouble("Bullet1_Damage", 15.0);
	storage->AddDouble("Bullet2_Damage", 40.0);


	game->SetControls(Form1, Image1, Timer1, Label1, Label2, Label3, Label4, Label5, gener);
	game->SetPlayerFunc([](Entity* ent){ game->AddEntity(ent); });

	gener->SetFuncs([](Entity* ent){ game->AddEntity(ent); },
					[](int score){ game->AddScore(score); });

	game->Init();
	game->SetButtonFuncs([](){ game->Restart(); },
						 [](){ game->Continue(); },
						 [](){ game->Restart(); },
						 [](){ Application->Terminate(); });

    game->Menu();
	}catch(String s){
		InfoBox(s);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	try{
		game->OnUpdate();
	}catch(String s){
		InfoBox(s);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
	game->Final();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	game->OnMouseMove(Vec2i(X, Y));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyPress(TObject *Sender, System::WideChar &Key)
{
	game->OnKeyPress(Key);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	game->OnMouseDown(Vec2i(X, Y));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	game->OnMouseUp(Vec2i(X, Y));
}
//---------------------------------------------------------------------------


void __fastcall TForm1::LabelMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
    game->OnMouseMove(Vec2i(((TControl*)Sender)->Left + X, ((TControl*)Sender)->Top + Y));
}
//---------------------------------------------------------------------------

