#include "antibodies.h"
#include "world.h"

Antibodies::Antibodies(int x, int y, int id, int heading) : Turtle(x, y, id, heading), type(2) {
    // Constructor
}


void World::antibodiesFunction(std::shared_ptr<Antibodies> antibodies) {
    // Increase the time alive of the antibody
    antibodies->setTimeAlive(antibodies->getTimeAlive() + 1);
    
    // Check if the antibody has exceeded its lifespan
    if (antibodies->getTimeAlive() > 900) {
        // If it has, kill the antibody
        kill(antibodies);
    }
}