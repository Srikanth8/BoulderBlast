#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//////////////////////////
//ACTOR IMPLEMENTATIONS///
//////////////////////////

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* studentWorld, int hitpoints)
	: GraphObject(imageID, startX, startY, startDirection)
{
	m_ID = imageID;
	m_studentWorld = studentWorld;
	m_alive = true;
	m_hitpoints = hitpoints;
}

void Actor::takeDamage()
{
	m_hitpoints = m_hitpoints - 2;

	if (m_hitpoints <= 0)
		return;

	if (getID() == IID_PLAYER)
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
	else if (getID() == IID_SNARLBOT || getID() == IID_KLEPTOBOT || getID() == IID_ANGRY_KLEPTOBOT)
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

void Actor::restoreHitpoints()
{
	m_hitpoints = 20;
}

void Actor::createBullet()
{
	switch (getDirection())
	{
	case up:
		getWorld()->createBulletAt(getX(), getY() + 1, KEY_PRESS_UP);
		break;
	case down:
		getWorld()->createBulletAt(getX(), getY() - 1, KEY_PRESS_DOWN);
		break;
	case left:
		getWorld()->createBulletAt(getX() - 1, getY(), KEY_PRESS_LEFT);
		break;
	case right:
		getWorld()->createBulletAt(getX() + 1, getY(), KEY_PRESS_RIGHT);
		break;
	}
}

int Actor::getID() const
{
	return m_ID;
}

int Actor::getHitpoints() const
{
	return m_hitpoints;
}

bool Actor::getAlive() const
{
	return m_alive;
}

void Actor::setAlive(bool status)
{
	m_alive = status;
}

StudentWorld* Actor::getWorld() const
{
	return m_studentWorld;
}

//////////////////////////
//PLAYER IMPLEMENTATIONS//
//////////////////////////

Player::Player(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints, int ammo)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(true);
	m_ammo = ammo;
}

void Player::doSomething()
{
	if (!getAlive())
		return;

	if (getHitpoints() <= 0)
	{
		getWorld()->playSound(SOUND_PLAYER_DIE);
		setAlive(false);
	}

	int ch;
	if (getWorld()->getKey(ch))
	{
		switch (ch)
		{
		case KEY_PRESS_UP:
			setDirection(up);
			if (getWorld()->canPlayerMoveThere(getX(), getY() + 1, KEY_PRESS_UP))
				moveTo(getX(), getY() + 1);
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (getWorld()->canPlayerMoveThere(getX(), getY() - 1, KEY_PRESS_DOWN))
				moveTo(getX(), getY() - 1);
			break;
		case KEY_PRESS_LEFT:
			setDirection(left);
			if (getWorld()->canPlayerMoveThere(getX() - 1, getY(), KEY_PRESS_LEFT))
				moveTo(getX() - 1, getY());
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (getWorld()->canPlayerMoveThere(getX() + 1, getY(), KEY_PRESS_RIGHT))
				moveTo(getX() + 1, getY());
			break;
		case KEY_PRESS_SPACE:
			if (m_ammo != 0)
			{
				createBullet();
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				m_ammo--;
			}
			break;
		case KEY_PRESS_ESCAPE:
			setAlive(false);
			break;
		}
	}
}

int  Player::getAmmo()
{
	return m_ammo;
}

void Player::increaseAmmo()
{
	m_ammo += 20;
}

//////////////////////////
//WALL IMPLEMENTATIONS////
//////////////////////////

Wall::Wall(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(true);
}

void Wall::doSomething() {}

//////////////////////////
//EXIT IMPLEMENTATIONS////
//////////////////////////

Exit::Exit(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(false);
	setReveal(false);
}

void Exit::doSomething() {}

void Exit::setReveal(bool status)
{
	m_haveIBeenRevealed = status;
}

bool Exit::getReveal()
{
	return m_haveIBeenRevealed;
}

///////////////////////////
//BOULDER IMPLEMENTATIONS//
///////////////////////////

Boulder::Boulder(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(true);
}

void Boulder::doSomething()
{
	if (!getAlive())
		return;

	if (getHitpoints() <= 0)
		setAlive(false);
}

//////////////////////////
//HOLE IMPLEMENTATIONS////
//////////////////////////

Hole::Hole(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(true);
}

void Hole::doSomething()
{
	if (!getAlive())
		return;

	if (getWorld()->isBoulderOnHole(getX(), getY()))
		setAlive(false);
}

///////////////////////////
//BULLETS IMPLEMENTATIONS//
///////////////////////////

Bullet::Bullet(int startX, int startY, StudentWorld* studentWorld, int startDirection_no, int imageID, int hitpoints)
	: Actor(imageID, startX, startY, none, studentWorld, hitpoints)
{
	switch (startDirection_no)
	{
	case KEY_PRESS_UP:
		setDirection(up);
		break;
	case KEY_PRESS_DOWN:
		setDirection(down);
		break;
	case KEY_PRESS_LEFT:
		setDirection(left);
		break;
	case KEY_PRESS_RIGHT:
		setDirection(right);
		break;
	}
	setVisible(true);
}

void Bullet::doSomething()
{
	if (!getAlive())
		return;

	if (getWorld()->canBulletDamage(getX(), getY()) || getWorld()->canBulletDie(getX(), getY()))
	{
		setAlive(false);
		return;
	}

	switch (getDirection())
	{
	case up:
		moveTo(getX(), getY() + 1);
		break;
	case down:
		moveTo(getX(), getY() - 1);
		break;
	case left:
		moveTo(getX() - 1, getY());
		break;
	case right:
		moveTo(getX() + 1, getY());
		break;
	}
}

///////////////////////////
//GOODIE IMPLEMENTATIONS///
///////////////////////////

Goodie::Goodie(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(true);
}

void Goodie::doSomething()
{
	if (!getAlive())
		return;
}

void Goodie::takeGoodieAndPlaySound()
{
	getWorld()->playSound(SOUND_GOT_GOODIE);
	setAlive(false);
}

//////////////////////////
//JEWEL IMPLEMENTATIONS///
//////////////////////////

Jewel::Jewel(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Goodie(startX, startY, studentWorld, imageID, startDirection, hitpoints) {}

void Jewel::doSomething()
{
	Goodie::doSomething();

	if (getWorld()->isPlayerOnMe(getX(), getY()))
	{
		getWorld()->increaseScore(50);
		takeGoodieAndPlaySound();
	}
}

//////////////////////////////////
//RESTORE_HEALTH IMPLEMENTATIONS//
//////////////////////////////////

Restore_Health::Restore_Health(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Goodie(startX, startY, studentWorld, imageID, startDirection, hitpoints) {}

void Restore_Health::doSomething()
{
	Goodie::doSomething();

	if (getWorld()->isPlayerOnMe(getX(), getY()))
	{
		getWorld()->increaseScore(500);
		getWorld()->getPlayer()->restoreHitpoints();
		takeGoodieAndPlaySound();
	}
}

///////////////////////////////
//EXTRA_LIFE IMPLEMENTATIONS///
///////////////////////////////

Extra_Life::Extra_Life(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Goodie(startX, startY, studentWorld, imageID, startDirection, hitpoints) {}

void Extra_Life::doSomething()
{
	Goodie::doSomething();

	if (getWorld()->isPlayerOnMe(getX(), getY()))
	{
		getWorld()->increaseScore(1000);
		getWorld()->incLives();
		takeGoodieAndPlaySound();
	}
}

//////////////////////////
//AMMO IMPLEMENTATIONS////
//////////////////////////

Ammo::Ammo(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Goodie(startX, startY, studentWorld, imageID, startDirection, hitpoints) {}

void Ammo::doSomething()
{
	Goodie::doSomething();

	if (getWorld()->isPlayerOnMe(getX(), getY()))
	{
		getWorld()->increaseScore(100);
		getWorld()->getPlayer()->increaseAmmo();
		takeGoodieAndPlaySound();
	}
}

//////////////////////////
//ROBOT IMPLEMENTATIONS///
//////////////////////////

Robot::Robot(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(true);
	m_ticks = 1;
}

bool Robot::shouldIdoSomething()
{
	int ticksToWait = (28 - getWorld()->getLevel()) / 4;

	if (ticksToWait < 3)
		ticksToWait = 3;

	if (m_ticks == ticksToWait)
	{
		m_ticks = 0;
		return true;
	}
	return false;
}

bool Robot::shootIfInLOS()
{
	int direction;

	switch (getDirection())
	{
	case up:
		direction = KEY_PRESS_UP;
		break;
	case down:
		direction = KEY_PRESS_DOWN;
		break;
	case left:
		direction = KEY_PRESS_LEFT;
		break;
	case right:
		direction = KEY_PRESS_RIGHT;
		break;
	}

	if (getWorld()->isPlayerInMyLOS(getX(), getY(), direction))
	{
		createBullet();
		return true;
	}
	return false;
}

void Robot::increaseTicks()
{
	m_ticks++;
}

/////////////////////////////
//SNARLBOT IMPLEMENTATIONS///
/////////////////////////////

Snarlbot::Snarlbot(int startX, int startY, StudentWorld* studentWorld, int startDirection_no, int imageID, int hitpoints)
	: Robot(startX, startY, studentWorld, imageID, right, hitpoints)
{
	if (startDirection_no == KEY_PRESS_DOWN)
		setDirection(down);
}

void Snarlbot::doSomething()
{
	if (!getAlive())
		return;

	if (getHitpoints() <= 0)
	{
		getWorld()->increaseScore(100);
		getWorld()->playSound(SOUND_ROBOT_DIE);
		setAlive(false);
	}

	increaseTicks();

	if (!shouldIdoSomething())
		return;

	if (shootIfInLOS())
	{
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		return;
	}

	switch (getDirection())
	{
	case up:
		if (getWorld()->canRobotMoveThere(getX(), getY() + 1))
			moveTo(getX(), getY() + 1);
		else
			setDirection(down);
		break;
	case down:
		if (getWorld()->canRobotMoveThere(getX(), getY() - 1))
			moveTo(getX(), getY() - 1);
		else
			setDirection(up);
		break;
	case left:
		if (getWorld()->canRobotMoveThere(getX() - 1, getY()))
			moveTo(getX() - 1, getY());
		else
			setDirection(right);
		break;
	case right:
		if (getWorld()->canRobotMoveThere(getX() + 1, getY()))
			moveTo(getX() + 1, getY());
		else
			setDirection(left);
		break;
	}
}

/////////////////////////////
//KLEPTOBOT IMPLEMENTATIONS//
/////////////////////////////

Kleptobot::Kleptobot(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Robot(startX, startY, studentWorld, imageID, startDirection, hitpoints)
{
	m_distanceBeforeTurning = randStraightLine();
	m_distance = 0;
	m_stolenGoodie = -1;
}

void Kleptobot::doSomething()
{
	if (!getAlive())
		return;

	if (getHitpoints() <= 0)
	{
		if (m_stolenGoodie != -1)
			getWorld()->createGoodieAt(getX(), getY(), m_stolenGoodie);

		if (getID() == IID_KLEPTOBOT)
			getWorld()->increaseScore(10);

		if (getID() == IID_ANGRY_KLEPTOBOT)
			getWorld()->increaseScore(20);

		getWorld()->playSound(SOUND_ROBOT_DIE);
		setAlive(false);
	}

	increaseTicks();

	if (!shouldIdoSomething())
		return;

	if (getID() == IID_ANGRY_KLEPTOBOT && shootIfInLOS())
	{
		getWorld()->playSound(SOUND_ENEMY_FIRE);
		return;
	}

	if (m_stolenGoodie == -1)
	{
		if (getWorld()->stealGoodieIfPossible(getX(), getY(), this))
		{
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			return;
		}
	}

	bool hasMoven = false;
	if (m_distance < m_distanceBeforeTurning)
	{
		switch (getDirection())
		{
		case up:
			if (getWorld()->canRobotMoveThere(getX(), getY() + 1))
			{
				moveTo(getX(), getY() + 1);
				hasMoven = true;
			}
			break;
		case down:
			if (getWorld()->canRobotMoveThere(getX(), getY() - 1))
			{
				moveTo(getX(), getY() - 1);
				hasMoven = true;
			}
			break;
		case left:
			if (getWorld()->canRobotMoveThere(getX() - 1, getY()))
			{
				moveTo(getX() - 1, getY());
				hasMoven = true;
			}
			break;
		case right:
			if (getWorld()->canRobotMoveThere(getX() + 1, getY()))
			{
				moveTo(getX() + 1, getY());
				hasMoven = true;
			}
			break;
		}

		if (hasMoven)
		{
			m_distance++;
			return;
		}
	}

	m_distance = 0;
	m_distanceBeforeTurning = randStraightLine();

	std::set<int> directionSet;
	int dir_fixed = randDirection();
	int dir = dir_fixed;

	while (directionSet.size() != 4)
	{
		switch (dir)
		{
		case KEY_PRESS_UP:
			if (getWorld()->canRobotMoveThere(getX(), getY() + 1))
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
				return;
			}
			else
				directionSet.insert(KEY_PRESS_UP);

			break;
		case KEY_PRESS_DOWN:
			if (getWorld()->canRobotMoveThere(getX(), getY() - 1))
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
				return;
			}
			else
				directionSet.insert(KEY_PRESS_DOWN);

			break;
		case KEY_PRESS_LEFT:
			if (getWorld()->canRobotMoveThere(getX() - 1, getY()))
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
				return;
			}
			else
				directionSet.insert(KEY_PRESS_LEFT);

			break;
		case KEY_PRESS_RIGHT:
			if (getWorld()->canRobotMoveThere(getX() + 1, getY()))
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
				return;
			}
			else
				directionSet.insert(KEY_PRESS_RIGHT);

			break;
		}
		dir = randDirection();
	}

	switch (dir_fixed)
	{
	case KEY_PRESS_UP: setDirection(up); break;
	case KEY_PRESS_DOWN: setDirection(down); break;
	case KEY_PRESS_LEFT: setDirection(left); break;
	case KEY_PRESS_RIGHT: setDirection(right); break;
	}
}

int Kleptobot::randStraightLine()
{
	return (rand() % 6) + 1;
}

int Kleptobot::randDirection()
{
	switch (rand() % 4 + 1)
	{
	case 1:
		return KEY_PRESS_UP;
		break;
	case 2:
		return KEY_PRESS_DOWN;
		break;
	case 3:
		return KEY_PRESS_LEFT;
		break;
	case 4:
		return KEY_PRESS_RIGHT;
		break;
	}
}

void Kleptobot::setStolenGoodie(int type)
{
	m_stolenGoodie = type;
}


///////////////////////////////////
//ANGRY_KLEPTOBOT IMPLEMENTATIONS//
///////////////////////////////////

Angry_Kleptobot::Angry_Kleptobot(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints)
	: Kleptobot(startX, startY, studentWorld, imageID, startDirection, hitpoints) {}

//////////////////////////////////////
//KLEPTOBOT_FACTORY IMPLEMENTATIONS///
//////////////////////////////////////

Kleptobot_Factory::Kleptobot_Factory(int startX, int startY, StudentWorld* studentWorld, std::string identity, int imageID, Direction startDirection, int hitpoints)
	: Actor(imageID, startX, startY, startDirection, studentWorld, hitpoints)
{
	setVisible(true);
	m_identity = identity;
}

void Kleptobot_Factory::doSomething()
{
	getWorld()->produceKleptobotIfPossible(getX(), getY(), m_identity);
}

////////////////////////////////////////////
//ANGRY_KLEPTOBOT_FACTORY IMPLEMENTATIONS///
////////////////////////////////////////////

Angry_Kleptobot_Factory::Angry_Kleptobot_Factory(int startX, int startY, StudentWorld* studentWorld, std::string identity)
	: Kleptobot_Factory(startX, startY, studentWorld, identity) {}