#ifndef GameEngineH
#define GameEngineH

#include "Cursor.h"
#include "Player.h"
#include "Generator.h"

#include "Button.h"

using namespace Graphics;

enum GameState{
	GSTATE_GAME,
	GSTATE_WANNAPAUSE,
    GSTATE_PAUSE,
	GSTATE_MENU
};

class GameEngine{
private:
	Storage* storage;
    Generator* gener;

	GameState state = GSTATE_MENU;

	Vec2i wndSize;

	TForm* form;
	TImage* image;
	TTimer* timer;
	vector<TLabel*> lab;

	MyCursor* cur;
    PlayerCFG plyCFG;
	Player* ply;
	vector<Entity*> obj;

	bool DEBUG_MODE;
    RECT rcOldClip;

	Timer background;
	int offsetBk = 0;
	Bitmap* bmpScreen = NULL;

	MyButton *butStart, *butContinue, *butRestart, *butExit;

	int score = 0;
	int maxScore = 0;
	bool hasPlayed = false;

//---------------------------------------------------------------------------
private:
	void ClipCursorToWindow();
	void UpclipCursorToWindow();

	void CheckCollisions();
	void LoadFromStorage();
	void LoadFontFromRes();

	void DrawBackground();
	void DrawEntities();
	void UpdateLabels();

	void ProcGame();
	void ProcPause();
	void ProcMenu();

//---------------------------------------------------------------------------
public:
	GameEngine(Storage*);
	~GameEngine();

	void SetControls(TForm*, TImage*, TTimer*, TLabel*, TLabel*, TLabel*, TLabel*, TLabel*, Generator*);

	void Init();
	void Final();

	void Restart();
	void Pause();
	void Continue();
	void Menu();

	void ClearScreen();

	void AddScore(int);
	void AddEntity(Entity*);
	void SetPlayerFunc(void(*)(Entity*));
	void SetButtonFuncs(void(*)(), void(*)(), void(*)(), void(*)());

	void OnUpdate();
	void OnMouseMove(Vec2i);
	void OnMouseDown(Vec2i);
	void OnMouseUp(Vec2i);
	void OnKeyPress(System::WideChar);
};

//---------------------------------------------------------------------------
#endif
