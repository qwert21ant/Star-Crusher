#ifndef BulletH
#define BulletH

#include "Entity.h"

struct BulletCFG{
	Vec2i HitboxSize;
	Vec2i WindowSize;

	int Speed;
	int Damage;
	EntityType Owner;

	Animation* AnimFly;
	Animation* AnimDestroy;
};

class Bullet : public Entity{
private:
	BulletCFG config;

//---------------------------------------------------------------------------
public:
	Bullet(BulletCFG Config, Vec2i Pos) : Entity(ENTITY_BULLET) {
		config = Config;
		damage = config.Damage;

		hitBox = Rect4i::BoundsCenter(Pos, config.HitboxSize);
		speed = Vec2d(0, config.Speed);

		AddAnim("Fly", new Animation(config.AnimFly));
		AddAnim("Boom", new Animation(config.AnimDestroy));

		SelAnim("Fly")->Play(true);
	};

	~Bullet() {};

	EntityType GetOwner() const { return config.Owner; };

	virtual void OnUpdate() override {
		if(GetState() == ESTATE_COLLAPSE) SetState(ESTATE_COLLAPSING);

		if(!hitBox.IsCollision(config.WindowSize)){
			SetState(ESTATE_DESTROY);
			return;
		}
		if(GetState() == ESTATE_COLLAPSING && !GetSelAnim()->IsPlaying()){
			SetState(ESTATE_DESTROY);
			return;
		}

		if(GetState() == ESTATE_ALIVE) Move();
	};

	virtual void OnCollision(Entity *ent) override {
		if(ent->GetType() == ENTITY_BULLET) return;
        if(ent->GetType() == ENTITY_LASER) return;
		if(ent->GetType() == config.Owner) return;

		if(GetState() != ESTATE_ALIVE) return;
		SetState(ESTATE_COLLAPSE);
		SelAnim("Boom")->Play(false);
	};
};

//---------------------------------------------------------------------------
#endif