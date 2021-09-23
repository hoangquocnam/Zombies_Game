#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include"Bullet.h"

using namespace sf;

class Zombie {
private:
	// CHASER + CRAWLER + BLOATER

	const double CHASER_SPEED = 100;
	const double CRAWLER_SPEED = 25;
	const double BLOATER_SPEED = 50;

	const double CHASER_BLOOD = 2;
	const double CRAWLER_BLOOD = 3;
	const double BLOATER_BLOOD = 7;

	double healthUpgrade = 0;

	// make speed vary
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	Vector2f Position;
	Sprite sprite;
	double Speed;
	double Health;
	bool Live;
	SoundBuffer zommbieBuffer;
	Sound zombiesound;
	int Type;

	Bullet bullet;
	Time timeshoot;

public:
	bool hit(int dam);
	bool isAlive();
	void spawn(double X, double Y, int type, int seed);
	FloatRect getPosition();
	Sprite getSprite();
	void update(double elapsedTime, Vector2f playerLocation);
	int getType();
	Vector2f getCenter();
	bool upgrade(int wave);
};