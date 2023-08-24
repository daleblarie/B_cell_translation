#include "antibodies.h"
#include "world.h"

Antibodies::Antibodies(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}


void World::antibodiesFunction(std::shared_ptr<Antibodies> antibody) {
    // Increase the time alive of the antibody
    antibody->setTimeAlive(antibody->getTimeAlive() + 1);

    // Check if the antibody has exceeded its lifespan
    if (antibody->getTimeAlive() > 900) {
        // If it has, kill the antibody
        kill(antibody);
    }
}
