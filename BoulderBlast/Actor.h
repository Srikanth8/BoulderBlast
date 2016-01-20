#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

//////////////////////////
//ACTOR DECLARATION///////
//////////////////////////

class Actor : public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, StudentWorld* studentWorld, int hitpoints);
	virtual void doSomething() = 0;
	void takeDamage();
	void restoreHitpoints();
	void createBullet();
	int getID() const;
	int getHitpoints() const;
	bool getAlive() const;
	void setAlive(bool status);
	StudentWorld* getWorld() const;

private:
	int m_ID;
	bool m_alive;
	int m_hitpoints;
	StudentWorld* m_studentWorld;	
};

//////////////////////////
//PLAYER DECLARATION/////
//////////////////////////

class Player : public Actor
{
public:
	Player(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_PLAYER, Direction startDirection = right, int hitpoints = 20, int ammo = 20);
	virtual void doSomething();
	int getAmmo();
	void increaseAmmo();

private:
	int m_ammo;
}; 

//////////////////////////
//WALL DECLARATION///////
//////////////////////////

class Wall : public Actor
{
public:
	Wall(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_WALL, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();

private:
}; 

//////////////////////////
//EXIT DECLARATION///////
//////////////////////////

class Exit : public Actor
{
public:
	Exit(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_EXIT, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();
	void setReveal(bool status);
	bool getReveal();

private:
	bool m_haveIBeenRevealed;
}; 

//////////////////////////
//BOULDER DECLARATION/////
//////////////////////////

class Boulder : public Actor
{
public:
	Boulder(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_BOULDER, Direction startDirection = none, int hitpoints = 10);
	virtual void doSomething();

private:
};

////////////////////////
//HOLE DECLARATION//////
////////////////////////

class Hole : public Actor
{
public:
	Hole(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_HOLE, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();

private:
};

//////////////////////////
//BULLET DECLARATION//////
//////////////////////////

class Bullet : public Actor
{
public:
	Bullet(int startX, int startY, StudentWorld* studentWorld, int startDirection_no, int imageID = IID_BULLET, int hitpoints = 1);
	virtual void doSomething();

private:
};

//////////////////////////
//GOODIE DECLARATION//////
//////////////////////////

class Goodie : public Actor
{
public:
	Goodie(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints);
	virtual void doSomething();
	void takeGoodieAndPlaySound();

private:
};

////////////////////////
//JEWEL DECLARATION/////
////////////////////////

class Jewel : public Goodie
{
public:
	Jewel(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_JEWEL, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();

private:
};

///////////////////////////////
//RESTORE_HEALTH DECLERATION///
///////////////////////////////

class Restore_Health : public Goodie
{
public:
	Restore_Health(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_RESTORE_HEALTH, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();

private:
}; 

///////////////////////////
//EXTRA_LIFE DECLERATION///
///////////////////////////

class Extra_Life : public Goodie
{
public:
	Extra_Life(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_EXTRA_LIFE, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();

private:
};

//////////////////////
//AMMO DECLERATION////
//////////////////////

class Ammo : public Goodie
{
public:
	Ammo(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_AMMO, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();

private:
};

///////////////////////
//ROBOT DECLERATION////
///////////////////////

class Robot : public Actor
{
public:
	Robot(int startX, int startY, StudentWorld* studentWorld, int imageID, Direction startDirection, int hitpoints);
	bool shouldIdoSomething();
	bool shootIfInLOS();
	void increaseTicks();

private:
	int m_ticks;
};

//////////////////////////
//SNARLBOT DECLERATION////
//////////////////////////

class Snarlbot : public Robot
{
public:
	Snarlbot(int startX, int startY, StudentWorld* studentWorld, int startDirection_no, int imageID = IID_SNARLBOT, int hitpoints = 10);
	virtual void doSomething();

private:
};

//////////////////////////
//KLEPTOBOT DECLERATION///
//////////////////////////

class Kleptobot : public Robot
{
public:
	Kleptobot(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_KLEPTOBOT, Direction startDirection = right, int hitpoints = 5);
	virtual void doSomething();
	void setStolenGoodie(int type);

private:
	int m_distanceBeforeTurning;
	int m_distance;
	int m_stolenGoodie;

	int randStraightLine();
	int randDirection();
};

////////////////////////////////
//ANGRY_KLEPTOBOT DECLERATION///
////////////////////////////////

class Angry_Kleptobot : public Kleptobot
{
public:
	Angry_Kleptobot(int startX, int startY, StudentWorld* studentWorld, int imageID = IID_ANGRY_KLEPTOBOT, Direction startDirection = right, int hitpoints = 8);

private:
};

//////////////////////////////////
//KLEPTOBOT_FACTORY DECLERATION///
//////////////////////////////////

class Kleptobot_Factory : public Actor
{
public:
	Kleptobot_Factory(int startX, int startY, StudentWorld* studentWorld, std::string identity = "normal", int imageID = IID_ROBOT_FACTORY, Direction startDirection = none, int hitpoints = 1);
	virtual void doSomething();

private:
	std::string m_identity;
};

////////////////////////////////////////
//ANGRY_KLEPTOBOT_FACTORY DECLERATION///
////////////////////////////////////////

class Angry_Kleptobot_Factory : public Kleptobot_Factory
{
public:
	Angry_Kleptobot_Factory(int startX, int startY, StudentWorld* studentWorld, std::string identity = "angry");

private:

};

#endif // ACTOR_H_