#ifndef BitmapH
#define BitmapH

#include "Utils.h"

class Bitmap{
private:
	TBitmap *bmp;

//---------------------------------------------------------------------------
public:
	Bitmap() : bmp(new TBitmap()) {};
	Bitmap(const Bitmap &Bmp) : bmp(new TBitmap()) { bmp->Assign(Bmp.bmp); };
	Bitmap(Bitmap *Bmp) : bmp(new TBitmap()) { bmp->Assign(Bmp->bmp); };
	Bitmap(TBitmap *Bmp) : bmp(new TBitmap())  { bmp->Assign(Bmp);};
	Bitmap(Vec2i Size) : bmp(new TBitmap(Size.x, Size.y)) {};
	Bitmap(int Width, int Height) : bmp(new TBitmap(Width, Height)) {};
	Bitmap(String File) : bmp(new TBitmap()) { bmp->LoadFromFile(File); };

	~Bitmap() { delete bmp; };

	int Width() const { return bmp->Width; };
	int Height() const { return bmp->Height; };
	Vec2i Size() const { return Vec2i(Width(), Height()); };
	Rect4i Rect() const { return Rect4i(Size()); };
	TCanvas* Canvas() const { return bmp->Canvas; };
    TBitmap* GetBitmap() const { return bmp; };

	void SetTransparent(bool Transp = true, TColor Col = clWhite){
		bmp->Transparent = Transp;
		bmp->TransparentMode = tmFixed;
		bmp->TransparentColor = Col;
	};

	void Clear(TColor Col = clWhite){
		bmp->Canvas->Pen->Color = Col;
		bmp->Canvas->Brush->Color = Col;
		bmp->Canvas->FillRect(Rect());
	};

	Bitmap* Select(Rect4i _Rect){
        if(Rect() == _Rect) return new Bitmap(bmp);

		TBitmap* tmp = new TBitmap();
		tmp->Transparent = bmp->Transparent;
		if(tmp->Transparent){
			tmp->TransparentMode = bmp->TransparentMode;
			tmp->TransparentColor = bmp->TransparentColor;
		}

		tmp->Width = _Rect.Width();
		tmp->Height = _Rect.Height();

		tmp->Canvas->CopyRect(_Rect - _Rect.LT(), Canvas(), _Rect);

		return new Bitmap(tmp);
	};

	void Crop(Rect4i _Rect){
		if(Rect() == _Rect) return;

		TBitmap* tmp = new TBitmap();
		tmp->Transparent = bmp->Transparent;
		if(tmp->Transparent){
			tmp->TransparentMode = bmp->TransparentMode;
			tmp->TransparentColor = bmp->TransparentColor;
		}

		tmp->Width = _Rect.Width();
		tmp->Height = _Rect.Height();

		tmp->Canvas->CopyRect(_Rect - _Rect.LT(), Canvas(), _Rect);

		delete bmp;
		bmp = tmp;
	};

	void Scale(double K){
		if(abs(K - 1) <= 0.00001) return;

		TBitmap* tmp = new TBitmap();
		tmp->Transparent = bmp->Transparent;
		if(tmp->Transparent){
			tmp->TransparentMode = bmp->TransparentMode;
			tmp->TransparentColor = bmp->TransparentColor;
		}

		tmp->Width = Width() * K;
		tmp->Height = Height() * K;

		tmp->Canvas->CopyRect(Rect4i(Size() * K), Canvas(), Rect4i(Size()));

		delete bmp;
		bmp = tmp;
	};

	void Rotate(double Rad, TColor bkColor = clWhite){
		if(abs(Rad) <= 0.00001) return;

		TBitmap* tmp = new TBitmap();
		tmp->Transparent = bmp->Transparent;
		if(tmp->Transparent){
			tmp->TransparentMode = bmp->TransparentMode;
			tmp->TransparentColor = bmp->TransparentColor;
		}

		tmp->Canvas->Brush->Color = bkColor;

		double c = abs(cos(Rad)), s = abs(sin(Rad));
		tmp->Width = Width() * c + Height() * s;
		tmp->Height = Height() * c + Width() * s;

		Vec2d pts[4] = {Rect().LT().Rotate(Rad, Rect().Center()),
						 Rect().RT().Rotate(Rad, Rect().Center()),
						 Rect().LB().Rotate(Rad, Rect().Center()),
						 Rect().RB().Rotate(Rad, Rect().Center())};

		double minX = min(min(pts[0].x, pts[1].x), min(pts[2].x, pts[3].x));
		double minY = min(min(pts[0].y, pts[1].y), min(pts[2].y, pts[3].y));
		Vec2d offset = {minX < 0 ? minX : 0, minY < 0 ? minY : 0};

		pts[0] -= offset;
		pts[1] -= offset;
		pts[2] -= offset;

		TPoint pts_fin[3] = {pts[0], pts[1], pts[2]};

		PlgBlt(tmp->Canvas->Handle, pts_fin, Canvas()->Handle, 0, 0, Width(), Height(), NULL, 0, 0);

        delete bmp;
		bmp = tmp;
	};
};

//---------------------------------------------------------------------------
#endif