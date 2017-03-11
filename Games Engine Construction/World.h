#pragma once
#include "Visualisation.h"
#include <vector>
#define WORLD World::GetInstance()

class Entity; // Foward declaration of the entity class.

class World
{	
public:
	World();
	~World();
	static World& GetInstance(); // Function to create new instance of World.
	bool Initialise(unsigned int width, unsigned int height); // Function to initialise the world, adding sprites, images and background to the world.
	void AddEntity(Entity* entity); // Function to create new entities within the world.
	void Start(); // Function to start the updating from other classes and update entities.
	void Stop(); // Function to delete the instance of world and stop input from other classes.
	void UpdateTime();

	void AddScore(int value) { score += value; }
	void setLives(int value) { lives = value; }

	int getLives() { return lives; }
	

	float GetDeltaTime() const { return delta_time; }
	Entity* GetEntity(int index) { return m_entity[index]; }

private:
	static World* instance; // Only instance of World allowed to exist.
	std::vector<Entity*> m_entity; // Creating a vector of Entity class.
	float m_fireI{ 0.5f };
	float m_enemyfireI{ 0.5f };
	int lives{ 3 };
	int score{ 0 };
	float delta_time;
	float current_time;
};

