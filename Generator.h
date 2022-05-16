#ifndef GeneratorH
#define GeneratorH

#include "Enemy.h"
#include "Enemy2.h"
#include "Asteroid.h"
#include "GlobalStorage.h"

class Generator{
private:
	Storage* storage;

	Timer timer;
	int sec = 0;
	int stage = 5;

	Timer asterTimer;

	AsteroidCFG asterCFG;
	map<String, EnemyCFG> enemys;

	void (*AddEntityFunc)(Entity*) = NULL;
	void (*AddScoreFunc)(int) = NULL;

	void LoadFromStorage();

//---------------------------------------------------------------------------
public:
	Generator(Storage*);
	~Generator();

	void Init();
	void Final();

	void SetFuncs(void (*)(Entity*), void (*)(int));

	void Restart();

	void OnUpdate();
};

//---------------------------------------------------------------------------
#endif
