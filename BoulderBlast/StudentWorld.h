#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);

	virtual int init();

	virtual int move();

	virtual void cleanUp();

	Player* getPlayer();

	bool canPlayerMoveThere(int x, int y, int moveDirection);

	bool canBouldersMoveThere(int x, int y);

	bool isBoulderOnHole(int x, int y);

	bool isPlayerOnMe(int x, int y);

	void createBulletAt(int x, int y, int direction);

	bool canBulletDamage(int x, int y);

	bool canBulletDie(int x, int y);

	bool isPlayerInMyLOS(int x, int y, int direction);

	bool canRobotMoveThere(int x, int y);

	bool stealGoodieIfPossible(int x, int y, Kleptobot* ptr);

	void createGoodieAt(int x, int y, int ID);

	void produceKleptobotIfPossible(int x, int y, std::string identity);

	~StudentWorld();

private:
	std::vector<Actor*> m_actorVector;
	Player* m_player;
	int m_bonusPoints;

	int getWhatIsAt(int x, int y); // returns ID
};

#endif // STUDENTWORLD_H_
