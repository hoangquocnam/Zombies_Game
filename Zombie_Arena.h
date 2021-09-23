#pragma once
#include"Zombies.h"
#include"Bullet.h"
using namespace sf;

int createBackground(VertexArray& VA, IntRect arena) {
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTEX_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worrldHeight = arena.height / TILE_SIZE;

	VA.setPrimitiveType(Quads);

	VA.resize(worldWidth * worrldHeight * VERTEX_IN_QUAD);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++) {
		for (int h = 0; h < worrldHeight; h++) {
			VA[currentVertex].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);
			VA[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);
			VA[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);
			VA[currentVertex + 3].position = Vector2f(w * TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			if (h == 0 || w == 0 || h == worrldHeight - 1 || w == worldWidth - 1) {
				VA[currentVertex].texCoords = Vector2f(0, TILE_SIZE * TILE_TYPES);
				VA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, TILE_SIZE * TILE_TYPES);
				VA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_SIZE * TILE_TYPES);
				VA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_SIZE * TILE_TYPES);

			}
			else {
				srand((int)time(0) + h * w - h);
				int r = rand() % 3;
				int v = r * TILE_SIZE;
				VA[currentVertex].texCoords = Vector2f(0, v);
				VA[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, v);
				VA[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + v);
				VA[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + v);
			}
			currentVertex += VERTEX_IN_QUAD;

		}
	}
	return TILE_SIZE;
}

Zombie* createHorde(int numZombies, IntRect arena, Bullet *&zombie_shoot, Time *&timeZombieshoot) {
	Zombie* zombies = new Zombie[numZombies];
	zombie_shoot = new Bullet[numZombies];
	timeZombieshoot = new Time[numZombies];
	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numZombies; i++) {
		srand((int)time(0) * i);
		int side = (rand() % 4);
		double X, Y;
		zombie_shoot[i] = Bullet(false);
		switch (side)
		{
		case 0:
			// left
			X = minX;
			Y = (double)(rand() % maxY) + minY;
			break;
		case 1:
			//right
			X = maxX;
			Y = (double)(rand() % maxY) + minY;
			break;

		case 2:
			//top
			X = (double)(rand() % maxX) + minX;
			Y = minY;
			break;

		case 3:
			//bottom
			X = (double)(rand() % maxX) + minX;
			Y = maxY;
			break;
		}
		srand((int)time(0) * i * 2);
		int type = rand() % 3;
		zombies[i].upgrade(numZombies / 5);
		zombies[i].spawn(X, Y, type, i);
		
	}
	return zombies;
}

bool Reload(int & bulletSpare, int clipSize, int & bullet_in_Clip) {
	if (bulletSpare >= (clipSize - bullet_in_Clip)) {
		bulletSpare -= (clipSize - bullet_in_Clip);
		bullet_in_Clip = clipSize;
		return true;
	}
	else if (bulletSpare > 0) {
		bullet_in_Clip += bulletSpare;
		bulletSpare = 0;
		return true;
	}
	else {
		return false;
	}
}
