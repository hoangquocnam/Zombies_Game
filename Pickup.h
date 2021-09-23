#pragma once
#include<SFML/Graphics.hpp>

using namespace sf;

class Pickup {
private:
	const int HEALTH_ADD = 50;
	const int AMMO_ADD = 12;
	const int CLOCK_ADD = 7;
	const int TIME_WAIT = 10;
	const int TIME_LIVE = 5;
	Sprite sprite;
	IntRect Arena;
	int Value;
	int Type;
	bool Spawned;
	double Sec_Since_Spawn, Sec_Since_Despawn, Sec_Live, Sec_Wait;

public:
	Pickup(int type);
	void setArena(IntRect arena);
	void spawn();
	FloatRect getPosition();
	Sprite getSprite();
	void update(double elapsedTime);
	bool isSpawned();
	int got();
	void upgrade();
};
