#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <list>

using namespace std;

// returns GameWorld Pointer
GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

//studentWorld constructor
StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath)
{ }

// StudentWorld destructor
StudentWorld::~StudentWorld() {
    cleanUp();
}


//Determining blocking movement
bool StudentWorld::checkBoundingBoxOverlap(Actor* checkfor, int destX, int destY, Direction direction , int inc, bool wall) {
    bool overlap = false;
    
    if (checkfor != m_Penelope) { //if self check, skip
        overlap = checkBoundingBoxOverlap (checkfor, m_Penelope, destX, destY, direction , inc);
        if (overlap) return true;
    }
    
    list<Actor*>::iterator itr = m_actors.begin();
    
    while (itr != m_actors.end()) {
        Actor*a = *itr;
        if (a->isAlive()) {
            if (a->canBecomeZombie() || (wall && a->isStatic())) { //TODO::::change
                if (checkfor != a) {
                    overlap = checkBoundingBoxOverlap (checkfor, a, destX, destY, direction, inc);
                    if (overlap)
                        return true;
                }
            }
        }
        itr++;
    }
    
    return overlap;
}



bool StudentWorld::checkBoundingBoxOverlap(Actor* checkfor, int destX, int destY, Direction direction , int inc) {
    bool overlap = false;
    
    if (checkfor != m_Penelope) { //if self check, skip
        overlap = checkBoundingBoxOverlap (checkfor, m_Penelope, destX, destY, direction , inc);
        if (overlap) return true;
    }
    
    list<Actor*>::iterator itr = m_actors.begin();
  
    
    while (itr != m_actors.end()) {
        Actor*a = *itr;
        if (a->isAlive()) {
            if (a->canBecomeZombie() ) {
                if (checkfor != a) {
                    overlap = checkBoundingBoxOverlap (checkfor, a, destX, destY, direction, inc);
                    if (overlap)
                        return true;
                }
            }
        }
        itr++;
    }
    
    return overlap;
}

//chekfor actor, checkWithActor, new dest X, new dest Y, direction, increment value (4 for P and 1 for DZ)
bool StudentWorld::checkBoundingBoxOverlap(Actor* checkfor, Actor* checkwith, int destX, int destY, Direction direction, int inc) {
    
    int addX = 0, addY = 0;
    if (direction == checkfor->right) addX = inc;
    if (direction == checkfor->left) addX = inc * -1;
    if (direction == checkfor->up) addY = inc;
    if (direction == checkfor->down) addY = inc * -1;

    
    int checkforLeftEdge = destX + addX;
    int checkforRightEdge = destX  + addX + SPRITE_WIDTH;
    int checkforBottomEdge = destY  + addY;
    int checkforTopEdge = destY + addY + SPRITE_WIDTH;
    
    
    int checkwithLeftEdge = checkwith->getX();
    int checkwithRightEdge = checkwith->getX() + SPRITE_WIDTH;
    int checkwithBottomEdge = checkwith->getY();
    int checkwithTopEdge = checkwith->getY() + SPRITE_WIDTH;
    
    
    //if right direction move, lets check edges and right side
    if (direction == checkfor->right) {
        if ((checkforBottomEdge >= checkwithBottomEdge && checkforBottomEdge <= checkwithTopEdge) ||
            (checkforTopEdge >= checkwithBottomEdge && checkforTopEdge <= checkwithTopEdge)) {

            if (checkforRightEdge >= checkwithLeftEdge && checkforRightEdge <= checkwithRightEdge)
                return true;
        }
    }
    
    if (direction == checkfor->left) {
        if ((checkforBottomEdge >= checkwithBottomEdge && checkforBottomEdge <= checkwithTopEdge) ||
            (checkforTopEdge >= checkwithBottomEdge && checkforTopEdge <= checkwithTopEdge)) {
            
            if (checkforLeftEdge >= checkwithLeftEdge && checkforLeftEdge <= checkwithRightEdge)
                return true;
        }
    }
    
    if (direction == checkfor->up) {
        if (checkforTopEdge >= checkwithBottomEdge && checkforTopEdge <= checkwithTopEdge) {
            
            if ( (checkforLeftEdge >= checkwithLeftEdge && checkforLeftEdge <= checkwithRightEdge) ||
                (checkforRightEdge >= checkwithLeftEdge && checkforRightEdge <= checkwithRightEdge))
                return true;
        }
    }

    if (direction == checkfor->down) {
        if (checkforBottomEdge >= checkwithBottomEdge && checkforBottomEdge <= checkwithTopEdge) {
            
            if ( (checkforLeftEdge >= checkwithLeftEdge && checkforLeftEdge <= checkwithRightEdge) ||
                (checkforRightEdge >= checkwithLeftEdge && checkforRightEdge <= checkwithRightEdge))
                return true;
        }
    }
    
    return false;
}

double StudentWorld::findEucleadeanDistance (double a1X, double a1Y, double a2X, double a2Y)  {
    double deltaX = a2X - a1X;
    double deltaY = a2Y - a1Y;
    return ((deltaX * deltaX) + (deltaY * deltaY));
}


//checks if 2 actors overlap by using Euclidian distance
double StudentWorld::findMinimumEucleadeanDistance (Actor *checkfor,  Level::MazeEntry me)  {
    double min = 999999.0;  //some big number
    if (me == Level::MazeEntry::player) {
        return findEucleadeanDistance (checkfor->getX(), checkfor->getY(), m_Penelope->getX(), m_Penelope->getY());
    } else if (me == Level::MazeEntry::dumb_zombie || me == Level::MazeEntry::citizen) { // or smart Zombie P43
         double td = 0.0;
        bool proceed = false;
        
        list<Actor*>::iterator itr = m_actors.begin();
        
        while (itr != m_actors.end()) {
            Actor*a = *itr;
            if (a->isAlive() && a != checkfor) {
                if (me == Level::MazeEntry::dumb_zombie) {
                    proceed = a->canBecomeZombie() && !a->canBeInfected();
                } else if (me == Level::MazeEntry::citizen) {
                    proceed = a->canBeInfected();
                }
                
                if (proceed) {
                    td = findEucleadeanDistance (checkfor->getX(), checkfor->getY(), a->getX(), a->getY());
                    if (td < min) {
                        min = td;
                    }
                }
            }
            itr++;
        }
    }
    return min;
}


//Eucleadean distance for Zombie for Person
bool StudentWorld::checkPersonInEucDistance(Actor* checkfor, int vomitXcoord, int vomitYcoord, int targetDistance) {
    bool bplayer = checkIfOverlap (checkfor, vomitXcoord, vomitYcoord, targetDistance, Level::MazeEntry::player, false, true, false);
    bool bcitizen = checkIfOverlap (checkfor, vomitXcoord, vomitYcoord, targetDistance, Level::MazeEntry::citizen, false, true, false);
    return bplayer || bcitizen;
 }

//Check If Overlap
bool StudentWorld::checkIfOverlap (Actor* checkfor, int x, int y, int targetDistance, bool damagable) {
    double td;
    bool ret = false;
    
    if (checkfor != m_Penelope && m_Penelope->isDamagable() && m_Penelope->isAlive() && !m_Penelope->hasBeenDestroyed()) { //if self check, return
        td = findEucleadeanDistance (x, y, m_Penelope->getX(), m_Penelope->getY());
        if (td <= (targetDistance * targetDistance)) {
            if (damagable) {
                m_Penelope->setDestroy(true);
                m_Penelope->setAliveStatus(false);
            }
            ret = true;
        }
    }
    
    list<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end()) {
        Actor*a = *itr;
        if (a != checkfor && a->isAlive() && a->isDamagable() && !a->hasBeenDestroyed()) {
            td = findEucleadeanDistance (x, y, a->getX(), a->getY());
            if (td <= targetDistance * targetDistance) {
                a->setDestroy(true);
                // a->setExited (true);
                if (!a->canExplode())
                    a->setAliveStatus(false);
                ret = true;
            }
        }
        itr++;
    }
    return ret;
}

/*
//Check obverlap for all damagable objects (flames uses it)
bool StudentWorld::checkIfOverlap (Actor* checkfor, int x, int y, int targetDistance, bool damagable) {
    double td;
    bool ret = false;
    
    if (checkfor != m_Penelope && m_Penelope->isDamagable() && m_Penelope->isAlive() && !m_Penelope->hasBeenDestroyed()) { //if self check, return
        td = findEucleadeanDistance (x, y, m_Penelope->getX(), m_Penelope->getY());
        if (td <= targetDistance * targetDistance) {
            if (damagable) {
                m_Penelope->setDestroy(true);
                m_Penelope->setExited (true);
                m_Penelope->setAliveStatus(true);
            }
             ret = true;
        }
    }

    list<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end()) {
        Actor*a = *itr;
        if (a != checkfor && a->isAlive() && a->isDamagable() && !a->hasBeenDestroyed()) {
            td = findEucleadeanDistance (x, y, a->getX(), a->getY());
            if (td <= targetDistance * targetDistance) {
                a->setDestroy(true);
                a->setExited (true);
                a->setAliveStatus(true);
                ret = true;
            }
        }
        itr++;
    }
    return ret;
}
 */


//Eucleadean distance used for Zombie for Person
bool StudentWorld::checkIfOverlap (Actor* checkfor, int x, int y, int targetDistance, Level::MazeEntry me, bool fexit, bool infect, bool destroy) {
    double td;
    bool ret = false;
    
    if (me == Level::MazeEntry::player) {
        if (checkfor != m_Penelope) { //if self check, return
            td = findEucleadeanDistance (x, y, m_Penelope->getX(), m_Penelope->getY());
            if (td <= targetDistance * targetDistance) {
                if (fexit) {
                    m_Penelope->setExited (fexit);
                }
                if (infect) {
                    m_Penelope->setInfectionCount(m_Penelope->getInfectionCount()+1);
                    m_Penelope->setInfectedStatus(infect);
                }
                if (destroy) {
                    m_Penelope->setDestroy(true);
                }
                ret = true;
            }
        }
    } else if (me == Level::MazeEntry::citizen) {  //overlap with all citizens
        list<Actor*>::iterator itr = m_actors.begin();
        
        while (itr != m_actors.end()) {
            Actor*a = *itr;
            if (a->isAlive()) {
                if (a->canBeInfected()) {
                    if (checkfor != a) {
                        td = findEucleadeanDistance (x, y, a->getX(), a->getY());
                        if (td <= targetDistance * targetDistance) {
                            if (fexit) {
                                a->setExited (fexit);
                                a->setAliveStatus(fexit);
                            }
                            if (infect) {
                                int ic = a->getInfectionCount();
                                a->setInfectionCount(ic+1);
                            }
                            if (destroy) {
                                a->setDestroy(true);
                            }
                            ret = true;
                        }
                    }
                }
            } //if alive
            itr++;
        } //while
    } else if (me == Level::MazeEntry::dumb_zombie) {  //overlap with all citizens, Zombie
        list<Actor*>::iterator itr = m_actors.begin();
        
        while (itr != m_actors.end()) {
            Actor*a = *itr;
            if (a->isAlive()) {
                if (a->canBecomeZombie()) {
                    if (checkfor != a) {
                        td = findEucleadeanDistance (x, y, a->getX(), a->getY());
                        if (td <= targetDistance * targetDistance) {
                            if (fexit) {
                                a->setExited (fexit);
                                a->setAliveStatus(fexit);
                            }
                            if (infect) {
                                int ic = a->getInfectionCount();
                                a->setInfectionCount(ic+1);
                            }
                            ret = true;
                        }
                    }
                }
            } //if alive
            itr++;
        } //while
    }
    return ret;
}

bool StudentWorld::areAllCitizensDeadOrExited() {
    list<Actor*>::iterator itr = m_actors.begin();
    
    while (itr != m_actors.end()) {
        Actor*a = *itr;
        if (a->isAlive()) {
            if (a->canBeInfected()) { //TODO::::change
                if (a != m_Penelope) {
                    if (a->isAlive() || !a->hasExited() || !a->hasBeenDestroyed())
                        return false;
                }
            }
            itr++;
        }
    }

    return true;
}

void StudentWorld::setPlayerExit (bool fexit) {
    m_Penelope->setExited (fexit);
}

void StudentWorld::moveZombie(Actor* checkfor) {
    Actor* min_actor = m_Penelope;
    double dp_sq = findEucleadeanDistance (checkfor->getX(), checkfor->getY(), m_Penelope->getX(), m_Penelope->getY());
    double temp_sq = dp_sq, min_sq = dp_sq;
    
    list<Actor*>::iterator itr = m_actors.begin();
    
    while (itr != m_actors.end()) {
        Actor*a = *itr;
        if (a->isAlive() || !a->hasExited() || !a->hasBeenDestroyed()) {
            if (a->canBeInfected()) { //TODO::::change
                temp_sq = findEucleadeanDistance (checkfor->getX(), checkfor->getY(), a->getX(), a->getY());
                if (temp_sq < min_sq) {
                    min_sq = temp_sq;
                    min_actor = a;
                }
            }
        }
        itr++;
    }
    
    //set direction
    if (min_sq > 80*80) {
        int i = randInt(1, 4);
        if (i == 1) checkfor->setDirection (checkfor->right);
        if (i == 2) checkfor->setDirection (checkfor->left);
        if (i == 3) checkfor->setDirection (checkfor->up);
        if (i == 4) checkfor->setDirection (checkfor->down);
    } else { //opposite to nearest person
        
        double diffx = checkfor->getX() - min_actor->getX();
        double diffy = checkfor->getY() - min_actor->getY();
       
        //on same row or column
        if (checkfor->getX() == min_actor->getX()) {
            if (diffx < 0)
                checkfor->setDirection (checkfor->right);
            else
                checkfor->setDirection (checkfor->left);
            return;
        }

        if (checkfor->getY() == min_actor->getY()) {
            if (diffy < 0)
                checkfor->setDirection (checkfor->up);
            else
                checkfor->setDirection (checkfor->down);
            return;
        }
        
        //get closer
        if (abs(diffx) < abs(diffy)) {
            if (diffx < 0)
                checkfor->setDirection (checkfor->right);
            else
                checkfor->setDirection (checkfor->left);
        } else {
            if (diffy < 0)
                checkfor->setDirection (checkfor->up);
            else
                checkfor->setDirection (checkfor->down);
        }
        
    }
}

void StudentWorld::moveCitizen (Actor* a, double dist_p, double dist_z) {
    //P44.6.a, b
    int hdiff = a->getX() - m_Penelope->getX();
    int vdiff = a->getY() - m_Penelope->getY();
    Direction newdirection = a->left;
    int d  = -1;
    int distanceOfMove = 2;
    bool blocked = false;
    
    if ((dist_p < dist_z  || dist_z == 999999.0) && (dist_p < 80*80)) {
        if (hdiff == 0 || vdiff == 0) {
            if (hdiff == 0) { //find is it left or right penelope is, so try move in that direction -- on same row
                newdirection = (vdiff > 0 ? a->down : a->up);
                //see if there is block
                blocked = checkBoundingBoxOverlap(a, a->getX(), a->getY(), newdirection, 2, true);
                if (!blocked) {
                    a->setDirection(newdirection);
                    if (newdirection == a->down)
                        a->moveTo(a->getX(), a->getY() - distanceOfMove);
                    else if (newdirection == a->up)
                        a->moveTo(a->getX(), a->getY() + distanceOfMove);
                    return;
                }
            }
            
            //same column
            if (vdiff == 0) { //find is it left or right penelope is, so try move in that direction -- on same row
                newdirection = (hdiff > 0 ? a->left : a->right);
                //see if there is block
                blocked = checkBoundingBoxOverlap(a, a->getX(), a->getY(), newdirection, 2, true);
                if (!blocked) {
                    a->setDirection(newdirection);
                    if (newdirection == a->left)
                        a->moveTo(a->getX() - distanceOfMove , a->getY());
                    else if (newdirection == a->right)
                        a->moveTo(a->getX() + distanceOfMove, a->getY());
                    return;
                }
            }
        }
        
        else if (hdiff != 0 && vdiff != 0){
            //not on same row or column
            Direction hd;
            Direction vd;
            if (hdiff > 0)
                hd = a->left;
            else
                hd = a->right;
            
            if (vdiff > 0)
                vd = a->down;
            else
                vd = a->up;
            
            // chossing random direction
            int r = randInt (1, 2);
            if (r == 1) {
                blocked = checkBoundingBoxOverlap(a, a->getX(), a->getY(), hd, 2, true);
                if (!blocked) {
                    a->setDirection(hd);
                    if (hd == a->left)
                        a->moveTo(a->getX() - distanceOfMove , a->getY());
                    else if (hd == a->right)
                        a->moveTo(a->getX() + distanceOfMove , a->getY());
                    return;
                } else {
                    //wither blocked or came as second choise
                    blocked = checkBoundingBoxOverlap(a, a->getX(), a->getY(), vd, 2, true);
                    if (!blocked) {
                        a->setDirection(vd);
                        if (vd == a->down)
                            a->moveTo(a->getX() , a->getY()- distanceOfMove);
                        else if (vd == a->up)
                            a->moveTo(a->getX() , a->getY()+ distanceOfMove);
                        return;
                    }
                }
            }
            if (r == 2) {
                //wither blocked or came as second choise
                blocked = checkBoundingBoxOverlap(a, a->getX(), a->getY(), vd, 2, true);
                if (!blocked) {
                    a->setDirection(vd);
                    if (vd == a->down)
                        a->moveTo(a->getX() , a->getY()- distanceOfMove);
                    else if (vd == a->up)
                        a->moveTo(a->getX() , a->getY()+ distanceOfMove);
                    return;
                } else {
                    blocked = checkBoundingBoxOverlap(a, a->getX(), a->getY(), hd, 2, true);
                    if (!blocked) {
                        a->setDirection(hd);
                        if (hd == a->left)
                            a->moveTo(a->getX() - distanceOfMove , a->getY());
                        else if (hd == a->right)
                            a->moveTo(a->getX() + distanceOfMove , a->getY());
                        return;
                    }
                }
            }
        }
    }
    
    else {
        //no movement yet, point 7
        Actor* min_actor = m_Penelope;
        double dp_sq = findEucleadeanDistance (a->getX(), a->getY(), m_Penelope->getX(), m_Penelope->getY());
        double temp_sq = dp_sq, min_sq = dp_sq;
        
        list<Actor*>::iterator itr = m_actors.begin();
        
        while (itr != m_actors.end()) {
            Actor* it = *itr;
            if (it->isAlive() || !it->hasExited() || !it->hasBeenDestroyed()) {
                if (!it->canBeInfected() && it->canBecomeZombie()) { //TODO::::change
                    temp_sq = findEucleadeanDistance (a->getX(), a->getY(), it->getX(), it->getY());
                    if (temp_sq < min_sq) {
                        min_sq = temp_sq;
                        min_actor = it;
                    }
                }
            }
            itr++;
        }
        
        if (min_sq <= 80*80) {
            double hdiff = a->getX() - min_actor->getX();
            double vdiff = a->getY() - min_actor->getY();
            
            bool lb, rb, ub, db;
            //left
            lb = checkBoundingBoxOverlap(a, a->getX(), a->getY(), a->left, 2, true);
            rb = checkBoundingBoxOverlap(a, a->getX(), a->getY(), a->right, 2, true);
            ub = checkBoundingBoxOverlap(a, a->getX(), a->getY(), a->up, 2, true);
            db = checkBoundingBoxOverlap(a, a->getX(), a->getY(), a->down, 2, true);
            
            if (lb || rb || ub || db)
                return;
            
            //lets find out whuch will take farthest
            if (abs(hdiff) > abs(vdiff)) {
                if (hdiff > 0) {
                    if (!lb)
                        a->moveTo(a->getX() + distanceOfMove , a->getY());
                    else if (!rb)
                        a->moveTo(a->getX() - distanceOfMove , a->getY());
                }
                if (vdiff > 0) {
                    if (!ub)
                        a->moveTo(a->getX() , a->getY()- distanceOfMove);
                    else if (!db)
                        a->moveTo(a->getX()  , a->getY() + distanceOfMove);
                }
            }

        }
    }
    return;

}



//P17 - Framework calls this method
int StudentWorld::init()
{
    if (getLevel() > 99)
        return GWSTATUS_PLAYER_WON;
    //gets length of integer
    int w = getNumWidth(getLevel());
    if (w == 1)  w++;
    ostringstream oss;
    oss.fill('0');
    oss << setw(w) << getLevel();

    //creates string with level file name
    string fn = "level" + oss.str() + ".txt";
    
    //loads level
    Level::LoadResult lr = loadLevel(fn);
    if (lr == Level::LoadResult::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    if (lr == Level::LoadResult::load_fail_file_not_found)
        return GWSTATUS_PLAYER_WON;
    
    Actor* a = nullptr;
    
    //based on level file, creates the game setup with the walls, Penelope, exti and other objects in the correct place and orientation
    // inserts these actors into the list m_actors
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        for (int y = 0; y < LEVEL_HEIGHT; y++) {
            Level::MazeEntry me = m_level->getContentsOf(x, y);
            switch (me) {
                case Level::MazeEntry::player: {
                    m_Penelope = new Penelope(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                    m_Penelope->setGameWorld(this);
                    break;
                }
                case Level::MazeEntry::wall: {
                    a = new Wall(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                    a->setGameWorld(this);
                    m_actors.push_back(a);
                    break;
                }
                case Level::MazeEntry::pit: {
                    
                    a = new Pit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                    a->setGameWorld(this);
                    m_actors.push_back(a);
                    break;
                    
                }
                case Level::MazeEntry::vaccine_goodie: {
                    createActor (me, x, y);
                    break;
                }
        
                case Level::MazeEntry::gas_can_goodie: {
                    createActor (me, x, y);
                    break;
                }
                case Level::MazeEntry::landmine_goodie: {
                    createActor (me, x, y);
                    break;
                }
                case Level::MazeEntry::exit: {
                    a = new Exit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                    a->setGameWorld(this);
                    m_actors.push_back(a);
                    break;
                }
                case Level::MazeEntry::dumb_zombie: {
                    a = new DumbZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                    a->setGameWorld(this);
                    m_actors.push_back(a);
                    break;
                }
                case Level::MazeEntry::smart_zombie: {
                    a = new SmartZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                    a->setGameWorld(this);
                    m_actors.push_back(a);
                    break;
                }
                case Level::MazeEntry::citizen: {
                    a = new Citizen(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
                    a->setGameWorld(this);
                    m_actors.push_back(a);
                    break;
                }
               default:
                    break;
            }
        }
    }
    //setTick(-(getTick()));
    return GWSTATUS_CONTINUE_GAME;
}

//P18 - Framework calls this method each tick
int StudentWorld::move()
{
    //setTick(1);
    //checks if Penelope has exited
    if (m_Penelope->hasExited()) {
        playSound(SOUND_LEVEL_FINISHED);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    // tells Penelope to do something if she is alive
    if (m_Penelope->isAlive()) {
        m_Penelope->doSomething();
    } else {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    list<Actor*>::iterator ibegin = m_actors.begin();
    
    // tels every actor on the list to do something if they are alive. If they're dead, it erases them from the list.
    //int i = 0;
    while (ibegin != m_actors.end()) {
        Actor*a = *ibegin;
         if (a->isAlive()) {
            a->doSomething();
            ibegin++;
        } else {
            
            if (a->isDumbZombie()) {
                int i = randInt(1, 10);
                if (i == 2) {
                    int j = randInt(1, 4);
                    switch (j) {
                        case 1:
                            if (m_level->getContentsOf(a->getX() + SPRITE_WIDTH, a->getY()) == Level::MazeEntry::empty)
                                createNewVaccine(a->getX() + SPRITE_WIDTH, a->getY());
                            break;
                        case 2:
                            if (m_level->getContentsOf(a->getX() - SPRITE_WIDTH, a->getY()) == Level::MazeEntry::empty)
                                createNewVaccine(a->getX() - SPRITE_WIDTH, a->getY());
                            break;
                        case 3:
                            if (m_level->getContentsOf(a->getX(), a->getY() + SPRITE_HEIGHT) == Level::MazeEntry::empty)
                                createNewVaccine(a->getX(), a->getY() + SPRITE_HEIGHT);
                            break;
                        case 4:
                            if (m_level->getContentsOf(a->getX(), a->getY() - SPRITE_HEIGHT) == Level::MazeEntry::empty)
                                createNewVaccine(a->getX(), a->getY() - SPRITE_HEIGHT);
                            break;
                        default:
                            break;
                    }
                }
            }
            
            delete(*ibegin);
            m_actors.erase(ibegin++);
        }
    }
    
    // prints line with lives and other game stats on top
    ostringstream ods;
    ods << "Score: \t" << getScore() << "\t\t Level: \t" << getLevel() << "\t\t Lives: \t" << getLives() << "\t\t Vacc: \t" << m_Penelope->getVaccines();
    ods << "\t\t Flames: \t" << m_Penelope->getFlameThrower() << "\t\t Mines: \t" << m_Penelope->getLandmines();
    ods << "\t\t Infected: \t"<< m_Penelope->getInfectionCount();
    setGameStatText(ods.str());
    
    return GWSTATUS_CONTINUE_GAME;
}

//cleanup is called by framework
void StudentWorld::cleanUp()
{
    //deletes all actors and deletes actor pointers form list
    list<Actor*> :: iterator it = m_actors.begin();
    while (it != m_actors.end()) {
        delete (*it);
        m_actors.erase(it++);
    }
    
    // deletes m_level and m_Penelope
    delete m_level;
    delete m_Penelope;
}

//private
Level::LoadResult StudentWorld::loadLevel(string assetFile) {
    m_level = new Level(assetPath());
    return m_level->loadLevel(assetFile);
}

//private
// recursive function to get length of number
int StudentWorld::getNumWidth(int num) {
    if (num < 10)
        return 1;
    return 1 + getNumWidth(num/10);
}

void StudentWorld::checkOverlap(Actor* e, Level::MazeEntry me1, Level::MazeEntry me2) {
    if ((me1 == Level::MazeEntry::player) && (me2 == Level::MazeEntry::exit)) {
        if ( e->doesOverlap (m_Penelope->getX(), m_Penelope->getY(), e->getX(), e->getY() )) {
            //if (allcitisens died or saved)
            m_Penelope->setExited(true);
            
        }
    } else if ((me1 == Level::MazeEntry::citizen) && (me2 == Level::MazeEntry::exit)) {

    }
}

Level* StudentWorld::getLevelObject() const {
    return m_level;
}


void StudentWorld::createActor (Level::MazeEntry me, int x, int y) {
    Actor *a;
    if (me == Level::MazeEntry::dumb_zombie) {
        a = new DumbZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
    } else if (me == Level::MazeEntry::smart_zombie) {
        a = new SmartZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
    } else if (me == Level::MazeEntry::vaccine_goodie) {
        a = new Vaccine(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
    } else if (me == Level::MazeEntry::gas_can_goodie) {
        a = new GasCan(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
    } else if (me == Level::MazeEntry::landmine_goodie) {
        a = new LandmineGoodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
    } else if (me == Level::MazeEntry::pit) {
        a = new Pit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
    }
    a->setGameWorld(this);
    m_actors.push_back(a);
}



void StudentWorld::createVomit (int x, int y) {
    Actor* a = new Vomit (x, y);
    a->setGameWorld(this);
    m_actors.push_back(a);
    
}

void StudentWorld::createNewFlame(int destX, int destY , Direction dir) {
    Actor* a = new Flame(destX, destY, dir);
    a->setGameWorld(this);
    m_actors.push_back(a);
}


void StudentWorld::createNewLandmine(int destX, int destY) {
    Actor* a = nullptr;
    a = new Landmine(destX, destY);
    a->setGameWorld(this);
    m_actors.push_back(a);
}

void StudentWorld::createNewVaccine(int destX, int destY) {
    Actor* a = nullptr;
    a = new Vaccine(destX, destY);
    a->setGameWorld(this);
    m_actors.push_back(a);
}

bool StudentWorld::doSomethingForGoodies(Actor *a) {
    if (!a->isAlive())
        return false;
    
    if (checkIfOverlap (a, a->getX(), a->getY(), 10, Level::MazeEntry::player, false, false, false)) {
        increaseScore(50);
        a->setAliveStatus(false);
        playSound(SOUND_GOT_GOODIE);
        return true;
    }
    return false;
}


void StudentWorld::addVaccine(int amt) {
    m_Penelope->setVaccines(m_Penelope->getVaccines() + amt);
}

void StudentWorld::addFlames(int amt) {
    m_Penelope->setFlameThrower(m_Penelope->getFlameThrower() + amt);
}

void StudentWorld::addLandmines(int amt) {
    m_Penelope->setLandmines(m_Penelope->getLandmines() + amt);
}

bool StudentWorld::KilledByPit (Actor* checkfor, int x, int y, int targetDistance) {
    double td;
    bool ret = false;
    
    if (checkfor != m_Penelope && m_Penelope->canBeKilledByPit() && m_Penelope->isAlive()) { //if self check, return
        td = findEucleadeanDistance (x, y, m_Penelope->getX(), m_Penelope->getY());
        if (td <= (targetDistance * targetDistance)) {
            if (m_Penelope->canBeKilledByPit()) {
                m_Penelope->setAliveStatus(false);
            }
            ret = true;
        }
    }
    
    list<Actor*>::iterator itr = m_actors.begin();
    while (itr != m_actors.end()) {
        Actor*a = *itr;
        if (a != checkfor && a->isAlive() && a->canBeKilledByPit()) {
            td = findEucleadeanDistance (x, y, a->getX(), a->getY());
            if (td <= targetDistance * targetDistance) {
                a->setAliveStatus(false);
                ret = true;
            }
        }
        itr++;
    }
    return ret;
}

