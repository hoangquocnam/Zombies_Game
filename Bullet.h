#pragma once
#include<SFML/Graphics.hpp>

using namespace sf;

class Bullet {
private:
	Vector2f Position;
	RectangleShape bulletShape;
	bool Flying = 0;
	double Speed = 1000;
	double DistanceX = 0;
	double DistanceY = 0;
	double maxX = 0, minX = 0, maxY = 0, minY = 0;
	int START_DAM = 1;
	int dam = 1;
public:
	Bullet(bool isPlayer = true);
	void stop();
	bool isFlying();
	void shoot(double X, double Y, double DesX, double DesY, int& countDead, bool& usePow);
	void Zombie_shoot(double X, double Y, double DesX, double DesY, int Level);
	FloatRect getPosition();
	RectangleShape getShape();
	void update(double elapsedTime, IntRect arena);
	int getDamage();
	void speedUp();
	void damUp();
	void reset();
};
