#ifndef PlayerH
#define PlayerH

#include "Entity.h"
#include "Bullet.h"
#include "Asteroid.h"

struct PlayerCFG{
	Vec2i HitboxSize;

	int Health;
	double FireSpeed;

	void (*AddEntityFunc)(Entity*);

	BulletCFG BulletConfig;

	Animation* AnimFly;
	Animation* AnimDestroy;
};

class Player : public Entity{
private:
	PlayerCFG config;
	Timer fire;

	Vec2i toPos;

//---------------------------------------------------------------------------
public:
	Player(PlayerCFG Config, Vec2i Pos) : Entity(ENTITY_PLAYER) {
		config = Config;
		health = config.Health;
        damage = 0;

		hitBox = Rect4i::BoundsCenter(Pos, config.HitboxSize);
		fire.SetSpeed(config.FireSpeed);

		AddAnim("Fly", new Animation(config.AnimFly));
		AddAnim("Boom", new Animation(config.AnimDestroy));

		SelAnim("Fly")->Play(true);
	};

	~Player() {};

	virtual void OnUpdate() override {
		if(GetState() == ESTATE_ALIVE && health <= 0){
			SetState(ESTATE_COLLAPSE);
            SelAnim("Boom")->Play(false);
			return;
		}

		if(GetState() == ESTATE_COLLAPSE && !GetSelAnim()->IsPlaying()){
			SetState(ESTATE_DESTROY);
			return;
		}

		if(GetState() != ESTATE_ALIVE) return;

		Vec2i pos = hitBox.Center();
		if(pos != toPos) speed = Vec2d(toPos - pos) * 0.15;

		Move();

		if(fire.Check())
			config.AddEntityFunc(new Bullet(config.BulletConfig, Vec2i(hitBox.Center().x, hitBox.top)));
	};

	virtual void OnCollision(Entity *ent) override {
		if(ent->GetState() != ESTATE_ALIVE &&
			ent->GetState() != ESTATE_COLLAPSE) return;

		if(ent->GetType() == ENTITY_ASTEROID)
			health -= ent->GetDamage();

		if(ent->GetType() == ENTITY_BULLET){
			if(((Bullet*) ent)->GetOwner() == GetType()) return;
			health -= ent->GetDamage();
		}

		if(ent->GetType() == ENTITY_LASER){
            health -= ent->GetDamage();
		}

		// TODO: Enemy
	};

	void OnMouseMove(Vec2i Pos){
		toPos = Pos;
	};
};

//---------------------------------------------------------------------------
#endif
