#include "Player.h"


using namespace sf;

Player::Player()
{
	Speed = START_SPEED;
	Health = START_HEALTH;
	MaxHealth = START_HEALTH;
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
}

void Player::spawn(IntRect arena, Vector2f resolution,int tileSize)
{
	
	Position.x = arena.width / 2;
	Position.y = arena.height / 2;
	
	Arena.left = arena.left;
	Arena.width = arena.width;
	Arena.top = arena.top;
	Arena.height = arena.height;

	TileSize = tileSize;

	Resolution.x = resolution.x;
	Resolution.y = resolution.y;
}

void Player::reset()
{
	Speed = START_SPEED;
	Health = START_HEALTH;
	MaxHealth = START_HEALTH;
}

Time Player::getLastHitTime()
{
	return LastHit;
}

bool Player::hit(Time timeHit)
{
	if (timeHit.asMilliseconds() - LastHit.asMilliseconds() > 200)
	{
		LastHit = timeHit;
		Health -= 10;
		return true;
	}
	else {
		return false;
	}
}

FloatRect Player::getPosition()
{
	return sprite.getGlobalBounds();
}

Vector2f Player::getCenter()
{
	return Position;
}

double Player::getRotation()
{
	return sprite.getRotation();
}
Sprite Player::getSprite()
{
	return sprite;
}
int Player::getHealth()
{
	return Health;
}

void Player::moveLeft()
{
	Left = true;
}
void Player::moveRight()
{
	Right = true;
}
void Player::moveUp()
{
	Up = true;
}
void Player::moveDown()
{
	Down = true;
}
void Player::stopLeft()
{
	Left = false;
}
void Player::stopRight()
{
	Right = false;
}
void Player::stopUp()
{
	Up = false;
}
void Player::stopDown()
{
	Down = false;
}

void Player::update(double elapsedTime, Vector2i mousePosition)
{
	if (Up) Position.y -= Speed * elapsedTime;
	
	if (Down) Position.y += Speed * elapsedTime;
	
	if (Right) Position.x += Speed * elapsedTime;
	
	if (Left) Position.x -= Speed * elapsedTime;
	
	sprite.setPosition(Position);

	if (Position.x > Arena.width - TileSize) {
		Position.x = Arena.width - TileSize;
	}
	if (Position.x < Arena.left + TileSize) {
		Position.x = Arena.left + TileSize;
	}
	if (Position.y > Arena.height - TileSize) {
		Position.y = Arena.height - TileSize;
	}
	if (Position.y < Arena.top + TileSize) {
		Position.y = Arena.top + TileSize;
	}

	double angle = (atan2(mousePosition.y - Resolution.y / 2, mousePosition.x - Resolution.x / 2) * 180) / 3.141;
	sprite.setRotation(angle);
}

void Player::SpeedUp() {
	Speed += START_SPEED *0.2;
}

void Player::HealthUp() {
	MaxHealth += START_HEALTH * 0.2;
}

void Player::IncreaseHealthLevel(int amount) {
	Health += amount;
	if (Health > MaxHealth) {
		Health = MaxHealth;
	}
}