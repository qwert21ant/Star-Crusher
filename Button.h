#ifndef ButtonH
#define ButtonH

#include "Utils.h"
#include "Bitmap.h"

class MyButton{
private:
	Rect4i box;
	Bitmap* bmpNorm;
	Bitmap* bmpDown;

	bool isDowned = false;

	void (*OnClick)();

//---------------------------------------------------------------------------
public:
	MyButton(Rect4i Box, Bitmap* bmpnorm,
			Bitmap* bmpdown, Rect4i SprSize) : box(Box), bmpNorm(bmpnorm),
			bmpDown(bmpdown) {};

	~MyButton() {};

	void SetFunc(void (*func)()){
		OnClick = func;
	};

	void SetPos(Vec2i Pos){
		box = Rect4i::BoundsCenter(Pos, box.Size());
	};

	void OnDraw(TCanvas* Canvas){
		if(isDowned) Canvas->StretchDraw(box, bmpDown->GetBitmap());
		else Canvas->StretchDraw(box, bmpNorm->GetBitmap());
	};

	void OnMouseDown(Vec2i Pos){
		if(box.PtInRect(Pos)) isDowned = true;
	};

	void OnMouseUp(Vec2i Pos){
		if(box.PtInRect(Pos) && isDowned)
			OnClick();
		isDowned = false;
	};
};

//---------------------------------------------------------------------------
#endif