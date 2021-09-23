#include"Zombies.h"
#include"TextureHolder.h"
#include<cstdlib>
#include<ctime>

using namespace std;

void Zombie::spawn(double X, double Y, int type, int seed) {
	switch (type)
	{
	case 0:
		// BLOATER
		sprite = Sprite(TextureHolder::getTexture("graphics/bloater.png"));
		Speed = BLOATER_SPEED;
		Health = BLOATER_BLOOD;
		
		break;
	case 1:
		// CHASER
		sprite = Sprite(TextureHolder::getTexture("graphics/chaser.png"));
		Speed = CHASER_SPEED;
		Health = CHASER_BLOOD;
		break;
	case 2:
		// CRAWLER
		sprite = Sprite(TextureHolder::getTexture("graphics/crawler.png"));
		Speed = CRAWLER_SPEED;
		Health = CRAWLER_BLOOD;
		break;
	}

	srand((int)time(0) * seed);

	double modifier = (rand() % MAX_VARRIANCE) + OFFSET;

	modifier /= 100;
	Speed *= modifier;

	Position.x = X;
	Position.y = Y;
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setPosition(Position);
	Live = 1;
	zommbieBuffer.loadFromFile("sound/zombie.wav");
	zombiesound.setBuffer(zommbieBuffer);
	Type = type;
	Health += healthUpgrade;
}

bool Zombie::hit(int dam) {
	Health -= dam;
	if (Health <= 0) {
		Live = 0;
		sprite.setTexture(TextureHolder::getTexture("graphics/blood.png"));
		return true;
	}
	return false;
}

bool Zombie::isAlive() {
	return Live;
}

FloatRect Zombie::getPosition() {
	return sprite.getGlobalBounds();
}

Sprite Zombie::getSprite() {
	return sprite;
}

void Zombie::update(double elapsedTime, Vector2f playerLocation) {
	double playerX = playerLocation.x;
	double playerY = playerLocation.y;

	if (playerX > Position.x)
	{
		Position.x += (Speed * elapsedTime);
	}
	if (playerY > Position.y)
	{
		Position.y += (Speed * elapsedTime);
	}
	if (playerX < Position.x)
	{
		Position.x -= (Speed * elapsedTime);
	}
	if (playerY < Position.y)
	{
		Position.y -= (Speed * elapsedTime);
	}

	sprite.setPosition(Position);

	float angle = (atan2(playerY - Position.y, playerX - Position.x) * 180) / 3.141;
	sprite.setRotation(angle);
	if (!Live) zombiesound.play();
}

int Zombie::getType() {
	return Type;
}

Vector2f Zombie::getCenter() {
	return Vector2f(getPosition().left + getPosition().width / 2, getPosition().top + getPosition().height / 2);
}

bool Zombie::upgrade(int wave) {
	if (wave > 1) {
		if (wave % 2 == 1) {
			healthUpgrade++;
		}
		return true;
	}
	healthUpgrade = 0;
	return false;
}

