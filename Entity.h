#ifndef EntityH
#define EntityH

#include "Animation.h"

#include <map>

using namespace std;

enum EntityType{
	ENTITY_PLAYER,
	ENTITY_BULLET,
    ENTITY_LASER,
	ENTITY_ASTEROID,
	ENTITY_ENEMY,
	ENTITY_CURSOR
};

enum EntityState{
	ESTATE_ALIVE,
	ESTATE_COLLAPSE,
	ESTATE_COLLAPSING,
	ESTATE_DESTROY
};

class Entity{
private:
	EntityType type;
	EntityState state;

//---------------------------------------------------------------------------
protected:
	Rect4i hitBox;
	Vec2d speed;

	int health;
	int damage;

	map<String, Animation*> animList;
	Animation *selAnim = NULL;

//---------------------------------------------------------------------------
protected:
	Entity(EntityType Type) : type(Type), state(ESTATE_ALIVE) {};

	void SetState(EntityState st) { state = st; };
	void Move() { hitBox += speed; };

//---------------------------------------------------------------------------
public:
	virtual ~Entity() { for(auto a : animList) delete a.second; };

    EntityType GetType() const { return type; };
	EntityState GetState() const { return state; };

	Rect4i GetHitBox() const { return hitBox; };
	Vec2d GetSpeed() const { return speed; };

	int GetHealth() const { return health; };
	int GetDamage() const { return damage; };

	Animation* GetSelAnim() const { return selAnim; };

	bool WannaDestroy() const { return state == ESTATE_DESTROY; };

	Animation* AddAnim(String Name, Animation* Anim) {
		if(!Anim) throw "Entity.AddAnim: " + BStoS(Name) + " is NULL";
		animList[Name] = Anim;
		return animList[Name];
	};

	Animation* SelAnim(String Name){
		auto anim = animList.find(Name);
		if(anim == animList.end()){
			throw "Entity.SelAnim: " + BStoS(Name) + " now found";
			return NULL;
		}

		if(selAnim) selAnim->Pause();
		selAnim = anim->second;
		return selAnim;
	};

	virtual void OnDraw(TCanvas* Canvas, bool ShowHitbox = false){
		if(selAnim){
            selAnim->Draw(Canvas, hitBox.Center());
			//Bitmap *sprite = selAnim->Sprite();
			//if(!sprite) return;
			//Rect4i rect = Rect4i::BoundsCenter(hitBox.Center(), sprite->Size());
//			Canvas->Draw(rect.LT().x, rect.LT().y, sprite->GetBitmap());
			//Canvas->StretchDraw(hitBox, sprite->GetBitmap());
		}

		if(ShowHitbox){
			Canvas->Pen->Color = clRed;
			Canvas->Pen->Width = 1;
			Canvas->Brush->Style = bsClear;
			Canvas->Rectangle(hitBox);
		}
	};

	virtual void OnUpdate() = 0;
	virtual void OnCollision(Entity*) = 0;
};

//---------------------------------------------------------------------------
#endif
