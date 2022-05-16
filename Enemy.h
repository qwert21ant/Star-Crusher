#ifndef EnemyH
#define EnemyH

#include "Bullet.h"
#include "Laser.h"

struct EnemyMovement{
	int type;
	int xMax;
};

Vec2i SpeedFunc(EnemyMovement mv, int y){
	switch(mv.type){
		case 1:
			return Vec2i(0, 1);
		break;
		case 2:
			return Vec2i(sin(y / 10.) * mv.xMax, 1);
		break;
		case 3:
			return Vec2i(2 * ((y / mv.xMax) % 2 ? 1 : -1), 1);
		break;
	}
	return Vec2i();
}

struct EnemyCFG{
	int type;

	Vec2i HitboxSize;
    Vec2i WindowSize;

	int Offset;
    int Score;
	int Health;
	double FireSpeed;

	EnemyMovement Movement;
	void (*AddEntityFunc)(Entity*);
	void (*AddScoreFunc)(int);

	BulletCFG BulletConfig;
	LaserCFG LaserConfig;

	Animation* AnimFly;
	Animation* AnimDestroy;
};

class Enemy : public Entity {
private:
	EnemyCFG config;
	Timer fire;

public:
	Enemy(EnemyCFG Config) : Entity(ENTITY_ENEMY) {
		config = Config;
		health = config.Health;
		damage = 0;

		hitBox = Rect4i::BoundsCenter(Vec2i(Random(config.Offset, config.WindowSize.x - config.Offset), -config.HitboxSize.y / 2 + 1), config.HitboxSize);
		fire.SetSpeed(config.FireSpeed);

		AddAnim("Fly", new Animation(config.AnimFly));
		AddAnim("Boom", new Animation(config.AnimDestroy));

		SelAnim("Fly")->Play(true);
	};

	~Enemy() {};

	virtual void OnUpdate() override {
		if(health <= 0 && GetState() == ESTATE_ALIVE){
			SetState(ESTATE_COLLAPSING);
			config.AddScoreFunc(config.Score);
			SelAnim("Boom")->Play(false);
			return;
		}

		if(!hitBox.IsCollision(config.WindowSize)){
            config.AddScoreFunc(config.Score * -3);
			SetState(ESTATE_DESTROY);
			return;
		}

		if(GetState() == ESTATE_COLLAPSING && !GetSelAnim()->IsPlaying()){
			SetState(ESTATE_DESTROY);
			return;
		}

		speed = SpeedFunc(config.Movement, hitBox.Center().y);

		if(GetState() == ESTATE_ALIVE) Move();

		if(fire.Check()){
			Bullet *blt = new Bullet(config.BulletConfig, Vec2i(hitBox.Center().x, hitBox.bottom));
			config.AddEntityFunc(blt);
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
