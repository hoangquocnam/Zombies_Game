#include <iostream>
#include <sstream>
#include<fstream>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "Player.h"
#include "Zombies.h"
#include "Zombie_Arena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;

enum class State
{
	PAUSE,
	LEVEL_UP,
	GAME_OVER,
	PLAYING,
};

int main()
{

#pragma region Utilities

	TextureHolder holder;
	Clock clock;
	State state = State::GAME_OVER;
	Time getTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	IntRect arena;
	Time GameTime;
	int TIME_START = 20;
	int countTime = 20;
	int score = 0;
	int HiScore = 0;

	Color colorText = Color{ 222, 203, 207 };

#pragma endregion

#pragma region Window

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombies", Style::Fullscreen);

#pragma endregion

#pragma region View

	View mainView(FloatRect(0, 0, resolution.x, resolution.y));
	View HUD_view(FloatRect(0, 0, resolution.x, resolution.y));

#pragma endregion

#pragma region Player

	Player player;
	bool usePow = 0;

#pragma endregion

#pragma region Zombies

	int numZombies;
	int numZombiesAlive = 0;
	Zombie* zombies = nullptr;
	int countDeadZombie = 0;

	CircleShape deadZombie[5];
	for (int i = 0; i < 5; i++) {
		deadZombie[i].setRadius(15);
		deadZombie[i].setFillColor(Color{ 233,138,21 });
		deadZombie[i].setOutlineColor(Color::Black);
		deadZombie[i].setOutlineThickness(5);
		deadZombie[i].setPosition(925 + 2 * i * 22, 990);
	}
	Bullet* zombie_shoot = nullptr;
	Time* timeZombieshoot = NULL;

#pragma endregion

#pragma region Bullet

	Bullet bullets[100];
	int currentBullet = 0;
	int bulletSpare = 36;
	int bullet_in_Clip = 6;
	int clipSize = 6;
	double fireRate = 5;
	Time lastPressedLeft;
	Time lastPressedSpace;
	Time reloadTime;
	Time timeGameOverText;

#pragma endregion

#pragma region Background

	VertexArray background;
	VertexArray barrier;

	Texture txtBackground = TextureHolder::getTexture("graphics/background_sheet.png");



#pragma endregion

#pragma region Crosschair

	Sprite crosschair = Sprite(TextureHolder::getTexture("graphics/crosshair.png"));
	crosschair.setOrigin(24, 24);

#pragma endregion

#pragma region Pick up

	Pickup HEALTH_PICKUP(1);
	Pickup AMMO_PICKUP(2);
	Pickup CLOCK_PICKUP(3);

#pragma endregion

#pragma region HUD + Text

	// HUD
	Sprite backgroundGameOver = Sprite(TextureHolder::getTexture("graphics/background.png"));
	backgroundGameOver.setPosition(0, 0);

	Sprite AmmoIcon = Sprite(TextureHolder::getTexture("graphics/ammo_pickup.png"));
	AmmoIcon.setPosition(115, 990);

	Sprite HealthIcon = Sprite(TextureHolder::getTexture("graphics/health.png"));
	HealthIcon.setPosition(380, 980);

	Sprite PowerIcon = Sprite(TextureHolder::getTexture("graphics/power.png"));
	PowerIcon.setPosition(850, 980);

	Sprite ClockIcon = Sprite(TextureHolder::getTexture("graphics/clock.png"));
	ClockIcon.setPosition(840, 55);

	Sprite ZombieIcon = Sprite(TextureHolder::getTexture("graphics/zombie.png"));
	ZombieIcon.setPosition(1555, 985);

	
	//Font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	//Pause Text
	Text PauseText;
	PauseText.setFont(font);
	PauseText.setCharacterSize(155);
	PauseText.setFillColor(Color{ 126, 101, 160 });
	PauseText.setPosition(590, 180);
	PauseText.setString("Press Enter \nto continue");

	// Exit Text
	Text ExitText;
	ExitText.setFont(font);
	ExitText.setCharacterSize(100);
	ExitText.setFillColor(Color{ 202, 56, 71 });
	ExitText.setPosition(650, 800);
	ExitText.setString("Press E to exit");

	//Game Over Text
	Text GameOverText;
	GameOverText.setFont(font);
	GameOverText.setCharacterSize(125);
	GameOverText.setFillColor(Color{ 126, 101, 160 });
	GameOverText.setPosition(250, 850);
	GameOverText.setString("Press Enter to play");

	//Level Up Text
	Text LevelUpText;
	LevelUpText.setFont(font);
	LevelUpText.setCharacterSize(80);
	LevelUpText.setFillColor(Color::White);
	LevelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream << "1- Increased rate of fire";
	levelUpStream << "\n2- Increased clip size (next reload)";
	levelUpStream << "\n3- Increased max health";
	levelUpStream << "\n4- Increased run speed";
	levelUpStream << "\n5- More and better health pickups";
	levelUpStream << "\n6- More and better ammo pickups";
	levelUpStream << "\n7- Increaseed damage of fire";
	LevelUpText.setString(levelUpStream.str());

	//Ammo Text
	Text AmmoText;
	AmmoText.setFont(font);
	AmmoText.setCharacterSize(55);
	AmmoText.setFillColor(Color{ 222, 202, 147 });
	AmmoText.setPosition(200, 980);

	// Score Text
	Text ScoreText;
	ScoreText.setFont(font);
	ScoreText.setCharacterSize(55);
	ScoreText.setFillColor(Color{ 238, 212, 161 });
	ScoreText.setPosition(1500, 50);

	// Hi Score
	Text HiScoreText;
	HiScoreText.setFont(font);
	HiScoreText.setCharacterSize(55);
	HiScoreText.setFillColor(Color::White);
	HiScoreText.setPosition(1500, 50);
	std::stringstream s;

	// Highest Score data
	ifstream fi("data.txt", ios::in);
	if (fi.is_open()) {
		fi >> HiScore;
		fi.close();
	}
	s << "Hi Score: " << HiScore;
	HiScoreText.setString(s.str());

	// Zombies remaining
	Text ZombiesRemainingText;
	ZombiesRemainingText.setFont(font);
	ZombiesRemainingText.setCharacterSize(55);
	ZombiesRemainingText.setFillColor(Color{ 114, 168, 110 });
	ZombiesRemainingText.setPosition(1625, 980);
	ZombiesRemainingText.setString("Zombies: 100");

	//// Wave number
	int wave = 0;
	Text WaveNumberText;
	WaveNumberText.setFont(font);
	WaveNumberText.setCharacterSize(55);
	WaveNumberText.setFillColor(colorText);
	WaveNumberText.setPosition(100, 50);
	WaveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color{ 240, 77, 83 });
	healthBar.setPosition(450, 980);
	healthBar.setOutlineThickness(5);
	healthBar.setOutlineColor(Color{ 165,49,13 });


	// Power
	Text PowerText;
	PowerText.setFont(font);
	PowerText.setCharacterSize(55);
	PowerText.setFillColor(colorText);
	PowerText.setPosition(1225, 980);

	// Time of game
	Text TimeText;
	TimeText.setFont(font);
	TimeText.setCharacterSize(55);
	TimeText.setFillColor(Color{ 54, 170, 43 });
	TimeText.setPosition(900, 50);


	int typePower;
	int frames_Since_LastHUD_Update = 0;
	int fpsFrameInterval = 100;



#pragma endregion

#pragma region Sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	Sound splat;
	splat.setBuffer(splatBuffer);

	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	SoundBuffer gameBuffer;
	gameBuffer.loadFromFile("sound/game.wav");
	Sound gameSound;
	gameSound.setBuffer(gameBuffer);

	SoundBuffer tickBuffer;
	tickBuffer.loadFromFile("sound/tick.wav");
	Sound tickSound;
	tickSound.setBuffer(tickBuffer);

	SoundBuffer gameOverBuffer;
	gameOverBuffer.loadFromFile("sound/game_over.wav");
	Sound game_overSound;
	game_overSound.setBuffer(gameOverBuffer);


#pragma endregion

	while (window.isOpen()) {
		//Event + Control + Keyboad + Mouse
#pragma region Control + Keyboad + Mouse
		if (gameSound.getStatus() != Music::Status::Playing) gameSound.play();
		Event event;
		//Escape
		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

		if (state == State::PLAYING) window.setMouseCursorVisible(false);
		else window.setMouseCursorVisible(true);

		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Enter && state == State::PLAYING) {
					state = State::PAUSE;
					
				}
				else if (event.key.code == Keyboard::Enter && state == State::PAUSE) {
					state = State::PLAYING;
					clock.restart();
				}
				else if (event.key.code == Keyboard::Enter && state == State::GAME_OVER) {
					// Reset the game
					wave = 0;
					score = 0;
					currentBullet = 0;
					bulletSpare = 36;
					bullet_in_Clip = 6;
					clipSize = 6;
					fireRate = 5;
					countDeadZombie = 0;
					player.reset();
					for (int i = 0; i < 100; i++) {
						bullets[i].reset();
					}
					TIME_START = 20;
					countTime = TIME_START;
					state = State::LEVEL_UP;
				}
			}

			// Zoom + Rotate
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				mainView.rotate(5);
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				mainView.rotate(-5);
			}
			if (Keyboard::isKeyPressed(Keyboard::Up) && mainView.getSize().x >= 960 && mainView.getSize().y >= 540) {
				mainView.zoom(0.5);
			}
			if (Keyboard::isKeyPressed(Keyboard::Down) && mainView.getSize().x <= 1920 && mainView.getSize().y <= 1080) {
				mainView.zoom(1.5);
			}
		}


		// Control
		if (state == State::PLAYING) {
			if (Keyboard::isKeyPressed(Keyboard::W)) player.moveUp();
			else player.stopUp();
			if (Keyboard::isKeyPressed(Keyboard::S)) player.moveDown();
			else player.stopDown();
			if (Keyboard::isKeyPressed(Keyboard::A)) player.moveLeft();
			else player.stopLeft();
			if (Keyboard::isKeyPressed(Keyboard::D)) player.moveRight();
			else player.stopRight();

			// Reload bullet
			if (state == State::PLAYING) {
				if (Keyboard::isKeyPressed(Keyboard::R)) {
					if (Reload(bulletSpare, clipSize, bullet_in_Clip)) reload.play();
					else reloadFailed.play();
				}
			}

			if (Keyboard::isKeyPressed(Keyboard::Space) && countDeadZombie >= 5) {
				if ((double)getTimeTotal.asMilliseconds() - lastPressedSpace.asMilliseconds() > 200) {
					if (usePow) {
						usePow = 0;
					}
					else {
						usePow = 1;
					}
					lastPressedSpace = getTimeTotal;
				}
			}

			if (Mouse::isButtonPressed(Mouse::Left)) {
				if ((double)getTimeTotal.asMilliseconds() - lastPressedLeft.asMilliseconds() > 1000.0 / fireRate && bullet_in_Clip > 0) {
					bullets[currentBullet].shoot(player.getCenter().x, player.getCenter().y, mouseWorldPosition.x, mouseWorldPosition.y, countDeadZombie, usePow);
					cout << bullets[currentBullet].getDamage() << endl;
					currentBullet++;
					if (currentBullet > 99) currentBullet = 0;
					lastPressedLeft = getTimeTotal;
					bullet_in_Clip--;
					if (bullet_in_Clip <= 0 && Reload(bulletSpare, clipSize, bullet_in_Clip)) reload.play();
					else reloadFailed.play();
					shoot.play();
				}
			}

			Vector2f playerPosition = player.getCenter();
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive() && zombies[i].getType() == 2) {
					srand((int)time(0) * i);
					if (!zombie_shoot[i].isFlying() && getTimeTotal.asMilliseconds() - timeZombieshoot[i].asMilliseconds() > (rand() % 5 + 3) * 1000) {
						zombie_shoot[i].Zombie_shoot(zombies[i].getCenter().x, zombies[i].getCenter().y, playerPosition.x, playerPosition.y, wave);
						timeZombieshoot[i] = getTimeTotal;

					}
				}
			}

		}
#pragma endregion


		// Level up
#pragma region Level Up
		if (state == State::LEVEL_UP) {
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition());

			//1 - Increased rate of fire
			if (Mouse::isButtonPressed(Mouse::Left) && mouseWorldPosition.x >= 150 && mouseWorldPosition.x <= 1010 && mouseWorldPosition.y >= 265 && mouseWorldPosition.y <= 315) {
				fireRate++;
				typePower = 1;
				state = State::PLAYING;
			}
			//2 - Increased clip size (next reload)
			if (Mouse::isButtonPressed(Mouse::Left) && mouseWorldPosition.x >= 150 && mouseWorldPosition.x <= 1410 && mouseWorldPosition.y >= 345 && mouseWorldPosition.y <= 400) {
				clipSize += clipSize;
				typePower = 2;
				state = State::PLAYING;
			}
			//3 - Increased max health
			if (Mouse::isButtonPressed(Mouse::Left) && mouseWorldPosition.x >= 150 && mouseWorldPosition.x <= 1030 && mouseWorldPosition.y >= 425 && mouseWorldPosition.y <= 485) {
				player.HealthUp();
				typePower = 3;
				state = State::PLAYING;
			}
			//4 - Increased run speed
			if (Mouse::isButtonPressed(Mouse::Left) && mouseWorldPosition.x >= 150 && mouseWorldPosition.x <= 962 && mouseWorldPosition.y >= 505 && mouseWorldPosition.y <= 570) {
				player.SpeedUp();
				typePower = 4;
				state = State::PLAYING;
			}
			//5
			if (Mouse::isButtonPressed(Mouse::Left) && mouseWorldPosition.x >= 150 && mouseWorldPosition.x <= 1370 && mouseWorldPosition.y >= 585 && mouseWorldPosition.y <= 655) {
				HEALTH_PICKUP.upgrade();
				typePower = 5;
				state = State::PLAYING;
			}
			//6
			if (Mouse::isButtonPressed(Mouse::Left) && mouseWorldPosition.x >= 150 && mouseWorldPosition.x <= 1335 && mouseWorldPosition.y >= 665 && mouseWorldPosition.y <= 740) {
				AMMO_PICKUP.upgrade();
				typePower = 6;
				state = State::PLAYING;
			}

			//7
			if (Mouse::isButtonPressed(Mouse::Left) && mouseWorldPosition.x >= 150 && mouseWorldPosition.x <= 1250 && mouseWorldPosition.y >= 745 && mouseWorldPosition.y <= 825) {
				for (int i = 0; i < 100; i++) {
					bullets[i].damUp();
				}
				typePower = 7;
				state = State::PLAYING;
			}


			//Setting Arena
			if (state == State::PLAYING) {
				wave++;
				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);

				player.spawn(arena, resolution, tileSize);
				HEALTH_PICKUP.setArena(arena);
				AMMO_PICKUP.setArena(arena);
				CLOCK_PICKUP.setArena(arena);
				numZombies = 5 * wave;
				numZombiesAlive = numZombies;
				delete[]zombies;
				delete[]zombie_shoot;
				delete[]timeZombieshoot;
				zombies = createHorde(numZombies, arena, zombie_shoot, timeZombieshoot);
				powerup.play();
				clock.restart();
			}
		}
#pragma endregion

		// Update
#pragma region Update

		if (state == State::PLAYING) {
			Time dt = clock.restart();

			//Crosschair
			crosschair.setPosition(mouseWorldPosition);

			getTimeTotal += dt;
			double dt_Sec = dt.asSeconds();

			// mouse
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			// player
			player.update(dt_Sec, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());

			//view
			mainView.setCenter(player.getCenter());

			for (int i = 0; i < 100; i++) {
				if (bullets[i].isFlying()) {
					bullets[i].update(dt_Sec, arena);
				}
			}
			//zombie
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			//bullet + bullet
			for (int i = 0; i < numZombies; i++) {
				
				for (int j = 0; j < 100; j++) {
					if (zombie_shoot[i].getPosition().intersects(bullets[j].getPosition()) && zombie_shoot[i].isFlying() && bullets[j].isFlying()) {
						zombie_shoot[i].stop();
						if (bullets[j].getPosition().width < 20) bullets[j].stop();
					}
				}
			}

			//zombie-shoot
			for (int i = 0; i < numZombies; i++) {
				if (zombie_shoot[i].isFlying() && zombie_shoot[i].getPosition().intersects(player.getPosition())) {
					zombie_shoot[i].stop();
					if (player.hit(getTimeTotal)) {
						hit.play();
					}
					else if (player.getHealth() <= 0) {
						state = State::GAME_OVER;
						game_overSound.play();
					}
				}
				
					zombie_shoot[i].update(dt_Sec, arena);
				
			}


			// bullet vs zombie
			// player's bullet
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++) {
					if (bullets[i].getPosition().intersects(zombies[j].getPosition()) && bullets[i].isFlying() && zombies[j].isAlive()) {
						if (bullets[i].getPosition().width < 20) bullets[i].stop();
						if (zombies[j].hit(bullets[i].getDamage())) {
							score += 10;
							if (score > HiScore) {
								ofstream fo("data.txt", ios::out);
								if (fo.is_open()) {
									fo << score;
									fo.close();
								}
								HiScore = score;
							}

							numZombiesAlive--;
							if (bullets[i].getPosition().width < 20 && countDeadZombie < 5) countDeadZombie++;

							if (numZombiesAlive <= 0) {
								// Finish/win a wave
								state = State::LEVEL_UP;
								TIME_START += 10;
								countTime = TIME_START;
								AMMO_PICKUP.upgrade();
								HEALTH_PICKUP.upgrade();
								CLOCK_PICKUP.upgrade();
								player.SpeedUp();
							}
						}
						splat.play();
					}
				}

			}

			// zombie_vs_player
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive() && zombies[i].getPosition().intersects(player.getPosition())) {
					if (player.hit(getTimeTotal)) {
						hit.play();
					}
					else if (player.getHealth() <= 0) {
						state = State::GAME_OVER;
						game_overSound.play();
					}
				}
			}

			// pick up
			if (player.getPosition().intersects(HEALTH_PICKUP.getPosition()) && HEALTH_PICKUP.isSpawned()) {
				player.IncreaseHealthLevel(HEALTH_PICKUP.got());
				pickup.play();
			}
			if (player.getPosition().intersects(AMMO_PICKUP.getPosition()) && AMMO_PICKUP.isSpawned()) {
				bulletSpare += AMMO_PICKUP.got();
				pickup.play();
			}
			if (player.getPosition().intersects(CLOCK_PICKUP.getPosition()) && CLOCK_PICKUP.isSpawned()) {
				countTime += CLOCK_PICKUP.got();
				pickup.play();
			}

			HEALTH_PICKUP.update(dt_Sec);
			AMMO_PICKUP.update(dt_Sec);
			CLOCK_PICKUP.update(dt_Sec);

			// health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			frames_Since_LastHUD_Update++;

			// Time game
			if (getTimeTotal.asSeconds() - GameTime.asSeconds() > 1) {
				GameTime = getTimeTotal;
				countTime--;
				if (countTime <= 0) {
					state = State::GAME_OVER;
					game_overSound.play();
				}
			}


			if (frames_Since_LastHUD_Update > fpsFrameInterval) {
				stringstream ssAmmo;
				stringstream ssScore;
				stringstream ssHiscore;
				stringstream ssWave;
				stringstream ssZombiesAlive;
				stringstream ssPower;
				stringstream ssTime;

				ssAmmo << bullet_in_Clip << '/' << bulletSpare;
				AmmoText.setString(ssAmmo.str());

				ssScore << "Score: " << score << " / " << HiScore;
				ScoreText.setString(ssScore.str());

				ssHiscore << "HIScore: " << HiScore;
				HiScoreText.setString(ssHiscore.str());

				ssWave << "Wave: " << wave;
				WaveNumberText.setString(ssWave.str());

				ssZombiesAlive << numZombiesAlive;
				ZombiesRemainingText.setString(ssZombiesAlive.str());
				frames_Since_LastHUD_Update = 0;

				ssPower << "Power: " << typePower;
				PowerText.setString(ssPower.str());

				ssTime << countTime;
				if (countTime <= 5) {
					TimeText.setFillColor(Color{ 252, 69, 43 });
					tickSound.play();
				}
				else TimeText.setFillColor(Color{ 54, 170, 43 });
				TimeText.setString(ssTime.str());
			}
		}

#pragma endregion

		//Draw
#pragma region Draw
		window.clear();

		// Playing
		if (state == State::PLAYING) {
			// main view
			window.setView(mainView);
			window.draw(background, &txtBackground);
			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			window.draw(player.getSprite());
			window.draw(crosschair);
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isFlying()) window.draw(bullets[i].getShape());
			}

			if (HEALTH_PICKUP.isSpawned()) {
				window.draw(HEALTH_PICKUP.getSprite());
			}
			if (AMMO_PICKUP.isSpawned()) {
				window.draw(AMMO_PICKUP.getSprite());
			}
			if (CLOCK_PICKUP.isSpawned()) {
				window.draw(CLOCK_PICKUP.getSprite());
			}

			for (int i = 0; i < numZombies; i++) {
				if (zombie_shoot[i].isFlying()) {
					window.draw(zombie_shoot[i].getShape());
				}
			}

			// HUD view 
			window.setView(HUD_view);
			window.draw(TimeText);
			window.draw(AmmoText);
			window.draw(ScoreText);
			window.draw(ZombiesRemainingText);
			window.draw(healthBar);
			window.draw(WaveNumberText);
			window.draw(PowerText);
			for (int i = 0; i < countDeadZombie; i++) {
				if (usePow && bullet_in_Clip > 0) deadZombie[i].setOutlineColor(Color{ 221,244,125 });
				else deadZombie[i].setOutlineColor(Color{ 0,59,54 });
				window.draw(deadZombie[i]);
			}

			// Icon
			window.draw(AmmoIcon);
			window.draw(HealthIcon);
			window.draw(PowerIcon);
			window.draw(ClockIcon);
			window.draw(ZombieIcon);

		}

		// Pause
		if (state == State::PAUSE) {
			window.draw(PauseText);
			window.draw(ExitText);
			if (Keyboard::isKeyPressed(Keyboard::E)) {
				state = State::GAME_OVER;
			}
		}

		// Game over
		if (state == State::GAME_OVER) {
			window.draw(backgroundGameOver);
			window.draw(GameOverText);
			window.draw(HiScoreText);
		}
		if (state == State::LEVEL_UP) {
			window.draw(backgroundGameOver);
			window.draw(LevelUpText);
		}

		if (game_overSound.getStatus() == Music::Playing) gameSound.setVolume(10);
		else gameSound.setVolume(100);
		window.display();
#pragma endregion


	}
	delete[] zombies;
	return 0;
}