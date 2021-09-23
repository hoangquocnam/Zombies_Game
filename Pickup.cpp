	#include"Pickup.h"
#include"TextureHolder.h"

using namespace sf;

Pickup::Pickup(int type) {
	Type = type;
	if (type == 2) {
		sprite.setTexture(TextureHolder::getTexture("graphics/ammo_pickup.png"));
		Value = AMMO_ADD;
	}
	else if (type == 1) {
		sprite.setTexture(TextureHolder::getTexture("graphics/health_pickup.png"));
		Value = HEALTH_ADD;
	}
	else if (type == 3) {
		sprite.setTexture(TextureHolder::getTexture("graphics/clock_pickup.png"));
		Value = CLOCK_ADD;
	}

	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().width / 2);
	Sec_Live = TIME_LIVE;
	Sec_Wait = TIME_WAIT;
}

void Pickup::setArena(IntRect arena) {
	Arena.left = arena.left + 50;
	Arena.width = arena.width - 50;
	Arena.top = arena.top + 50;
	Arena.height = arena.height - 50;
	spawn();
}

void Pickup::spawn() {
	srand(time(0) * Type);
	int X = rand() % (Arena.width - 100) + 50;
	srand(time(0) * Type * Type);
	int Y = rand() % (Arena.height - 100) + 50;
	Sec_Since_Spawn = 0;
	Spawned = 1;
	sprite.setPosition(X, Y);
}


FloatRect Pickup::getPosition() {
	return sprite.getGlobalBounds();
}

Sprite Pickup::getSprite() {
	return sprite;
}

bool Pickup::isSpawned() {
	return Spawned;
}

int Pickup::got() {
	Spawned = 0;
	Sec_Since_Despawn = 0;
	return Value;
}

void Pickup::upgrade() {
	if (Type == 1) {
		Value += (HEALTH_ADD * 0.5);
	}
	else if (Type == 2){
		Value += (AMMO_ADD * 0.5);
	}
	else if (Type == 3) {
		Value += 5;
	}
	Sec_Live += (TIME_LIVE * 0.5);
	Sec_Wait += (TIME_WAIT * 0.5);
}

void Pickup::update(double elapsedTime) {
	if (Spawned) {
		Sec_Since_Spawn += elapsedTime;
	}
	else Sec_Since_Despawn += elapsedTime;

	if (Sec_Since_Spawn > Sec_Live && Spawned) {
		Sec_Since_Despawn = 0;
		Spawned = 0;
	}
	if (Sec_Since_Despawn > Sec_Wait && !Spawned) {
		spawn();
	}
}