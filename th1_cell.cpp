#include "th1_cell.h"
#include "world.h"

Th1Cell::Th1Cell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}

void World::th1CellFunction(std::shared_ptr<Th1Cell> th1_cell) {
    Patch& current_patch = get_patch(th1_cell->getX(), th1_cell->getY());

    chemotaxis(th1_cell);
    move_turtle(th1_cell);

    // Secretes the following cytokines
    current_patch.setIfG(current_patch.getIfG() + 1);

    th1_cell->setTimeAlive(th1_cell->getTimeAlive() + 1);

    // Uncomment the following lines if you want the Th1 cells to die after being alive for more than 500 time steps.
    //if (th1_cell->getTimeAlive() > 500) {
    //    kill(th1_cell);
    //}
}
