#include "bacteria.h"
#include "world.h"
Bacteria::Bacteria(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}


void World::bacteriaFunction(std::shared_ptr<Bacteria> bacteria) {
    Patch& current_patch = get_patch(bacteria->getX(), bacteria->getY());

    // Check if the current patch type is 2
    if (current_patch.getPatchType() == 2) {
        int x = rand() % 2; // Randomly generate 0 or 1
        if (x == 0) {
            bacteria->setX(49);
            bacteria->setY(0);
            bacteria->setS1pr1Level(0);
        } else {
            auto available_fdcs = get_fdcs_with_no_presented_antigen(bacteria->getX(), bacteria->getY());
            if (!available_fdcs.empty()) {
                auto fdc = available_fdcs[rand() % available_fdcs.size()];
                fdc->setTimePresenting(0);
                fdc->setPresentedAntigen(bacteria->getEpitopeType());
                fdc->setColor("red");
                int rTI = rand() % bacteria->getNumTIep();
                int rTD = rand() % bacteria->getNumTDep();
                if (rTI > rTD) {
                    fdc->setPresentedAntigen(1);
                } else {
                    fdc->setPresentedAntigen(2);
                }
                kill(bacteria);
            }
        }
    }

    bacteria->setS1pr1Level(bacteria->getS1pr1Level() + 0.004);
    chemotaxis(bacteria);
    move_turtle(bacteria);
}
