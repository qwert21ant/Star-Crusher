//---------------------------------------------------------------------------

#pragma hdrstop

#include "Generator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Generator::Generator(Storage* _Storage){
	storage = _Storage;
}

Generator::~Generator(){
}

void Generator::LoadFromStorage(){
	asterCFG = {
		storage->GetVec2i("Asteroid_Size"),
		storage->GetVec2i("Window_Size"),
		(int) storage->GetDouble("Asteroid_Damage"),
		storage->GetRange("Asteroid_Scale"),
		storage->GetRange("Asteroid_Speed"),
		storage->GetRange("Asteroid_RotSpeed"),
		storage->GetRange("Asteroid_DevAngle"),
		storage->GetAnimation("Asteroid"), storage->GetAnimation("Asteroid_Boom")
	};

	BulletCFG bul1CFG = {
		storage->GetVec2i("Bullet2_Size"),
		storage->GetVec2i("Window_Size"),
		(int) storage->GetDouble("Bullet2_Speed"),
		(int) storage->GetDouble("Bullet2_Damage"),
		ENTITY_ENEMY,
		storage->GetAnimation("Bullet2"), storage->GetAnimation("Bullet2_Boom")
	};

	LaserCFG laserCFG = {
		storage->GetVec2i("Laser_Size"),
		0.01,
		1,
		ENTITY_ENEMY,
		storage->GetAnimation("Laser")
	};

	EnemyCFG enemy1_1 = { 1,
		storage->GetVec2i("Enemy1_Size"),
		storage->GetVec2i("Window_Size"),
		20,
		100,
		150,
		storage->GetDouble("Enemy1_FireSpeed"),
		{1, 0},
		AddEntityFunc, AddScoreFunc,
		bul1CFG, laserCFG,
		storage->GetAnimation("Enemy1"), storage->GetAnimation("Enemy1_Boom")
	};

	enemys["1_1"] = enemy1_1;

	EnemyCFG enemy1_2 = { 1,
		storage->GetVec2i("Enemy1_Size"),
		storage->GetVec2i("Window_Size"),
		80,  //Offset
		100, //Score
		150, //Health
		storage->GetDouble("Enemy1_FireSpeed"),
		{2, 4},
		AddEntityFunc, AddScoreFunc,
		bul1CFG, laserCFG,
		storage->GetAnimation("Enemy1"), storage->GetAnimation("Enemy1_Boom")
	};

	enemys["1_2"] = enemy1_2;

	EnemyCFG enemy2_1 = { 2,
		storage->GetVec2i("Enemy2_Size"),
		storage->GetVec2i("Window_Size"),
		40,
		200,
		300,
		storage->GetDouble("Enemy2_FireSpeed"),
		{0, 0},
		AddEntityFunc, AddScoreFunc,
		bul1CFG, laserCFG,
		storage->GetAnimation("Enemy2"), storage->GetAnimation("Enemy2_Boom")
	};

	enemys["2_1"] = enemy2_1;
}

void Generator::Init(){
	LoadFromStorage();

	timer.SetSpeed(0.009);
	asterTimer.SetSpeed(0.008);
}

void Generator::Final(){

}

void Generator::SetFuncs(void (*Func1)(Entity*), void (*Func2)(int)){
	if(!AddEntityFunc) AddEntityFunc = Func1;
	if(!AddScoreFunc) AddScoreFunc = Func2;
}

void Generator::Restart(){
	sec = 0;
	stage = 1;
	asterTimer.SetSpeed(0.008);

	LoadFromStorage();
}

void Generator::OnUpdate(){
	if(timer.Check()){
		switch(stage){
			case 1: {
				AddEntityFunc(new Enemy(enemys["1_1"]));
				break;
			}
			case 2: {
				if(sec && sec % 2) AddEntityFunc(new Enemy(enemys["1_1"]));
				else AddEntityFunc(new Enemy(enemys["1_2"]));
				break;
			}
			case 3: {
				AddEntityFunc(new Enemy(enemys["1_2"]));
				break;
			}
			case 4: {
				if(sec && !(sec % 3)) AddEntityFunc(new Enemy2(enemys["2_1"]));
				if(sec && sec % 2) AddEntityFunc(new Enemy(enemys["1_2"]));
				break;
			}
			case 5: {
				if(!sec) asterTimer.SetSpeed(0.016);
				if(sec && !(sec % 5)){
					asterTimer.SetSpeed(0.008);

					enemys["1_1"].Score *= 1.3;
					enemys["1_1"].Health *= 1.3;
					if(rand() % 2) enemys["1_1"].BulletConfig.Damage *= 1.3;
					else enemys["1_1"].FireSpeed *= 1.3;

					enemys["1_2"].Score *= 1.3;
					enemys["1_2"].Health *= 1.3;
					if(rand() % 2) enemys["1_2"].BulletConfig.Damage *= 1.3;
					else enemys["1_2"].FireSpeed *= 1.3;

					enemys["2_1"].Score *= 1.5;
					enemys["2_1"].Health *= 1.2;
					if(rand() % 2) enemys["2_1"].LaserConfig.Damage *= 1.3;

					sec = 0;
					stage = 1;
				}
				break;
			}
			default:
                stage = 1;
		}

		sec++;
		if(sec % 10 == 0){
			stage++;
			asterTimer.SetSpeed(asterTimer.GetSpeed() * 1.1);
			sec = 0;
		}
	}
	if(asterTimer.Check())
		AddEntityFunc(new Asteroid(asterCFG));
}