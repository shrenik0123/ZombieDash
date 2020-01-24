#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <list>
using namespace std;

class Actor;
class Exit;
class Penelope;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    void checkOverlap(Actor* e, Level::MazeEntry me1, Level::MazeEntry me2);
    
    bool checkBoundingBoxOverlap(Actor* first, int destX, int destY, Direction direction, int inc);
    bool checkBoundingBoxOverlap(Actor* first, Actor* second, int destX, int destY, Direction direction, int inc);
    bool checkBoundingBoxOverlap(Actor* first, int destX, int destY, Direction direction, int inc, bool wall);
   
    double findMinimumEucleadeanDistance (Actor *checkFor, Level::MazeEntry me);
    double findEucleadeanDistance (double a1X, double a1Y, double a2X, double a2Y);
    bool checkPersonInEucDistance(Actor* first, int vomitXcoord, int vomitYcoord, int targetDistance);
    
    bool doesOverlap (Actor* checkfor, int x, int y, int targetDistance, bool checkwithPenelope) ;
    
    Level* getLevelObject() const;
    long int getTick() const;
    void setTick(long int amt);
    
    bool checkIfOverlap (Actor* checkfor, int x, int y, int targetDistance, bool damagable);
    bool checkIfOverlap (Actor* checkfor, int x, int y, int targetDistance, Level::MazeEntry me, bool setalive, bool infect, bool destroy);
    
    bool areAllCitizensDeadOrExited();
    void setPlayerExit (bool fexit);
    
    void createActor (Level::MazeEntry me, int x, int y);
    void createVomit (int x, int y);

    void moveCitizen (Actor* a, double dist_p, double dist_z);
    void moveZombie  (Actor* a);
    
    void createNewFlame(int destX, int destY, Direction d);
    void createNewLandmine(int destX, int destY);
    void createNewVaccine(int destX, int destY);
    bool doSomethingForGoodies(Actor *a);
    void addFlames(int amt);
    void addVaccine(int amt);
    void addLandmines(int amt);
    bool KilledByPit (Actor* checkfor, int x, int y, int targetDistance);
    
private:
    Penelope*        m_Penelope = nullptr;
    list <Actor*>    m_actors;
    Level* m_level = nullptr;
    long int m_tick = 0;
    
    //private methods
    int getNumWidth(int num);
    Level::LoadResult loadLevel(string assetFile);
    Level::MazeEntry getCoord(Level::MazeEntry m) const;
};

#endif // STUDENTWORLD_H_


/*
 // Add an actor to the world.
 void addActor(Actor* a);
 
 // Record that one more citizen on the current level is gone (exited,
 // died, or turned into a zombie).
 void recordCitizenGone();
 
 // Indicate that the player has finished the level if all citizens
 // are gone.
 void recordLevelFinishedIfAllCitizensGone();
 
 // For each actor overlapping a, activate a if appropriate.
 void activateOnAppropriateActors(Actor* a);
 
 // Is an agent blocked from moving to the indicated location?
 bool isAgentMovementBlockedAt(double x, double y) const;
 
 // Is creation of a flame blocked at the indicated location?
 bool isFlameBlockedAt(double x, double y) const;
 
 // Is there something at the indicated location that might cause a
 // zombie to vomit (i.e., a human)?
 bool isZombieVomitTriggerAt(double x, double y) const;
 
 // Return true if there is a living human, otherwise false.  If true,
 // otherX, otherY, and distance will be set to the location and distance
 // of the human nearest to (x,y).
 bool locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);
 
 // Return true if there is a living zombie or Penelope, otherwise false.
 // If true, otherX, otherY, and distance will be set to the location and
 // distance of the one nearest to (x,y), and isThreat will be set to true
 // if it's a zombie, false if a Penelope.
 bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const;
 
 // Return true if there is a living zombie, false otherwise.  If true,
 // otherX, otherY and distance will be set to the location and distance
 // of the one nearest to (x,y).
 bool locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const;
 */
