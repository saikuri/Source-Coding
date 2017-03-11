#pragma once
#include <HAPI_lib.h>
#include "Rec.h"
#include "Vector.h"
#include "World.h"

using namespace HAPISPACE;
struct Screen; // Forward declaration of the screen class.
class Vector;

class Entity
{
public:
	Entity(int x, int y);
	~Entity();
	Entity();
	Entity(Vector m_vector);

	virtual void Update() = 0;
	virtual void Blit() = 0;
	virtual void Collision() = 0;
	Vector GetPosition() const { return m_position; }
	void SetPosition(Vector position);
	void SetActive();
	bool GetActive() { return Active; }
	std::string GetTag() { return m_tag; }
	Rec GetRec() { return m_rec; }

protected:
	int m_spriteIndex{ 0 };
	int m_enemyIndex{ 1 };
	int m_bulletIndex{ 2 };
	int m_enemybulletIndex{ 0 };
	int m_explosionIndex{ 0 };
	int m_frame{ 0 };
	const int m_frameTime = 1;
	DWORD m_time = HAPI.GetTime();
	Screen* m_screen{ nullptr };
	Vector m_position{0,0}; // Position of sprites.
	bool Active{ false };
	std::string m_tag;
	Rec m_rec;
};

class PlayerEntity: public Entity
{
public:
	PlayerEntity(int spriteIndex, std::string tag); // Fills entity with the correct index.

	virtual void Update() override;
	virtual void Blit() override;
	void Collision() override;

private:
	const int rumbleSize{ 350 }; // Creating an area for the sprite to rumble on screen.
	int maxMotor = 65535; // Assigning the value of the rumble.
	const int deadZone{ 20000 }; // Creating a controller deadzone.
	WORD leftMotor = 0; // Storing rumble motors as a word.
	WORD rightMotor = 0; // Storing rumble motors as a word.
};

class EnemyEntity : public Entity
{
public:
	EnemyEntity(int enemyIndex, std::string tag); // Fills entity with the correct index.

	virtual void Update() override;
	virtual void Blit() override;
	void Collision() override;
	void Restart();
	void SetShoottimer(float m_time);
	float GetShoottimer() const { return shoottimer; }

private:
	int m_random = (rand() / RAND_MAX) * 5;
	float timer = 0;
	float shoottimer{ 0 };
	const float MAX_TIMER = 1.5f;
	enum State
	{
		k_LEFT = 0,
		k_RIGHT = 1,
		k_DOWN = 2,
		k_FIRE = 3
	};

	State m_state{ k_DOWN };

};

class BulletEntity : public Entity
{
public:
	BulletEntity(int bulletIndex, std::string tag);

	virtual void Update() override;
	virtual void Blit() override;
	void Collision() override;

private:
};

class EnemyBulletEntity : public Entity
{
public:
	EnemyBulletEntity(int enemybulletIndex, std::string tag);

	virtual void Update() override;
	virtual void Blit() override;
	void Collision() override;

};

class ExplosionEntity : public Entity
{
public:
	ExplosionEntity(int explosionIndex, std::string tag);

	virtual void Update() override;
	virtual void Blit() override;
	void Collision() override;

};

