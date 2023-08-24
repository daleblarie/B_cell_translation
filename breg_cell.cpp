#include "breg_cell.h"
#include "world.h"

BregCell::BregCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}


void World::bregFunction(std::shared_ptr<BregCell> breg_cell) {
    // Get the patch where the BReg cell is
    Patch& breg_patch = get_patch(breg_cell->getX(), breg_cell->getY());

    // Secretes the cytokines
    breg_patch.setIl10(breg_patch.getIl10() + 5);
    breg_patch.setTgfB(breg_patch.getTgfB() + 1);

    // Chemotaxis and move
    chemotaxis(breg_cell);
    move_turtle(breg_cell);

    // Check TNF status
    bool die_by_tnf = checkTNFStatus(breg_cell);

    // Increase the time alive
    breg_cell->setTimeAlive(breg_cell->getTimeAlive() + 1);

    // Kill if the time alive exceeds 300
    if((breg_cell->getTimeAlive() > 300)|| die_by_tnf) {
        kill(breg_cell);
    }
}
