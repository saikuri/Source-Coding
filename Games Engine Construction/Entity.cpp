#include "Entity.h"
#include "Input.h"
#include "Visualisation.h"
#include "Screen.h"

Entity::Entity(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
	m_screen = VIS.GetScreen(); // Generating screen.
}

Entity::~Entity()
{
}

Entity::Entity(Vector m_vector)
{
	m_position = m_vector;
}

void Entity::SetPosition(Vector position)
{
	m_position = position;
}

void Entity::SetActive()
{
	if (Active) 
	{
		Active = false;
	}
	else
	{
		Active = true;
	}
}

Entity::Entity()
{
	m_screen = VIS.GetScreen(); // Generating screen.
}

PlayerEntity::PlayerEntity(int spriteIndex, std::string tag)
{
	m_spriteIndex = spriteIndex;
	VIS.GetImageWidth(m_spriteIndex);
	VIS.GetImageHeight(m_spriteIndex);
	m_position.x = 300;
	m_position.y = 800;
	m_tag = tag;
	m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_spriteIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_spriteIndex));
}

// Keyboard and Controller input through instance of Input class.
void PlayerEntity::Update()
{
	if (INPUT.GetKey('W') || INPUT.GetAnalogue(HK_ANALOGUE_LEFT_THUMB_Y) > deadZone)
	{
		m_position.y -= 5;
	}

	if (INPUT.GetKey('S') || INPUT.GetAnalogue(HK_ANALOGUE_LEFT_THUMB_Y) < -deadZone)
	{
		m_position.y += 5;
	}

	if (INPUT.GetKey('A') || INPUT.GetAnalogue(HK_ANALOGUE_LEFT_THUMB_X) < -deadZone)
	{
		m_position.x -= 5;
	}

	if (INPUT.GetKey('D') || INPUT.GetAnalogue(HK_ANALOGUE_LEFT_THUMB_X) > deadZone)
	{
		m_position.x += 5;
	}

	m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_spriteIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_spriteIndex));

	Collision();
}

void PlayerEntity::Blit()
{
	if(Active)
	{
		VIS.ClipSprite(m_spriteIndex, m_position.x, m_position.y, 0); // Blitting player.
	}
}

void PlayerEntity::Collision()
{
	if (m_rec.Intersecting(WORLD.GetEntity(1)->GetRec()))
	{
		m_position.x = 600;
		m_position.y = 800;
	}
}

EnemyEntity::EnemyEntity(int enemyIndex, std::string tag)
{
	m_enemyIndex = enemyIndex;
	m_position.x = 600; // Hard coding enemy x co-ordinate.
	m_position.y = 300; // Hard coding enemy y co-ordinate.
	m_tag = tag;
	m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_enemyIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_enemyIndex));

}

//Randomising enemy position through specific points.
void EnemyEntity::Update()
{
	m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_enemyIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_enemyIndex));

	HAPI.DebugText(std::to_string(m_random));
	switch (m_state)
	{
	case 0:
		// Setting a timer to change the AI behaviour.
		if (timer <= 0)
		{
			timer = MAX_TIMER; // Resetting the timer.
			m_random = ((float)rand() / RAND_MAX) * 5;
			m_state = (State)m_random;
		}
		else
		{
			timer -= WORLD.GetDeltaTime();
		}

		m_position.x--;

		// If AI is reaching edge of screen then change behaviour.
		if (m_position.x == 0)
		{
			//m_state = k_DOWN;
		}
		break;
	case 1:
		// Setting a timer to change the AI behaviour.
		if (timer <= 0)
		{
			timer = MAX_TIMER; // Resetting the timer.
			m_random = ((float)rand() / RAND_MAX) * 5;
			m_state = (State)m_random;
		}
		else
		{
			timer -= WORLD.GetDeltaTime();
		}
		m_position.x++;
		break;
	case 2:
		// Setting a timer to change the AI behaviour.
		if (timer <= 0)
		{
			timer = MAX_TIMER; // Resetting the timer.
			m_random = ((float)rand() / RAND_MAX) * 5;
			m_state = (State)m_random;
		}
		else
		{
			timer -= WORLD.GetDeltaTime();
		}
		m_position.y++;
		
		break;
	case 3:
		// Setting a timer to change the AI behaviour.
		if (timer <= 0)
		{
			timer = MAX_TIMER; // Resetting the timer.
			m_random = ((float)rand() / RAND_MAX) * 5;
			m_state = (State)m_random;
		}
		else
		{
			timer -= WORLD.GetDeltaTime();
		}
		m_position.y--;
		break;
	case 4:
		
		HAPI.DebugText("I'MA FIRING MAH LAZOR");
		
		timer = MAX_TIMER; // Resetting the timer.
		m_random = ((float)rand() / RAND_MAX) * 5;
		m_state = (State)m_random;
	}
}

void EnemyEntity::Blit()
{
	if (Active)
	{
		VIS.ClipSprite(m_enemyIndex, m_position.x, m_position.y, 0); // Blitting enemy.
	}
}

void EnemyEntity::Collision()
{
}

void EnemyEntity::Restart()
{
	HAPI.DebugText("Restart Called");
	m_random = ((float)rand() / RAND_MAX) * 4;
	m_state = (State)m_random;
	m_position.x = ((float)rand() / RAND_MAX) * m_screen->screenWidth;
	m_position.y = ((float)rand() / RAND_MAX) * m_screen->screenHeight / 2;
	
	if (m_position.x < 0)
	{
		m_position.x = 0;
	}

	if (m_position.x > m_screen->screenWidth)
	{
		m_position.x = m_screen->screenWidth;
	}
}

void EnemyEntity::SetShoottimer(float m_time)
{
	shoottimer = m_time;
}

BulletEntity::BulletEntity(int bulletIndex, std::string tag)
{
	m_bulletIndex = bulletIndex;
	m_tag = tag;
	m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_bulletIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_bulletIndex));
}

void BulletEntity::Update()
{
	m_position.y -= 5;
	m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_bulletIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_bulletIndex));
	Collision();
	
	if (!Active)
	{
		m_rec = Rec(0, 0, 0, 0);
	}
}


void BulletEntity::Blit()
{
	if (Active)
	{
		VIS.ClipSprite(m_bulletIndex, m_position.x, m_position.y, 0);
	}
}

void BulletEntity::Collision()
{
	if (m_rec.Intersecting(WORLD.GetEntity(1)->GetRec()))
	{
		WORLD.GetEntity(2)->SetPosition(WORLD.GetEntity(1)->GetPosition());
		WORLD.GetEntity(2)->SetActive();
		((EnemyEntity*)WORLD.GetEntity(1))->Restart();

		WORLD.AddScore(50);
		SetActive();		
	}
}

EnemyBulletEntity::EnemyBulletEntity(int enemybulletIndex, std::string tag)
{
	m_enemybulletIndex = enemybulletIndex;
	m_tag = tag;
	m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_enemybulletIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_enemybulletIndex));
}

void EnemyBulletEntity::Update()
{
	if (Active)
	{
		m_position.y += 5;
		m_rec = Rec(m_position.x, m_position.x + VIS.GetImageWidth(m_enemybulletIndex), m_position.y, m_position.y + VIS.GetImageHeight(m_enemybulletIndex));
		Collision();
		if (m_position.y > m_screen->screenHeight)
		{
			SetActive();
		}
	}
}

void EnemyBulletEntity::Blit()
{
	if (Active)
	{
		VIS.ClipSprite(m_enemybulletIndex, m_position.x, m_position.y, 0);
	}
}

void EnemyBulletEntity::Collision()
{

	if (m_rec.Intersecting(WORLD.GetEntity(0)->GetRec()))
	{
		HAPI.DebugText("COLLISION BITCH");
		WORLD.GetEntity(2)->SetPosition(WORLD.GetEntity(0)->GetPosition());
		WORLD.GetEntity(2)->SetActive();
		WORLD.setLives(WORLD.getLives() - 1);
		
		SetActive();		
	}
}

ExplosionEntity::ExplosionEntity(int explosionIndex, std::string tag)
{
	m_explosionIndex = explosionIndex;
	m_tag = tag;
}

void ExplosionEntity::Update()
{
	if (Active)
	{
		DWORD m_currentTime = HAPI.GetTime();

		if ((m_currentTime - m_time) > m_frameTime)
		{
			m_frame += 1;

			if (m_frame >= 3)
			{
				m_frame = 0;
				SetActive();
			}

			m_time = m_currentTime;
		}
	}
}

void ExplosionEntity::Blit()
{
	if (Active)
	{
		VIS.ClipSpriteSheet(m_explosionIndex, m_position.x, m_position.y, m_frame);
	}
}

void ExplosionEntity::Collision()
{
}
