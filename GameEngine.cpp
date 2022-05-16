//---------------------------------------------------------------------------

#pragma hdrstop

#include "GameEngine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

GameEngine::GameEngine(Storage* _Storage){
	storage = _Storage;
}

GameEngine::~GameEngine(){
	delete cur;

	delete butStart;
    delete butContinue;
	delete butRestart;
	delete butExit;
}

void GameEngine::SetControls(TForm* Form, TImage* Image,
		TTimer* Timer, TLabel* Lab1, TLabel* Lab2, TLabel* Lab3,
		TLabel* Lab4, TLabel* Lab5, Generator* Gener){
	form = Form;
	image = Image;
	timer = Timer;
	lab.push_back(Lab1);
	lab.push_back(Lab2);
	lab.push_back(Lab3);
	lab.push_back(Lab4);
	lab.push_back(Lab5);
	gener = Gener;
}

//---------------------------------------------------------------------------

void GameEngine::Init(){
	LoadFromStorage();
	LoadFontFromRes();

	form->Width = wndSize.x + form->Width - form->ClientWidth;
	form->Height = wndSize.y + form->Height - form->ClientHeight;
	form->Left = (Screen->Width - form->Width) / 2;
	form->Top = (Screen->Height - form->Height) / 2;

	image->Width = wndSize.x;
	image->Height = wndSize.y;
	image->Left = 0;
	image->Top = 0;

	timer->Interval = 10;
	timer->Enabled = true;

	for(auto l : lab){
		l->Font->Name = "Fifaks 1.0 dev1";
		l->Font->Color = clWhite;
	}

	lab[0]->Font->Height = 30;
	lab[0]->Left = 10;
	lab[0]->Top = 10;

	lab[1]->Font->Height = 30;
	lab[1]->Caption = "Score: 000000";
	lab[1]->Left = wndSize.x - lab[1]->Width - 10;
	lab[1]->Top = 10;

	lab[2]->Font->Height = 70;

	lab[3]->Font->Height = 30;
	lab[3]->Left = 80;
	lab[3]->Top = wndSize.y / 2 - 50;

	lab[4]->Font->Height = 30;
	lab[4]->Left = 80;
	lab[4]->Top = wndSize.y / 2 - 20;

	if(!DEBUG_MODE) ShowCursor(false);

	cur = new MyCursor(storage->GetAnimation("Cursor"));
    GetClipCursor(&rcOldClip);

	gener->Init();
}

void GameEngine::Final(){
	ShowCursor(true);
	gener->Final();
}

//---------------------------------------------------------------------------

void GameEngine::Restart(){
	state = GSTATE_GAME;
    lab[0]->Visible = true;
	lab[1]->Visible = true;
	lab[2]->Visible = false;
	lab[3]->Visible = false;
	lab[4]->Visible = false;

	ply = new Player(plyCFG, wndSize / 2.0);
	for(auto e : obj)
		delete e;
	obj.clear();
	gener->Restart();
	score = 0;
}

void GameEngine::Pause(){
	state = GSTATE_WANNAPAUSE;
	lab[2]->Visible = true;

	lab[2]->Caption = "Pause";
	lab[2]->Left = (wndSize.x - lab[2]->Width) / 2;
	lab[2]->Top = wndSize.y / 2 - 100;

	butContinue->SetPos(storage->GetVec2i("Button1_Pause_Pos"));
	butRestart->SetPos(storage->GetVec2i("Button2_Pause_Pos"));
	butExit->SetPos(storage->GetVec2i("Button3_Pause_Pos"));
}

void GameEngine::Continue(){
	state = GSTATE_GAME;
	lab[0]->Visible = true;
	lab[1]->Visible = true;
	lab[2]->Visible = false;
	lab[3]->Visible = false;
	lab[4]->Visible = false;
}

void GameEngine::Menu(){
	state = GSTATE_MENU;
	lab[0]->Visible = false;
	lab[1]->Visible = false;
	lab[2]->Visible = true;

	lab[2]->Caption = "Menu";
	lab[2]->Left = (wndSize.x - lab[2]->Width) / 2;
	lab[2]->Top = wndSize.y / 2 - 130;

	if(1){
		lab[3]->Visible = true;
		lab[4]->Visible = true;

		lab[3]->Caption = "Best score: ";
		String s = IntToStr(maxScore);
		for(int i = 0; i < 6 - s.Length(); i++)
			lab[3]->Caption += "0";
		lab[3]->Caption += s;

		lab[4]->Caption = "     Score: ";
		s = IntToStr(score);
		for(int i = 0; i < 6 - s.Length(); i++)
			lab[4]->Caption += "0";
		lab[4]->Caption += s;
	}

	butStart->SetPos(storage->GetVec2i("Button1_Menu_Pos"));
	butExit->SetPos(storage->GetVec2i("Button3_Menu_Pos"));
}

//---------------------------------------------------------------------------

void GameEngine::ClearScreen(){
	image->Canvas->Brush->Style = bsSolid;
	image->Canvas->Brush->Color = clWhite;
	image->Canvas->FillRect(Rect4i(wndSize));
}

//---------------------------------------------------------------------------

void GameEngine::AddScore(int Score){
	score += Score;
    score = max(score, 0);
}

void GameEngine::AddEntity(Entity* entity){
	obj.push_back(entity);
}

void GameEngine::SetPlayerFunc(void (*func)(Entity*)){
    plyCFG.AddEntityFunc = func;
}

void GameEngine::SetButtonFuncs(void (*func1)(), void (*func2)(), void (*func3)(), void (*func4)()){
	butStart->SetFunc(func1);
	butContinue->SetFunc(func2);
	butRestart->SetFunc(func3);
    butExit->SetFunc(func4);
}

//---------------------------------------------------------------------------

void GameEngine::ClipCursorToWindow(){
	TRect rc = form->ClientRect;

	TPoint p1 = form->ClientToScreen({rc.Left, rc.Top});
	TPoint p2 = form->ClientToScreen({rc.Right, rc.Bottom});

	RECT res = {p1.X, p1.Y, p2.X, p2.Y};

	ClipCursor(&res);
}

void GameEngine::UpclipCursorToWindow(){
    ClipCursor(&rcOldClip);
}

void GameEngine::CheckCollisions(){
	for(int i = 0; i < obj.size(); i++)
		for(int j = i+1; j < obj.size(); j++)
			if(obj[i]->GetHitBox().IsCollision(obj[j]->GetHitBox())){
				obj[i]->OnCollision(obj[j]);
				obj[j]->OnCollision(obj[i]);
			}

	for(int i = 0; i < obj.size(); i++)
		if(ply->GetHitBox().IsCollision(obj[i]->GetHitBox())){
			ply->OnCollision(obj[i]);
			obj[i]->OnCollision((Entity*) ply);
		}
}

void GameEngine::LoadFromStorage(){
	DEBUG_MODE = storage->GetDouble("DEBUG_MODE");
	wndSize = storage->GetVec2i("Window_Size");

	plyCFG = {
		storage->GetVec2i("Player_Size"),
		(int) storage->GetDouble("Player_Health"),
		storage->GetDouble("Player_FireSpeed"),
        plyCFG.AddEntityFunc,
		{
			storage->GetVec2i("Bullet1_Size"),
			wndSize,
			(int) -storage->GetDouble("Bullet1_Speed"),
			(int) storage->GetDouble("Bullet1_Damage"),
			ENTITY_PLAYER,
			storage->GetAnimation("Bullet1"),
			storage->GetAnimation("Bullet1_Boom")
		},
		storage->GetAnimation("Ship"),
		storage->GetAnimation("Ship_Boom")
	};

	background.SetSpeed(storage->GetDouble("Background_ScrollSpeed"));

	Vec2i butSize = storage->GetVec2i("Button_Size");
	butStart = new MyButton(Rect4i(butSize),
			storage->GetBitmap("Button1_Up"), storage->GetBitmap("Button1_Down"), butSize);
	butContinue = new MyButton(Rect4i(butSize),
			storage->GetBitmap("Button1_Up"), storage->GetBitmap("Button1_Down"), butSize);
	butRestart = new MyButton(Rect4i(butSize),
			storage->GetBitmap("Button2_Up"), storage->GetBitmap("Button2_Down"), butSize);
	butExit = new MyButton(Rect4i(butSize),
			storage->GetBitmap("Button3_Up"), storage->GetBitmap("Button3_Down"), butSize);
}

void GameEngine::LoadFontFromRes(){
	TResourceStream* rs(new TResourceStream((int)GetModuleHandle(NULL), L"MY_FONT", L"RT_RCDATA"));

	DWORD dwInstalled = 0;
	HANDLE h = AddFontMemResourceEx(rs->Memory, rs->Size, 0, &dwInstalled);
	Win32Check(h != 0);

	delete rs;

	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
}

void GameEngine::DrawBackground(){
	Bitmap* bk = storage->GetBitmap("Background");
	if(background.Check()){
		offsetBk++;
		if(offsetBk == bk->Size().y)
			offsetBk = 0;
	}
	Rect4i rect = Rect4i(bk->Size() - wndSize, bk->Size()) - Vec2i(0, offsetBk);
	image->Canvas->CopyRect(Rect4i(wndSize), bk->Canvas(), rect);
	if(rect.top < 0){
		Rect4i rect2(bk->Size() - Vec2i(wndSize.x, -rect.top), bk->Size());
		image->Canvas->CopyRect(Rect4i(0, 0, wndSize.x, -rect.top), bk->Canvas(), rect2);
	}
}

void GameEngine::DrawEntities(){
	for(auto e : obj)
		e->OnDraw(image->Canvas, DEBUG_MODE);
	ply->OnDraw(image->Canvas, DEBUG_MODE);
}

void GameEngine::UpdateLabels(){
	String s = IntToStr(max(ply->GetHealth(), 0));
	lab[0]->Caption = "HP: ";
	for(int i = 0; i < 4 - s.Length(); i++)
		lab[0]->Caption += "0";
	lab[0]->Caption += s;

	s = IntToStr(score);
	lab[1]->Caption = "Score: ";
	for(int i = 0; i < 6 - s.Length(); i++)
		lab[1]->Caption += "0";
	lab[1]->Caption += s;
}

//---------------------------------------------------------------------------

void GameEngine::ProcGame(){
	CheckCollisions();

	for(auto e : obj)
		e->OnUpdate();
	ply->OnUpdate();

	for(int i = 0; i < obj.size(); i++)
		if(obj[i]->WannaDestroy()){
			delete obj[i];
			obj.erase(obj.begin() + i);
		}

	DrawBackground();
	DrawEntities();
	UpdateLabels();

    gener->OnUpdate();

	if(ply->WannaDestroy()){
		delete ply;
        if(score > maxScore)
			maxScore = score;
		Menu();
		return;
	}

	if(state == GSTATE_WANNAPAUSE){
		state = GSTATE_PAUSE;
		bmpScreen = new Bitmap(image->Picture->Bitmap);
	}
}

void GameEngine::ProcPause(){
	image->Canvas->Draw(0, 0, bmpScreen->GetBitmap());
	image->Canvas->Brush->Color = TColor(0x00373737);
	image->Canvas->FillRect(Rect4i::BoundsCenter(wndSize / 2.0, Vec2i(wndSize.x, 250)));

	butContinue->OnDraw(image->Canvas);
	butRestart->OnDraw(image->Canvas);
	butExit->OnDraw(image->Canvas);
}

void GameEngine::ProcMenu(){
	DrawBackground();
	image->Canvas->Brush->Color = TColor(0x00373737);
	image->Canvas->FillRect(Rect4i::BoundsCenter(wndSize / 2.0, Vec2i(wndSize.x, 300)));

	butStart->OnDraw(image->Canvas);
	butExit->OnDraw(image->Canvas);
}

void GameEngine::OnUpdate(){
	ClearScreen();
	switch(state){
		case GSTATE_GAME:
        case GSTATE_WANNAPAUSE:
			ProcGame();
		break;
		case GSTATE_PAUSE:
			ProcPause();
		break;
		case GSTATE_MENU:
			ProcMenu();
		break;
	}
	cur->OnDraw(image->Canvas);
}

void GameEngine::OnMouseMove(Vec2i Pos){
	if(state == GSTATE_GAME) ClipCursorToWindow();
	else UpclipCursorToWindow();

	cur->OnMouseMove(Pos);
	if(state == GSTATE_GAME) ply->OnMouseMove(Pos);
}

void GameEngine::OnMouseDown(Vec2i Pos){
	if(state != GSTATE_MENU && state != GSTATE_PAUSE) return;
	if(state == GSTATE_MENU) butStart->OnMouseDown(Pos);
	if(state == GSTATE_PAUSE) butContinue->OnMouseDown(Pos);
	butRestart->OnMouseDown(Pos);
	butExit->OnMouseDown(Pos);
}

void GameEngine::OnMouseUp(Vec2i Pos){
	if(state != GSTATE_MENU && state != GSTATE_PAUSE) return;
	if(state == GSTATE_MENU) butStart->OnMouseUp(Pos);
	if(state == GSTATE_PAUSE) butContinue->OnMouseUp(Pos);
	butRestart->OnMouseUp(Pos);
	butExit->OnMouseUp(Pos);
}

void GameEngine::OnKeyPress(System::WideChar Key){
	if(Key == VK_ESCAPE){
		if(state == GSTATE_MENU) return;
		if(state == GSTATE_GAME){
			Pause();
		}else if(state == GSTATE_PAUSE){
			Continue();
			if(bmpScreen) delete bmpScreen;
		}
	}
}

//---------------------------------------------------------------------------
