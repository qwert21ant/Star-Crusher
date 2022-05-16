#ifndef CursorH
#define CursorH

#include "Entity.h"

class MyCursor : public Entity {
public:
	MyCursor(Animation* Anim) : Entity(ENTITY_CURSOR) {
		AddAnim("Main", Anim);
		SelAnim("Main")->Play(true);
	};

	~MyCursor() {};

	void OnMouseMove(Vec2i Pos){
		hitBox = Rect4i::Bounds(Pos, Vec2i());
	};

	virtual void OnUpdate() override {};
	virtual void OnCollision(Entity* ent) override {};
};

//---------------------------------------------------------------------------
#endif