#include "Actor.h"
#include "StudentWorld.h"

//actor destructor
Actor::~Actor() {
    m_gameWorld = nullptr;
}

//MoveableActor destructor
MoveableActor::~MoveableActor() { }

// getter-setter for infection status
bool MoveableActor::isInfected() const { return m_infected; }
void MoveableActor::setInfectedStatus (bool status) { m_infected = status; }

// getter-setter for infection count
int  MoveableActor::getInfectionCount() const { return m_infectionCount; }
void MoveableActor::setInfectionCount(int cnt) { m_infectionCount = cnt;}

// getter-setter for vaccine status
int  Penelope::getVaccines() const {  return m_vaccines; }
void Penelope::setVaccines(int cnt){ m_vaccines = cnt; }

// getter-setter for landmine status
int  Penelope::getLandmines() const {  return m_landmines; }
void Penelope::setLandmines(int cnt){ m_landmines = cnt; }

// getter-setter for flames status
int  Penelope::getFlameThrower() const {  return m_flamethrower; }
void Penelope::setFlameThrower(int cnt){ m_flamethrower = cnt; }

// getter-setter for exit status
bool MoveableActor::hasExited()  const { return m_exited; }
void MoveableActor::setExited (bool f) { m_exited = f;}

void Actor::setAliveStatus(bool a) {
    m_alive = a;
}

StudentWorld* Actor::getGameWorld() const {
    return m_gameWorld;
}

void Actor::setGameWorld(StudentWorld* p) {
    m_gameWorld = p;
}

//Determining Blocking of Movement
//LeftEdge1, rightEdge1, bottomEdge1, topEdge1
bool Actor::determineBlockingOfMovement (Direction d, int leftEdge1, int bottomEdge1, Level::MazeEntry me) const { // Penelope coordinates, finds next coordinate and wall
    //bool yBlock = false, xBlock = false;
    int addXme = 0,  addYme = 0;
    
    if (d == right) addXme = 1;
    if (d == left)  addXme = 0;
    if (d == up)    addYme = 1;
    if (d == down)  addYme = 0;
    
    //find out next maze Left Bottom x,y coordinate, from first object
    
    int leftEdgeME1 = leftEdge1/LEVEL_WIDTH;
    int bottomEdgeME1 = bottomEdge1/LEVEL_HEIGHT;
    
    int rightEdge1 = leftEdge1  + SPRITE_WIDTH;
    int topEdge1 = bottomEdge1  + SPRITE_HEIGHT;
    
    int leftEdge2 = (leftEdgeME1+addXme)*LEVEL_WIDTH;
    
    int bottomEdge2 = (bottomEdgeME1+addYme)*LEVEL_HEIGHT;
    int rightEdge2 = leftEdge2  + SPRITE_WIDTH;
    
    
    
    Level::MazeEntry m1, m2 ;
    if (d == left) {
        m1 = getGameWorld()->getLevelObject()->getContentsOf((leftEdge1)/SPRITE_WIDTH, bottomEdge1/SPRITE_HEIGHT);
        if (bottomEdge1 % SPRITE_HEIGHT == 0) { //if perfect alignment, lets take next block as below
            m2 = m1;
        } else {
            m2 = getGameWorld()->getLevelObject()->getContentsOf((leftEdge1)/SPRITE_WIDTH, (bottomEdge1/SPRITE_HEIGHT)+1);
        }
        if ((leftEdge1 < rightEdge2) && (leftEdge1 > 0) && (me == m1 || me == m2))
            return true;
    }
    
    if (d == right) {
        m1 = getGameWorld()->getLevelObject()->getContentsOf((rightEdge1)/SPRITE_WIDTH, bottomEdge1/SPRITE_HEIGHT);
        if (bottomEdge1 % SPRITE_HEIGHT == 0) { //if perfect alignment, lets take next block as below
            m2 = m1;
        } else {
            m2 = getGameWorld()->getLevelObject()->getContentsOf((rightEdge1)/SPRITE_WIDTH, (bottomEdge1/SPRITE_HEIGHT)+1);
        }
        if ((rightEdge1 >= leftEdge2) && (rightEdge1 > 0) && (me == m1 || me == m2))
            return true;
    }
    
    if (d == up) {
        m1 = getGameWorld()->getLevelObject()->getContentsOf((leftEdge1)/SPRITE_WIDTH, topEdge1/SPRITE_HEIGHT);
        if (leftEdge1 % SPRITE_WIDTH == 0) { //if perfect alignment, lets take next block as below
            m2 = m1;
        } else {
            m2 = getGameWorld()->getLevelObject()->getContentsOf((rightEdge1/SPRITE_WIDTH), topEdge1/SPRITE_HEIGHT);
        }
        if ((topEdge1 >= bottomEdge2) && (topEdge1 > 0) && (me == m1 || me == m2))
            return true;
    }
    
    if (d == down) {
        m1 = getGameWorld()->getLevelObject()->getContentsOf((leftEdge1)/SPRITE_WIDTH, (bottomEdge1/SPRITE_HEIGHT)-1);
        if (leftEdge1 % SPRITE_WIDTH == 0) { //if perfect alignment, lets take next block as below
            m2 = m1;
        } else {
            m2 = getGameWorld()->getLevelObject()->getContentsOf((rightEdge1/SPRITE_WIDTH), (bottomEdge1/SPRITE_HEIGHT)-1);
        }
        if ((bottomEdge1 <= bottomEdge2) && (bottomEdge1 > 0) && (me == m1 || me == m2))
            return true;
    }
    return false;
}


//checks if 2 actors overlap by using Euclidian distance
bool Actor::doesOverlap(double a1X, double a1Y, double a2X, double a2Y) const {
    int deltaX = a2X - a1X;
    int deltaY = a2Y - a1Y;
    return ((deltaX*deltaX) + (deltaY*deltaY) <= 100);
}
 


void Penelope::doSomething() {
    //checks alive and infection status
    if (!isAlive())
        return;
    if (isInfected())
        setInfectionCount(getInfectionCount()+1);
    if (getInfectionCount() == 500) {
        setAliveStatus(false);
        getGameWorld()->playSound(SOUND_PLAYER_DIE);
        getGameWorld()->playSound(SOUND_ZOMBIE_BORN);
        getGameWorld()->createActor (Level::MazeEntry::dumb_zombie, getX(), getY());
        return;
    }
   
    int step = 4;
   // bool block, boundingBox;
    int ch;
    //bool b ;
    // makes Penelope move in the direction user inputted if possible // 957522360
    if (getGameWorld()->getKey(ch)) {
        switch (ch) {
            case KEY_PRESS_RIGHT:
                setDirection(right);
                
                if ( determineBlockingOfMovement (right, getX(), getY(), Level::MazeEntry::wall) ||
                     getGameWorld()->checkBoundingBoxOverlap (this, getX(), getY(), right, step))
                    //(determineBlockingOfMovement(right, getX(), getY(), Level::MazeEntry::dumb_zombie)))
                    return;
                moveTo(getX()+4, getY());
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                if ( determineBlockingOfMovement(left, getX()-4, getY(), Level::MazeEntry::wall) ||
                     getGameWorld()->checkBoundingBoxOverlap (this, getX(), getY(),left, step))
                    //(determineBlockingOfMovement(left, getX()-4, getY(), Level::MazeEntry::dumb_zombie))
                    return;
                moveTo(getX()-4, getY());
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if ( determineBlockingOfMovement(down, getX(), getY(), Level::MazeEntry::wall) ||
                     getGameWorld()->checkBoundingBoxOverlap (this,getX(), getY(), down, step))
                    //(determineBlockingOfMovement(down, getX(), getY(), Level::MazeEntry::dumb_zombie)))
                    return;
                moveTo(getX(), getY()-4);
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if ( determineBlockingOfMovement(up, getX(), getY(), Level::MazeEntry::wall) ||
                    getGameWorld()->checkBoundingBoxOverlap (this,getX(), getY(), up, step))
                    //(determineBlockingOfMovement(up, getX(), getY(), Level::MazeEntry::dumb_zombie)))
                    return;
                moveTo(getX(), getY()+4);
                break;
            case KEY_PRESS_ENTER:  //vaccines
                if (getVaccines() > 0) {
                    setInfectedStatus(false);
                    setInfectionCount(0);
                    setVaccines(getVaccines()-1);
                }
                break;
            case KEY_PRESS_TAB:  //landmines
                if (getLandmines() > 0) {
                    getGameWorld()->createNewLandmine(getX(), getY());
                    setLandmines(getLandmines() - 1);
                }
                break;
            case KEY_PRESS_SPACE:
                if (getFlameThrower() == 0)
                    return;
                else
                    setFlameThrower(getFlameThrower()-1);
                int dir = getDirection();
                switch (dir) {
                    case right:
                        for (int i = 1; i < 4; i++) {
                            if (!determineBlockingOfMovement(right, getX() + i *SPRITE_WIDTH, getY(), Level::MazeEntry::wall))
                                getGameWorld()->createNewFlame(getX() + i *SPRITE_WIDTH, getY(), right);
                            else
                                return;
                        }
                        break;
                    case left:
                        for (int i = 1; i < 4; i++) {
                            if (!determineBlockingOfMovement(left, getX() - i *SPRITE_WIDTH, getY(), Level::MazeEntry::wall))
                                getGameWorld()->createNewFlame(getX() - i *SPRITE_WIDTH, getY(), left);
                            else
                                return;
                        }
                        break;
                    case up:
                        for (int i = 1; i < 4; i++) {
                            if (!determineBlockingOfMovement(up, getX(), getY() + i *SPRITE_HEIGHT, Level::MazeEntry::wall))
                                getGameWorld()->createNewFlame(getX(), getY() + i *SPRITE_HEIGHT, up);
                            else
                                return;
                        }
                        break;
                    case down:
                        for (int i = 1; i < 4; i++) {
                            if (!determineBlockingOfMovement(down, getX(), getY() - i *SPRITE_HEIGHT, Level::MazeEntry::wall))
                                getGameWorld()->createNewFlame(getX(), getY() - i *SPRITE_HEIGHT, down);
                            else
                                return;
                        }
                        break;
                }
                //todo: wall and exit
        }
    }
}

                

// doSomething for Wall which does nothing
//*** TODO:  validated steps on P30  **************
void Wall::doSomething() {
    return;
}

//### P30 - Exit dosomething
void Exit::doSomething() {
 
    //### if overlaps with citizen
    //*** TODO: change the overlap function *************
    bool set_dead = true;
    if (getGameWorld()->checkIfOverlap (this, getX(), getY(), 10, Level::MazeEntry::citizen, set_dead, false, false)) {
        getGameWorld()->increaseScore(500);
        setAliveStatus(false);
        //### *** TODO:  set citizen object to dead  P31.1b
        getGameWorld()->playSound(SOUND_CITIZEN_SAVED);
    }
  
    //### TODO:@@@@@@@@@@@@@@@@ testign remove this code later

    
    // if overlaps with penelope and all citizens are dead or no citizens in grid
    set_dead = false;
    if (getGameWorld()->checkIfOverlap (this, getX(), getY(), 10, Level::MazeEntry::player, set_dead, false, false)) {
        //*** TODO:  if all citizens exited or dead
        //inform that current level is done
        if (getGameWorld()->areAllCitizensDeadOrExited()) {
            getGameWorld()->setPlayerExit(true);
        }
    }
    //### TODO remaining P30
}


/*
 A pit must be given an opportunity to do something during every tick (in its doSomething() method). When given an opportunity to do something during a tick, the pit will cause any person or zombie that overlaps with it to be destroyed (they fall into the pit). When the person/zombie is destroyed, it must behave just as it were damaged by a flame (e.g., if a dumb zombie falls into a pit, the player gets 1000 points, the game plays a dying noise, etc.; if Penelope falls into a pit the current level will end; a citizen falling into a pit dies, and the player loses 1000 points, etc.).
 */

void Pit::doSomething() {
    getGameWorld()->KilledByPit (this, getX(), getY(), 10);
}

//DumbZombie
void DumbZombie::doSomething() {
    if (!isAlive())
        return;
    
    setTick(getTick()+1);
    if (getTick() % 2 == 0)
        return;
    
    //compute vomit coordinates
    int vomitXcoord = getX(), vomitYcoord = getY();
    Direction d = getDirection();
    if (d == left)  vomitXcoord -= SPRITE_WIDTH;
    if (d == right) vomitXcoord += SPRITE_WIDTH;
    if (d == down)  vomitYcoord -= SPRITE_HEIGHT;
    if (d == up)    vomitYcoord += SPRITE_HEIGHT;
    
    int destX = getX(), destY = getY();
    

    if (getGameWorld()->checkPersonInEucDistance(this, vomitXcoord, vomitYcoord, 10)) {
        int r = randInt(1, 3);
        if (r == 1) {
            getGameWorld()->createVomit (vomitXcoord, vomitYcoord);
            getGameWorld()->playSound(SOUND_ZOMBIE_VOMIT);
            return;
        }
    }
    
    //add section 3 (it's in the comments above) pg 40
    if (getMovementPlanDistance() == 0) {
        setMovementPlanDistance(randInt(3, 10)); //TODO : check if its inclusive
        int i = randInt(1, 4);
        
        
        
        switch (i) {
            case 1:
                setDirection (right);
                destX += 1;
                break;
            case 2:
                setDirection (left);
                destX -= 1;
                break;
            case 3:
                setDirection (up);
                destY += 1;
                break;
            case 4:
                setDirection (down);
                destY -= 1;
                break;
        }
    }
    
    
    if (getGameWorld()->checkBoundingBoxOverlap(this, destX, destY, getDirection(), 1, true))
    {
        setMovementPlanDistance(0);
        return;
    }
    else {
        switch (getDirection()) {
            case right:
                moveTo(getX()+1, getY());
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            default:
                break;
        }
        setMovementPlanDistance(getMovementPlanDistance()-1);
    }
    
    //### TODO:
    /*
    1 in 10 dumb zombies are mindlessly carrying a vaccine goodie that they'll
    drop when they die. If this dumb zombie has a vaccine goodie, it will
    introduce a new vaccine goodie at its (x,y) coordinate by adding it to the
    StudentWorld object.
    */
    
} //it moves to the right and then stops, same thing with the left. Maybe. SOmetimes it does. It went and then stopped


void SmartZombie::doSomething() {
    if (!isAlive())
        return;
    setTick(getTick()+1);
    if (getTick() % 2 == 0)
        return;
    
    //compute vomit coordinates
    int vomitXcoord = getX(), vomitYcoord = getY();
    Direction d = getDirection();
    if (d == left)  vomitXcoord -= SPRITE_WIDTH;
    if (d == right) vomitXcoord += SPRITE_WIDTH;
    if (d == down)  vomitYcoord -= SPRITE_HEIGHT;
    if (d == up)    vomitYcoord += SPRITE_HEIGHT;
    int destX = getX(), destY = getY();
  
    if (getGameWorld()->checkPersonInEucDistance(this, vomitXcoord, vomitYcoord, 10)) {
        int r = randInt(1, 3);
        if (r == 1) {
            getGameWorld()->createVomit (vomitXcoord, vomitYcoord);
            getGameWorld()->playSound(SOUND_ZOMBIE_VOMIT);
            return;
        }
    }
    
    
    //add section 3 (it's in the comments above) pg 40
    if (getMovementPlanDistance() == 0) {
        setMovementPlanDistance(randInt(3, 10));
        int i = randInt(1, 4);
        switch (i) {
            case 1:
                setDirection (right);
                destX += 1;
                break;
            case 2:
                setDirection (left);
                destX -= 1;
                break;
            case 3:
                setDirection (up);
                destY += 1;
                break;
            case 4:
                setDirection (down);
                destY -= 1;
                break;
        }
    }
    
    getGameWorld()->moveZombie (this) ;
    
    
    if (getGameWorld()->checkBoundingBoxOverlap(this, destX, destY, getDirection(), 1, true))
    {
        setMovementPlanDistance(0);
        return;
    }
    else {
        switch (getDirection()) {
            case right:
                moveTo(getX()+1, getY());
                break;
            case left:
                moveTo(getX()-1, getY());
                break;
            case up:
                moveTo(getX(), getY()+1);
                break;
            case down:
                moveTo(getX(), getY()-1);
                break;
            default:
                break;
        }
        setMovementPlanDistance(getMovementPlanDistance()-1);
    }
    
} //it moves to the right and then stops, same thing with the left. Maybe. SOmetimes it does. It went and then stopped

SmartZombie::~SmartZombie() {
    if (hasBeenDestroyed()) {
        getGameWorld()->playSound(SOUND_ZOMBIE_DIE);
        getGameWorld()->increaseScore (2000);
    }
}

DumbZombie::~DumbZombie() {
    if (hasBeenDestroyed()) {
        getGameWorld()->playSound(SOUND_ZOMBIE_DIE);
        getGameWorld()->increaseScore (1000);
    }
}

Citizen::~Citizen() {
    if (hasBeenDestroyed()) {
        getGameWorld()->increaseScore (-1000);
    }
}

Penelope::~Penelope() {
    if (hasBeenDestroyed()) {
        getGameWorld()->playSound(SOUND_PLAYER_DIE); //TODO**** check if this is valid for normal exit scenario
    }
    
}



void Citizen::doSomething() {
    if (!isAlive())
        return;
    
    //step 2
    if (isInfected()) {
        setInfectionCount(getInfectionCount()+1);
        if (getInfectionCount() == 500) {
            setAliveStatus(false);
            getGameWorld()->playSound(SOUND_ZOMBIE_BORN);
        
            int rd = randInt(1, 100);
            if (rd <= 70) {
                getGameWorld()->createActor (Level::MazeEntry::dumb_zombie, getX(), getY());
            } else {
                getGameWorld()->createActor (Level::MazeEntry::smart_zombie, getX(), getY());
            }
            return;
        }
    }

    //step 3
    setTick(getTick()+1);
    if (getTick() % 2 == 0)
        return;
    
    //step 4
    double dist_p = getGameWorld()->findMinimumEucleadeanDistance (this, Level::MazeEntry::player);
    double dist_z = getGameWorld()->findMinimumEucleadeanDistance (this, Level::MazeEntry::dumb_zombie);
    
    getGameWorld()->moveCitizen(this, dist_p, dist_z);
    
    /*
     if (dist_p < dist_z || dist_z == 999999.0) {
        if (dist_p < 80) {
            getGameWorld()->moveCitizen(this, dist_p, dist_z);
        }
    }
    */
    
    //TODO
}

void Vomit::doSomething() {
    if (!isAlive())
        return;
    setTick(getTick()+1);
    if (getTick() >= 2) {
        setAliveStatus(false);
        return;
    }

    getGameWorld()->checkPersonInEucDistance(this, getX(), getY(), 10);
}


void Vaccine::doSomething() {
    if (getGameWorld()->doSomethingForGoodies(this))
        getGameWorld()->addVaccine(1);
}

void GasCan::doSomething() {
    if (getGameWorld()->doSomethingForGoodies(this))
         getGameWorld()->addFlames(5);
}


void LandmineGoodie::doSomething() {
    if (getGameWorld()->doSomethingForGoodies(this))
        getGameWorld()->addLandmines(2);
}


/*
 1. It must check to see if it is currently alive. If not, then doSomething() must return immediately – none of the following steps should be performed.
 2. After exactly two ticks from its creation, the flame must set its state to dead so it can be destroyed and removed from the level by the StudentWorld object. The doSomething() method must return immediately, doing nothing more during this tick.
 32
 3. Otherwise, the flame will damage all damageable objects that overlap8 with the flame. The following objects are all damageable and must react to being damaged in the appropriate way: Penelope, citizens, all types of goodies, landmines, and all types of zombies.
 */

void Flame::doSomething() {
    if (!isAlive())
        return;
    setTick(getTick()+1);
    if (getTick() >= 2) {
        setAliveStatus(false);
        return;
    }
    //*** TODO: do part 3
    
    getGameWorld()->checkIfOverlap (this, getX(), getY(), 10, true); //true is for damagable
    
    if (getGameWorld()->checkBoundingBoxOverlap(this, getX(), getY(), getDirection(), 1, true))
        return;
    
    /*
    if (getGameWorld()->checkIfOverlap (this, getX(), getY(), 10, Level::MazeEntry::dumb_zombie, true, false, true)) { // both dumb and smart zombies and all citizens overlap
        getGameWorld()->playSound(SOUND_ZOMBIE_DIE);
    }
    */
    //###TODO###
    //set LandMine Goodie then to dead
    //set Gas Goodie then to dead
    //set vaccine goodie
    //Exclude Vomit object
    //Flamce cant' damage itself
    //Pit can't damage
    //Exit can't be damaged
    
    //Exit is blocked by Flames
    /*An exit DOES block flames (i.e., a flame fired by Penelope or emitted by a
                               landmine cannot overlap or be created past an exit).
    */
    
    /*
     A wall blocks the movement of citizens, zombies, and Penelope (its bounding box
     must never intersect with any actor’s bounding box)5
     • A wall blocks flames (i.e., a flame fired by Penelope or emitted by a landmine
     cannot overlap6 or be created past a wall).
     */
    
    /*
     Penelope can be infected by vomit. When vomit overlaps with Penelope, her
     infection status becomes true.
     • Penelope can be damaged. If a flame object overlaps with Penelope it will kill her.
     When killed:
     o She must immediately has her status set to dead,
     o The game must play a SOUND_PLAYER_DIE sound effect
     o (The StudentWorld object should detect her death and the current level
     ends)
     • Penelope blocks other objects from moving nearby/onto her. Penelope’s bounding
     box must never intersect with that of any citizen, zombie, or wall.
     */
}

//Landmine
void Landmine::doSomething() {
    //if it overlaps with a flame, then make the landmine explode
    //just done ------------------------------
    
    if (!isAlive() && !hasBeenDestroyed())
        return;
    if (!m_active && !hasBeenDestroyed()) {
        m_safetyTicks--;
        if (m_safetyTicks == 0)
            m_active = true;
        return;
    }
    if (getGameWorld()->checkIfOverlap (this, getX(), getY(), 10, Level::MazeEntry::dumb_zombie, false, false, false) ||
        getGameWorld()->checkIfOverlap (this, getX(), getY(), 10, Level::MazeEntry::player, false, false, false) || hasBeenDestroyed())        // overlaps with Penelope, Citizen or Zombie or flame
    {
        setAliveStatus(false);
        getGameWorld()->playSound(SOUND_LANDMINE_EXPLODE);
        getGameWorld()->createNewFlame(getX(), getY(), up);
        getGameWorld()->createNewFlame(getX() + SPRITE_WIDTH, getY(), up);
        getGameWorld()->createNewFlame(getX() - SPRITE_WIDTH, getY(), up);
        getGameWorld()->createNewFlame(getX(), getY() + SPRITE_HEIGHT, up);
        getGameWorld()->createNewFlame(getX(), getY() - SPRITE_HEIGHT, up);
        getGameWorld()->createNewFlame(getX() + SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
        getGameWorld()->createNewFlame(getX() + SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
        getGameWorld()->createNewFlame(getX() - SPRITE_WIDTH, getY() + SPRITE_HEIGHT, up);
        getGameWorld()->createNewFlame(getX() - SPRITE_WIDTH, getY() - SPRITE_HEIGHT, up);
        getGameWorld()->createActor(Level::MazeEntry::pit, getX(), getY());
    }
}

