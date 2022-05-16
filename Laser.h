#ifndef LaserH
#define LaserH

#include "Entity.h"

struct LaserCFG{
	Vec2i HitboxSize;

	double TTL;
	int Damage;
	EntityType Owner;

	Animation* Anim;
};

class Laser : public Entity{
private:
	LaserCFG config;

//---------------------------------------------------------------------------
public:
	Laser(LaserCFG Config, Vec2i Pos) : Entity(ENTITY_LASER) {
		config = Config;
		damage = config.Damage;

		hitBox = Rect4i::Bounds(Vec2i(Pos.x - config.HitboxSize.x / 2, Pos.y), config.HitboxSize);
		speed = Vec2d();

		AddAnim("Main", new Animation(config.Anim));
		SelAnim("Main")->Play(true);
	};

	~Laser() {};

	EntityType GetOwner() const { return config.Owner; };

	void Kill() { SetState(ESTATE_DESTROY); };

	virtual void OnUpdate() override {};

	virtual void OnCollision(Entity *ent) override {};
};

//---------------------------------------------------------------------------
#endif