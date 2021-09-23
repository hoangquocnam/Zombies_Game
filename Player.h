#pragma once
#include <SFML/Graphics.hpp>
#include"TextureHolder.h"

using namespace sf;

class Player {
private:
	const int START_SPEED = 250;
	const int START_HEALTH = 100;

	Vector2f Position;
	Texture texture;
	Sprite sprite = Sprite(TextureHolder::getTexture("graphics/player.png"));;

	Vector2f Resolution;

	IntRect Arena;
	int TileSize = 0;

	bool Up = 0;
	bool Down = 0;
	bool Left = 0;
	bool Right = 0;

	int Health = START_HEALTH;
	int MaxHealth = START_HEALTH;

	double Speed = START_SPEED;
	Time LastHit;

public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tilesize);
	void reset();
	bool hit(Time timeHit);
	Time getLastHitTime();
	FloatRect getPosition();
	Vector2f getCenter();
	double getRotation();
	Sprite getSprite();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	void update(double elapsedTime, Vector2i mousePosition);
	void SpeedUp();
	void HealthUp();
	void IncreaseHealthLevel(int amount);
	int getHealth();
};