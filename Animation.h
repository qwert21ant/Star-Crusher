#ifndef AnimationH
#define AnimationH

#include "Bitmap.h"

class Animation{
private:
	vector<Bitmap*> sprites;
	Bitmap* sprite;

	double scale = 1.0;

	double iSprite = 0;
	double speed;

	double rotation = 0;
	double rotatSpeed = 0;

	bool isPlaying = false;
	bool isCycle = false;

//---------------------------------------------------------------------------
public:
	Animation(Bitmap* Bmp, Rect4i ZeroSprite, int ColSprites, double Speed, double RotatSpeed = 0.0) :
			speed(Speed), rotatSpeed(RotatSpeed) {
		if(ColSprites < 1) throw String("Animation: ColSprites < 1");
		if(!Bmp) throw String("Anitmation: Bmp = NULL");

		if(ColSprites == 1) speed = 0;
		else speed = Speed;

		for(int i = 0; i < ColSprites; i++){
			Bitmap* spr = new Bitmap(Bmp);
			Rect4i rect = Rect4i(ZeroSprite + Vec2i(ZeroSprite.Width() * i, 0));
			spr->Crop(rect);
			sprites.push_back(spr);
		}

		sprite = new Bitmap(sprites[0]);
	};

	Animation(const Animation& A) : sprite(new Bitmap(A.sprite)),
			iSprite(A.iSprite), speed(A.speed),
			rotation(A.rotation), rotatSpeed(A.rotatSpeed),
			isPlaying(A.isPlaying), isCycle(A.isCycle) {
		for(int i = 0; i < A.sprites.size(); i++)
			sprites.push_back(A.sprites[i]);
	};

	Animation(Animation* A) : Animation(*A) {};

	~Animation() { delete sprite; };

	Rect4i GetSpriteBox() const { return sprites[0]->Size(); };
    int GetColSprites() const { return sprites.size(); };
	double GetSpeed() const { return speed; };

	void SetRotationSpeed(double RotSpeed) { rotatSpeed = min(RotSpeed, 2 * M_PI); };
	void SetScale(double Scale) { scale = Scale; };

	bool IsPlaying() const { return isPlaying; };

	void Play(bool IsCycle){
		isPlaying = true;
		isCycle = IsCycle;
		iSprite = 0;
	};

	void Pause() { isPlaying = false; };
	void Continue() { isPlaying = true; };

	void Draw(TCanvas* Canvas, Vec2i Pos){
		if(isPlaying){
			if(rotatSpeed || speed){
				delete sprite;
				sprite = new Bitmap(sprites[(int) iSprite]);
			}

			if(rotatSpeed){
				rotation += rotatSpeed;
				if(rotation >= 2 * M_PI) rotation -= 2 * M_PI;
				delete sprite;
				sprite = new Bitmap(sprites[(int) iSprite]);
				sprite->Rotate(rotation);
			}

			if(speed){
				iSprite += speed;
				if(iSprite >= (int) sprites.size()){
					iSprite -= (int) sprites.size();
					if(!isCycle) isPlaying = false;
				}
			}else if(!isCycle) isPlaying = false;
		}

		if(rotatSpeed){
			Rect4i rect = Rect4i::BoundsCenter(Pos, sprite->Size() * scale);
			if(abs(scale - 1.0) >= 0.0001) Canvas->StretchDraw(rect, sprite->GetBitmap());
			else Canvas->Draw(rect.LT().x, rect.LT().y, sprite->GetBitmap());
		}else{
			Rect4i rect = Rect4i::BoundsCenter(Pos, sprites[(int) iSprite]->Size() * scale);
			if(abs(scale - 1.0) >= 0.0001) Canvas->StretchDraw(rect, sprites[(int) iSprite]->GetBitmap());
			else Canvas->Draw(rect.LT().x, rect.LT().y, sprites[(int) iSprite]->GetBitmap());
        }

	};
};

//---------------------------------------------------------------------------
#endif

