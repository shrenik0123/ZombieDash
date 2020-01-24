#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "StudentWorld.h"
#include "Level.h"

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor (int imageID, int startX, int startY, Direction dir, int depth) : GraphObject (imageID, startX, startY, dir, depth) {
        m_alive = true;
        m_tick = 0;
    }
    virtual ~Actor();
    virtual void doSomething() = 0;
    
    StudentWorld* getGameWorld() const;
    void setGameWorld(StudentWorld* p);
    
    bool doesOverlap(double a1X, double a1Y, double a2X, double a2Y) const;
    bool determineBlockingOfMovement (Direction d, int LeftEdge1, int bottomEdge1 , Level::MazeEntry me) const;
    
    bool isAlive() const { return m_alive; }
    void setAliveStatus(bool a);
    
    void setTick (int t) { m_tick = t; }
    int getTick() const {  return m_tick; }
    
    //added new methods
    virtual bool canBecomeZombie() = 0;  //person, zombie and citizen
    virtual bool canBeInfected() = 0;   //person and citizen
    virtual bool isStatic() = 0 ; // wall and exit
    virtual bool isDamagable() { return true; } //damagable
    
    virtual bool hasExited() { return false; }
    virtual void setExited (bool f) {}
    
    virtual bool isInfected() const { return false;};
    virtual void setInfectedStatus (bool status) {};
    
    virtual int  getInfectionCount() const { return false;}
    virtual void setInfectionCount (int cnt) {}
    
    void setDestroy(bool b) { m_destroyed = b; }
    virtual bool hasBeenDestroyed() { return m_destroyed; }
    
    virtual bool canExplode() { return false; }
    virtual bool canBeKilledByPit() { return false; }
    
    virtual bool isDumbZombie() { return false; }
    virtual bool isSmartZombie() { return false; }

    
private:
    StudentWorld* m_gameWorld = nullptr;
    bool m_alive = true;
    int m_tick = 0;
    bool m_destroyed = false;
};

//Zombies, citizens, fire, penelope. just these 4 and make a different one for projectiles
class MoveableActor : public Actor {
public:
    MoveableActor(int imageID, int startX, int startY, Direction dir, int depth) : Actor(imageID, startX, startY, dir, depth)
    {
        m_infected = false;
        m_infectionCount = 0;
        m_exited = false;
    }
    virtual ~MoveableActor();
    virtual void doSomething() = 0;
    
    bool isInfected() const;
    void setInfectedStatus (bool status);
    
    int  getInfectionCount() const;
    void setInfectionCount(int cnt);
    
    bool hasExited()  const;
    void setExited (bool f);
    
    virtual bool canBeInfected() {
        return true;
    }
    
    virtual bool canBecomeZombie() {
        return true;
    }
    
    virtual bool isStatic() {
        return false;
    }
    
    virtual bool canBeKilledByPit() {
        return true;
    }
    
private:
    bool m_infected;
    int  m_infectionCount;
    bool m_exited = false;
};

class Environment : public Actor {
public:
    Environment(int imageID, int startX, int startY, Direction dir, int depth) : Actor(imageID, startX, startY, dir, depth) {}
    virtual ~Environment() {}
    virtual void doSomething() = 0;
    
    virtual bool canBeInfected() {
        return false;
    }
    
    virtual bool canBecomeZombie() {
        return false;
    }
    
    virtual bool isStatic() {
        return false;
    }
    
    bool hasExited() { return true; }
    void setExited (bool f) {}
};

//Penelope class
class Penelope : public MoveableActor {
public:
    Penelope (int x, int y) : MoveableActor (IID_PLAYER, x, y, right, 0) {
        m_landmines = 0;
        m_flamethrower = 0;
        m_vaccines = 0;
    }
    int  getVaccines() const;
    void setVaccines(int cnt);
    
    int  getLandmines() const;
    void setLandmines(int cnt);
    
    int  getFlameThrower() const;
    void setFlameThrower(int cnt);
    
    virtual ~Penelope();
    virtual void doSomething();
private:
    int  m_vaccines;
    int m_landmines = 0;
    int m_flamethrower = 0;
};

class Citizen : public MoveableActor {
public:
    Citizen (int x, int y) : MoveableActor (IID_CITIZEN, x, y, right, 0) {}
    virtual ~Citizen();
    virtual void doSomething();
};

//Dumb Zombie class
class DumbZombie : public MoveableActor {
public:
    DumbZombie (int x, int y) : MoveableActor (IID_ZOMBIE, x, y, right, 0) {
        m_movementPlanDistance = 0;
    }
    virtual ~DumbZombie();
    virtual void doSomething();
    
    void setMovementPlanDistance(int d) {
        m_movementPlanDistance = d;
    }
    
    int getMovementPlanDistance() const {
        return m_movementPlanDistance;
    }
    
    virtual bool canBeInfected() {
        return false;
    }
    
    virtual bool isDumbZombie() { return true; }
    
private:
    int m_movementPlanDistance = 0;
};


class SmartZombie : public DumbZombie {
public:
    SmartZombie (int x, int y) : DumbZombie (x, y) {}
    virtual ~SmartZombie();
    virtual void doSomething();
    virtual bool isDumbZombie() { return false; }
    virtual bool isSmartZombie() { return true; }
};

class Wall : public Environment {
public:
    Wall(int x, int y) : Environment(IID_WALL, x, y, right, 0) {}
    virtual ~Wall() {}
    virtual void doSomething();
    
    bool canBecomeZombie() {
        return false;  //cange later
    }
    
    virtual bool isStatic() {
        return true;
    }
    
    virtual bool isDamagable() {
        return false;
    }
};

//Pit class
class Pit : public Environment {
public:
    Pit(int x, int y) : Environment(IID_PIT, x, y, right, 0) {}
    virtual ~Pit() {}
    virtual void doSomething();
    virtual bool isDamagable() {
        return false;
    }
    
};

//Exit class
class Exit : public Environment {
public:
    Exit(int x, int y) : Environment(IID_EXIT, x, y, right, 1) {}
    virtual ~Exit() {}
    virtual void doSomething();
    
    virtual bool isDamagable() {
        return false;
    }
    
    virtual bool isStatic() {
        return true;
    }
};

//vaccine class
class Vaccine : public Environment {
public:
    Vaccine(int x, int y) : Environment(IID_VACCINE_GOODIE, x, y, right, 1) {}
    virtual ~Vaccine() {}
    virtual void doSomething();
};


class Flame : public Environment {
public:
    Flame(int x, int y, Direction dir) : Environment(IID_FLAME, x, y, dir, 0) {}
    virtual ~Flame() {}
    virtual void doSomething();
    virtual bool isDamagable() {
        return false;
    }
};


//GasCan Goddie class
class GasCan : public Environment {
public:
    GasCan(int x, int y) : Environment(IID_GAS_CAN_GOODIE, x, y, right, 1) {}
    virtual ~GasCan() {}
    virtual void doSomething();
};


//Landmine Goddie class
class LandmineGoodie : public Environment {
public:
    LandmineGoodie(int x, int y) : Environment(IID_LANDMINE_GOODIE, x, y, right, 1) {}
    virtual ~LandmineGoodie() {}
    virtual void doSomething();
};

//Vomit class
class Vomit : public Environment {
public:
    Vomit(int x, int y) : Environment(IID_VOMIT, x, y, right, 0) {}
    virtual ~Vomit() {}
    virtual void doSomething();
    virtual bool isDamagable() {
        return false;
    }
};



class Landmine : public Environment {
public:
    Landmine(int x, int y) : Environment(IID_LANDMINE, x, y, right, 1) {
        m_safetyTicks = 30;
        m_active = false;
    }
    virtual ~Landmine() {}
    virtual void doSomething();
    virtual bool canExplode() { return true; }
private:
    int m_safetyTicks = 30;
    bool m_active = false;
};


#endif // ACTOR_H_


