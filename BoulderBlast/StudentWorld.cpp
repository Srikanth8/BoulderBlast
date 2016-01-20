#include "StudentWorld.h"
#include <string>
#include <sstream> 
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{
	m_bonusPoints = 1000;
}

int StudentWorld::getWhatIsAt(int x, int y)
{
	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
			return m_actorVector[i]->getID();
	}

	if (m_player->getX() == x && m_player->getY() == y)
		return m_player->getID();
}

Player* StudentWorld::getPlayer()
{
	return m_player;
}

bool StudentWorld::canPlayerMoveThere(int x, int y, int moveDirection)
{
	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
		{
			switch (m_actorVector[i]->getID())
			{
			case IID_WALL:
			case IID_HOLE:
			case IID_ROBOT_FACTORY:
			case IID_SNARLBOT:
			case IID_KLEPTOBOT:
			case IID_ANGRY_KLEPTOBOT:
				return false;
				break;
			case IID_BOULDER:
				switch (moveDirection)
				{
				case KEY_PRESS_UP:
					if (canBouldersMoveThere(x, y + 1))
					{
						m_actorVector[i]->moveTo(x, y + 1);
						return true;
					}
					else
						return false;
					break;
				case KEY_PRESS_DOWN:
					if (canBouldersMoveThere(x, y - 1))
					{
						m_actorVector[i]->moveTo(x, y - 1);
						return true;
					}
					else
						return false;
					break;
				case KEY_PRESS_LEFT:
					if (canBouldersMoveThere(x - 1, y))
					{
						m_actorVector[i]->moveTo(x - 1, y);
						return true;
					}
					else
						return false;
					break;
				case KEY_PRESS_RIGHT:
					if (canBouldersMoveThere(x + 1, y))
					{
						m_actorVector[i]->moveTo(x + 1, y);
						return true;
					}
					else
						return false;
					break;
				}
			}
		}
	}
	return true;
}

bool StudentWorld::canBouldersMoveThere(int x, int y)
{
	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
		{
			switch (m_actorVector[i]->getID())
			{
			case IID_WALL:
			case IID_ROBOT_FACTORY:
			case IID_SNARLBOT:
			case IID_KLEPTOBOT:
			case IID_ANGRY_KLEPTOBOT:
			case IID_BOULDER:
			case IID_AMMO:
			case IID_EXTRA_LIFE:
			case IID_RESTORE_HEALTH:
			case IID_EXIT:
			case IID_JEWEL:
				return false;
				break;
			}
		}
	}
	return true;
}

bool StudentWorld::isBoulderOnHole(int x, int y)
{
	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y && m_actorVector[i]->getID() == IID_BOULDER)
		{
			m_actorVector[i]->setAlive(false);
			return true;
		}
	}
	return false;
}

bool StudentWorld::isPlayerOnMe(int x, int y)
{
	if (m_player->getX() == x && m_player->getY() == y)
		return true;
	else
		return false;
}

void StudentWorld::createBulletAt(int x, int y, int direction)
{
	Bullet* newBullet = new Bullet(x, y, this, direction);
	m_actorVector.insert(m_actorVector.begin(), newBullet);
}

bool StudentWorld::canBulletDamage(int x, int y)
{
	if (m_player->getX() == x && m_player->getY() == y)
	{
		m_player->takeDamage(); 
		return true;
	}

	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
		{
			switch (m_actorVector[i]->getID())
			{
			case IID_SNARLBOT:
			case IID_KLEPTOBOT:
			case IID_ANGRY_KLEPTOBOT:
			case IID_BOULDER:
				m_actorVector[i]->takeDamage();
				return true;
				break;
			}
		}
	}
	return false;
}

bool StudentWorld::canBulletDie(int x, int y)
{
	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
		{
			switch (m_actorVector[i]->getID())
			{
			case IID_WALL:
			case IID_ROBOT_FACTORY:
				return true;
				break;
			}
		}
	}
	return false;
}

bool StudentWorld::isPlayerInMyLOS(int x, int y, int direction)
{
	while (m_player->getX() != x || m_player->getY() != y)
	{
		switch (direction)
		{
		case KEY_PRESS_UP:
			y++;
			break;
		case KEY_PRESS_DOWN:
			y--;
			break;
		case KEY_PRESS_LEFT:
			x--;
			break;
		case KEY_PRESS_RIGHT:
			x++;
			break;
		}

		if (x == 0 || y == 0 || x == VIEW_WIDTH || y == VIEW_HEIGHT)
			return false;

		for (int i = 0; i < m_actorVector.size(); i++)
		{
			if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
			{
				switch (m_actorVector[i]->getID())
				{
				case IID_BOULDER:
				case IID_ANGRY_KLEPTOBOT:
				case IID_KLEPTOBOT:
				case IID_SNARLBOT:
				case IID_ROBOT_FACTORY:
				case IID_WALL:
					return false;
				}
			}
		}
	}
	return true;
}

bool StudentWorld::canRobotMoveThere(int x, int y)
{
	if (m_player->getX() == x && m_player->getY() == y)
		return false;

	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
		{
			switch (m_actorVector[i]->getID())
			{
			case IID_WALL:
			case IID_HOLE:
			case IID_ROBOT_FACTORY:
			case IID_SNARLBOT:
			case IID_KLEPTOBOT:
			case IID_ANGRY_KLEPTOBOT:
			case IID_BOULDER:
				return false;
				break;
			}
		}
	}
	return true;
}

bool StudentWorld::stealGoodieIfPossible(int x, int y, Kleptobot* ptr)
{
	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getX() == x && m_actorVector[i]->getY() == y)
		{
			Actor* a_ptr = m_actorVector[i];
			Goodie* g_ptr = dynamic_cast<Goodie*>(a_ptr);
			if (g_ptr != nullptr && g_ptr->getID() != IID_JEWEL && rand() % 10 == 1)
			{
				ptr->setStolenGoodie(g_ptr->getID());
				g_ptr->setAlive(false);
				return true;
			}
		}
	}
	return false;
}

void StudentWorld::createGoodieAt(int x, int y, int ID)
{
	switch (ID)
	{
	case IID_RESTORE_HEALTH:
		m_actorVector.insert(m_actorVector.begin(), new Restore_Health(x, y, this));
		break;
	case IID_EXTRA_LIFE:
		m_actorVector.insert(m_actorVector.begin(), new Extra_Life(x, y, this));
		break;
	case IID_AMMO:
		m_actorVector.insert(m_actorVector.begin(), new Ammo(x, y, this));
		break;
	}
}

void StudentWorld::produceKleptobotIfPossible(int x, int y, std::string identity)
{
	for (int m = 0; m < m_actorVector.size(); m++)
	{
		if (m_actorVector[m]->getX() == x && m_actorVector[m]->getY() == y)
		{
			if (m_actorVector[m]->getID() == IID_KLEPTOBOT || m_actorVector[m]->getID() == IID_ANGRY_KLEPTOBOT)
				return;
		}
	}

	int x1 = x - 3;
	int x2 = x + 3;
	int y1 = y - 3;
	int y2 = y + 3;

	if (x1 < 0)  x1 = 0;
	if (x2 > 14) x2 = 14;
	if (y1 < 0)  y1 = 0;
	if (y2 > 14) y2 = 14;

	int total = 0;

	for (int i = x1; i <= x2; i++)
	{
		for (int j = y1; j <= y2; j++)
		{
			for (int m = 0; m < m_actorVector.size(); m++)
			{
				if (m_actorVector[m]->getX() == i && m_actorVector[m]->getY() == j)
				{
					Actor* a_ptr = m_actorVector[m];
					Kleptobot* k_ptr = dynamic_cast<Kleptobot*>(a_ptr);
					if (k_ptr != nullptr)
					{
						total++;
					}
				}
			}
		}
	}

	if (total < 3)
	{
		if (rand() % 50 == 1)
		{
			if (identity == "normal")
				m_actorVector.push_back(new Kleptobot(x, y, this));
			else if (identity == "angry")
				m_actorVector.push_back(new Angry_Kleptobot(x, y, this));
			playSound(SOUND_ROBOT_BORN);
		}
	}
}

int StudentWorld::init()
{
	Level lev(assetDirectory());

	unsigned int levelNumber = getLevel();
	if (levelNumber > 99)
		return GWSTATUS_PLAYER_WON;

	string ones_digit, tens_digit;

	tens_digit = '0' + (levelNumber / 10);
	ones_digit = '0' + (levelNumber % 10);

	string levelName = "level" + tens_digit + ones_digit + ".dat";

	Level::LoadResult result = lev.loadLevel(levelName);
	if (result == Level::load_fail_file_not_found)
		return GWSTATUS_PLAYER_WON;
	else if (result == Level::load_fail_bad_format)
		return GWSTATUS_LEVEL_ERROR;
	else if (result == Level::load_success)
	{
		for (int x = 0; x < VIEW_WIDTH; x++)
		{
			for (int y = 0; y < VIEW_HEIGHT; y++)
			{
				Level::MazeEntry ge = lev.getContentsOf(x, y); // coordinates
				switch (ge)
				{
				case Level::empty:
					break;
				case Level::exit:
					m_actorVector.push_back(new Exit(x, y, this));
					break;
				case Level::player:
					m_player = new Player(x, y, this);
					break;
				case Level::horiz_snarlbot:
					m_actorVector.push_back(new Snarlbot(x, y, this, KEY_PRESS_RIGHT));
					break;
				case Level::vert_snarlbot:
					m_actorVector.push_back(new Snarlbot(x, y, this, KEY_PRESS_DOWN));
					break;
				case Level::kleptobot_factory:
					m_actorVector.push_back(new Kleptobot_Factory(x, y, this));
					break;
				case Level::angry_kleptobot_factory:
					m_actorVector.push_back(new Angry_Kleptobot_Factory(x, y, this));
					break;
				case Level::wall:
					m_actorVector.push_back(new Wall(x, y, this));
					break;
				case Level::boulder:
					m_actorVector.push_back(new Boulder(x, y, this));
					break;
				case Level::hole:
					m_actorVector.push_back(new Hole(x, y, this));
					break;
				case Level::jewel:
					m_actorVector.push_back(new Jewel(x, y, this));
					break;
				case Level::restore_health:
					m_actorVector.push_back(new Restore_Health(x, y, this));
					break;
				case Level::extra_life:
					m_actorVector.push_back(new Extra_Life(x, y, this));
					break;
				case Level::ammo:
					m_actorVector.push_back(new Ammo(x, y, this));
					break;
				}
			}
		}
	}
	m_bonusPoints = 1000;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
	// Update the Game Status Line
	//updateDisplayText(); // update the score/lives/level text at screen top
	// The term "actors" refers to all robots, the Player, Goodies,
	// Boulders, Jewels, Holes, Bullets, the Exit, etc.
	// Give each actor a chance to do something

	int score = getScore();
	int level = getLevel() + 1;
	int lives = getLives();
	int health = m_player->getHitpoints();
	int ammo = m_player->getAmmo();
	unsigned int bonus = m_bonusPoints;
	// Next, create a string from your statistics, of the form:
	// Score: 0000100 Level: 03 Lives: 3 Health: 70% Ammo: 216 Bonus: 34

	int healthPercent = (100 / 20) * health;

	ostringstream oss;
	oss << "Score: ";
	oss.fill('0');
	oss << setw(7) << score;
	oss << " ";
	oss << "Level: ";
	oss << setw(2) << level;
	oss << " ";
	oss << "Lives: ";
	oss.fill(' ');
	oss << setw(2) << lives;
	oss << " ";
	oss << "Health: ";
	oss << setw(3) << healthPercent;
	oss << "%";
	oss << " ";
	oss << "Ammo: ";
	oss << setw(3) << ammo;
	oss << " ";
	oss << "Bonus: ";
	oss << setw(4) << bonus;

	// Finally, update the display text at the top of the screen with your
	// newly created stats
	setGameStatText(oss.str()); // calls our provided GameWorld::setGameStatText

	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getAlive())
		{
			// ask each actor to do something (e.g. move)
			m_actorVector[i]->doSomething();

			if (!m_player->getAlive())
				return GWSTATUS_PLAYER_DIED;

			//if (getWhatIsAt(m_player->getX(), m_player->getY()) == IID_EXIT) // note to self: may not work if there is bullet on exit
			//increaseScoreAppropriately();
			//return GWSTATUS_FINISHED_LEVEL;
		}
	}

	if (m_player->getAlive())
		m_player->doSomething();

	// Remove newly-dead actors after each tick

	for (int i = 0; i < m_actorVector.size(); i++)
	{
		std::vector<Actor*>::iterator it = m_actorVector.begin() + i;
		if (m_actorVector[i]->getAlive() == false)
		{
			m_actorVector[i]->setVisible(false);
			delete m_actorVector[i];
			m_actorVector.erase(it);
			i--;
		}
	}// delete dead game objects 

	if (m_player->getAlive())
		m_player->doSomething();

	if (m_bonusPoints > 0)
		m_bonusPoints--;	// Reduce the current bonus for the Level by one


	// If the player has collected all of the Jewels on the level, then we
	// must expose the Exit so the player can advance to the next level

	bool jewelPresent = false;
	Exit* ptr = nullptr;
	for (int i = 0; i < m_actorVector.size(); i++)
	{
		if (m_actorVector[i]->getID() == IID_JEWEL)
			jewelPresent = true;
		if (m_actorVector[i]->getID() == IID_EXIT)
			ptr = dynamic_cast<Exit*>(m_actorVector[i]);
	}

	if (!jewelPresent && ptr != nullptr)
	{
		ptr->setVisible(true);
		if (!ptr->getReveal())
			playSound(SOUND_REVEAL_EXIT);
		ptr->setReveal(true);
	}
	// make the exit Active

	// return the proper result
	if (!m_player->getAlive())
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}

	if (getWhatIsAt(m_player->getX(), m_player->getY()) == IID_EXIT && !jewelPresent)
	{
		increaseScore(2000 + m_bonusPoints);
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	/*if (m_player->checkIfPlayerIsOnExit())
	{
	//increaseScoreAppropriately();
	return GWSTATUS_FINISHED_LEVEL;
	}*/

	return GWSTATUS_CONTINUE_GAME;

	// the player hasn’t completed the current level and hasn’t died, so
	// continue playing the current level	
}

void StudentWorld::cleanUp()
{
	delete m_player;

	for (int i = 0; i < m_actorVector.size(); i++)
		delete m_actorVector[i];

	m_actorVector.clear();
}

StudentWorld::~StudentWorld()
{
	cleanUp();
}