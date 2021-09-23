#include"Bullet.h"

using namespace sf;

Bullet::Bullet(bool isPlayer) {
	if (isPlayer) {
		bulletShape.setSize(Vector2f(7, 7));
		bulletShape.setFillColor(Color{ 229, 170, 46 });
		Speed = 1000;
	}
	else {
		bulletShape.setSize(Vector2f(10, 10));
		bulletShape.setOutlineThickness(5);
		bulletShape.setOutlineColor(Color{ 39,114,13 });
		bulletShape.setFillColor(Color{95,200,65});
		Speed = 300;
	}
}

void Bullet::stop() {
	Flying = 0;
}

bool Bullet::isFlying() {
	return Flying;
}

void Bullet::shoot(double X, double Y, double DesX, double DesY, int &countDead, bool &usePow) {
	Flying = 1;
	if (countDead >= 5 && usePow) {
		bulletShape.setSize(Vector2f(20, 20));
		bulletShape.setFillColor(Color{ 255, 112, 64 });
		bulletShape.setOutlineThickness(5);
		bulletShape.setOutlineColor(Color{ 255, 226, 64 });
		dam = 5 + START_DAM;
		countDead = 0;
		usePow = 0;
	}
	else {
		bulletShape.setSize(Vector2f(7, 7));
		bulletShape.setFillColor(Color{ 229, 170, 46 });
		bulletShape.setOutlineThickness(0);
		dam = START_DAM;
	}
	Position.x = X;
	Position.y = Y;

	double gradient = (X - DesX) / (Y - DesY);
	if (gradient < 0) gradient *= -1;
	double ratio = Speed / (1 + gradient);
	DistanceY = ratio;
	DistanceX = ratio * gradient;
	if (DesX < X) DistanceX *= -1;
	if (DesY < Y) DistanceY *= -1;

	double range = 1000;
	minX = X - range;
	maxX = X + range;
	minY = Y - range;
	maxY = Y + range;
	bulletShape.setPosition(Position);

}

FloatRect Bullet::getPosition() {
	return bulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape() {
	return bulletShape;
}

void Bullet::update(double elapsedTime, IntRect arena) {
	Position.x += DistanceX * elapsedTime;
	Position.y += DistanceY * elapsedTime;

	bulletShape.setPosition(Position);
	if (Position.x < minX || Position.x > maxX || Position.y < minY || Position.y > maxY) {
		Flying = 0;
	}
	if (Position.x < arena.left + 50 || Position.x > arena.width - 50 || Position.y < arena.top + 50 || Position.y >arena.height - 50) {
		Flying = 0;
	}
}

int Bullet::getDamage() {
	return dam;
}

void Bullet::Zombie_shoot(double X, double Y, double DesX, double DesY, int Level) {
	Flying = 1;
	Speed += Level*10;
	Position.x = X;
	Position.y = Y;

	double gradient = (X - DesX) / (Y - DesY);
	if (gradient < 0) gradient *= -1;
	double ratio = Speed / (1 + gradient);
	DistanceY = ratio;
	DistanceX = ratio * gradient;
	if (DesX < X) DistanceX *= -1;
	if (DesY < Y) DistanceY *= -1;

	double range = 1000;
	minX = X - range;
	maxX = X + range;
	minY = Y - range;
	maxY = Y + range;
	bulletShape.setPosition(Position);
	dam = 1;
	
}

void Bullet::speedUp() {
	Speed += Speed * 0.3;
}

void Bullet::damUp() {
	START_DAM++;
}

void Bullet::reset() {
	START_DAM = 1;
	dam = 1;
}