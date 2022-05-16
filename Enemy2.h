#ifndef Enemy2H
#define Enemy2H

#include "Enemy.h"

class Enemy2 : public Entity {
private:
	EnemyCFG config;
	Timer fire;
	Timer lasTimer;
	bool isLaser = false;
	Laser* laser;

public:
	Enemy2(EnemyCFG Config) : Entity(ENTITY_ENEMY) {
		config = Config;
		health = config.Health;
		damage = 0;

		hitBox = Rect4i::BoundsCenter(Vec2i(Random(config.Offset, config.WindowSize.x - config.Offset), -config.HitboxSize.y / 2 + 1), config.HitboxSize);
		fire.SetSpeed(config.FireSpeed);
		lasTimer.SetSpeed(config.LaserConfig.TTL);

		AddAnim("Fly", new Animation(config.AnimFly));
		AddAnim("Boom", new Animation(config.AnimDestroy));

		SelAnim("Fly")->Play(true);
	};

	~Enemy2() {};

	virtual void OnUpdate() override {
		if(lasTimer.Check() && isLaser){
			laser->Kill();
			isLaser = false;
		}

		if(health <= 0 && GetState() == ESTATE_ALIVE){
			SetState(ESTATE_COLLAPSING);
			config.AddScoreFunc(config.Score);
			SelAnim("Boom")->Play(false);
			if(isLaser) laser->Kill();
			return;
		}

		if(GetState() == ESTATE_COLLAPSING && !GetSelAnim()->IsPlaying()){
			SetState(ESTATE_DESTROY);
			return;
		}

		if(GetState() != ESTATE_ALIVE) return;

		speed = hitBox.Center().y < 100 ? Vec2d(0, 2) : Vec2d(0, 0);

		Move();

		if(fire.Check() && hitBox.Center().y >= 100){
			isLaser = true;
			laser = new Laser(config.LaserConfig, Vec2i(hitBox.Center().x, hitBox.bottom - 1));
			config.AddEntityFunc(laser);
		}
	};

	virtual void OnCollision(Entity* ent) override {
        if(ent->GetState() != ESTATE_ALIVE &&
			ent->GetState() != ESTATE_COLLAPSE) return;

		if(ent->GetType() == ENTITY_ASTEROID) return;

		if(ent->GetType() == ENTITY_BULLET){
			if(((Bullet*) ent)->GetOwner() == GetType()) return;
			health -= ent->GetDamage();
			return;
		}
	};
};

#endif