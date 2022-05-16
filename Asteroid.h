#ifndef AsteroidH
#define AsteroidH

#include "Entity.h"

struct AsteroidCFG{
	Vec2i HitboxSize;
	Vec2i WindowSize;

	int Damage;
	Range Scale;
	Range Speed;
	Range RotSpeed;
	Range DevAngle;

	Animation* AnimFly;
	Animation* AnimDestroy;
};

class Asteroid : public Entity {
private:
	AsteroidCFG config;

//---------------------------------------------------------------------------
public:
	Asteroid(AsteroidCFG Config) : Entity(ENTITY_ASTEROID) {
		config = Config;
		damage = config.Damage;

		double scale = Random(config.Scale) / 100.;

		hitBox = Rect4i::BoundsCenter(Vec2i(Random(20, config.WindowSize.x - 20), 0), config.HitboxSize * scale);
        hitBox -= Vec2i(0, hitBox.Height() / 2 - 1);

		speed = Vec2i(0, Random(config.Speed)).Rotate(GradToRad(Random(config.DevAngle)), Vec2i());

		AddAnim("Fly", new Animation(config.AnimFly));
		AddAnim("Boom", new Animation(config.AnimDestroy));

		SelAnim("Fly")->SetRotationSpeed(Random(config.RotSpeed) / 100.);
        GetSelAnim()->SetScale(scale);
		GetSelAnim()->Play(true);
	};

	~Asteroid() {};

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

		Move();
	};

	virtual void OnCollision(Entity *ent) override {
		if(ent->GetType() == ENTITY_PLAYER){
            if(GetState() != ESTATE_ALIVE) return;
			SetState(ESTATE_COLLAPSE);
			SelAnim("Boom")->Play(false);
            speed /= 1.5;
			return;
		}
	};
};

//---------------------------------------------------------------------------
#endif
