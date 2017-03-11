#include "World.h"
#include "Entity.h"
#include "Input.h"
#include "Sound.h"

World* World::instance = nullptr;

World::World()
{
}


World::~World()
{
	for (auto& entity : m_entity)
	{
		delete entity;
	}
}


World& World::GetInstance()
{
	// Checks if there is an instance, and creates a new instance.
	if (instance == nullptr)
	{
		instance = new World();
	}

	return *instance;
}

// Function to initialise the world, adding sprites, images and background to the world.
bool World::Initialise(unsigned int width, unsigned int height)
{
	if (!VIS.Initialise(width, height)) // Initalise screen.
	{
		return false;
	}

	if (!SOUND.Play("Music.wav"))
	{
		return false;
	}

	int spriteIndex;
	int enemyIndex;
	int bulletIndex;
	int enemybulletIndex;
	int explosionIndex;
	VIS.SetBackground("background.jpg"); // Load background image.
	HAPI.SetShowFPS(true, 20, 500);
	VIS.LoadSprite("spaceship.png", spriteIndex); // Load player image.
	VIS.LoadSprite("enemyship.png", enemyIndex); // Load Enemy image.
	VIS.LoadSprite("projectile.png", bulletIndex);
	VIS.LoadSprite("enemyprojectile.png", enemybulletIndex);
	VIS.LoadSprite("explosion.png", explosionIndex);
	PlayerEntity* player = new PlayerEntity(spriteIndex, "player"); // Create player entity.
	player->SetActive();
	EnemyEntity* enemy = new EnemyEntity(enemyIndex, "enemy"); // Create enemy entity.
	enemy->SetActive();
	ExplosionEntity* explosion = new ExplosionEntity(explosionIndex, "explode");
	AddEntity(player); // Adding entity.
	AddEntity(enemy); // Adding entity.
	AddEntity(explosion);
	EnemyBulletEntity* enemybullet = new EnemyBulletEntity(enemybulletIndex, "enemybullet");
	AddEntity(enemybullet);

	for (int index = 0; index < 10; index++)
	{
		BulletEntity* bullet = new BulletEntity(bulletIndex, "bullet");
		AddEntity(bullet);
	}
	return true;
}

// Function to start the updating from other classes and update entities.
void World::AddEntity(Entity* entity)
{
	m_entity.push_back(entity);
}

// Function to start the updating from other classes and update entities.
void World::Start()
{
	while (HAPI.Update())
	{
		INPUT.Update(); // Calls Input update function.
		VIS.Update(); // Calls Visualisation update function.

		UpdateTime();

		HAPI.RenderText(10, 10, HAPI_TColour(0, 255, 0), ("Score : ") + std::to_string(score), 30);
		HAPI.RenderText(10, 30, HAPI_TColour(0, 255, 0), ("Lives : ") + std::to_string(lives), 30);

		// Calls entity to update and blit.
		for (auto& entity : m_entity)
		{
			entity->Update();
		}

		for (auto& entity : m_entity)
		{
			entity->Blit();
		}

		for (auto& entity : m_entity)
		{
			// Creating projectile firing for enemy.
			if (entity->GetTag() == "enemybullet" && entity->GetActive() == true && entity->GetPosition().y <= 0)
			{
				if (entity->GetActive())
					entity->SetActive();
			}

			if (entity->GetTag() == "enemybullet" && entity->GetActive() == false && m_enemyfireI <= 0)
			{
 				entity->SetPosition(m_entity[1]->GetPosition());
				m_enemyfireI = 0.5f; // Setting enemy firing interval.
				entity->SetActive();
				break;
			}
		}

		for (auto& entity : m_entity)
		{
			// Creating projectile firing for player.
			if (entity->GetTag() == "bullet" && entity->GetActive() == true && entity->GetPosition().y <= 0)
			{
				if(entity->GetActive())
					entity->SetActive();
			}
		
			Vector offset{3,0};
			if (entity->GetTag() == "bullet" && entity->GetActive() == false && INPUT.GetKey(HK_SPACE) && m_fireI <= 0)
			{
				entity->SetPosition(m_entity[0]->GetPosition() + offset);
				m_fireI = 0.5f;
				entity->SetActive();
				break;
			}
		}
		m_fireI -= delta_time;
		m_enemyfireI -= delta_time;
	}
}

// Function to delete the instance of world and stop input from other classes.
void World::Stop()
{
	if (instance)
	{
		delete instance;
	}
	VIS.Stop(); // Calls Visualisation stop function.
	INPUT.Stop(); // Calls Input stop function.
	SOUND.Stop();
}

void World::UpdateTime()
{
	delta_time = (float)(HAPI.GetTime() - current_time) / 1000;

	if (delta_time >= 1)
	{
		delta_time = 1;
	}

	current_time = (float)HAPI.GetTime();
}