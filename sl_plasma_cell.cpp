#include "sl_plasma_cell.h"
#include "world.h"

SLPlasmaCell::SLPlasmaCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}

void World::sl_plasma_cell_function(std::shared_ptr<SLPlasmaCell> sl_plasma_cell) {
  Patch& current_patch = get_patch(sl_plasma_cell->getX(), sl_plasma_cell->getY());
    if(!sl_plasma_cell->getInBlood()) {

        if(current_patch.getPatchType() == 2) {
            sl_plasma_cell->setInBlood(true);
            sl_plasma_cell->setVisible(false);
        }

        // Assuming you have a check_breg_status function
        bool turn_into_breg = checkBregStatus(sl_plasma_cell);
        if (turn_into_breg){std::shared_ptr<BregCell> sl_plasma_cell = turnIntoBreg(sl_plasma_cell);};

        chemotaxis(sl_plasma_cell);
        sl_plasma_cell->move();
    }

    if(sl_plasma_cell->getTimeAlive() % 50 == 0) {
        auto antibody = std::make_shared<Antibodies>(sl_plasma_cell->getX(), sl_plasma_cell->getX(), global_ID_counter++, sl_plasma_cell->getHeading());
        antibody->copy_other_turtle_attributes(sl_plasma_cell);
        antibody->setTimeAlive(0);
        antibody->setAntibodyType(sl_plasma_cell->getIsotype());
        antibody->setVisible(false);
        
        std::weak_ptr<Turtle> antibody_weak_ptr = antibody;
        all_turtles.push_back(antibody_weak_ptr);
        all_antibodies.push_back(antibody);
        current_patch.add_turtle(antibody);
    }

    // Checks level of TNF-a stimulation for apoptosis
    bool die_by_tnf = checkTNFStatus(sl_plasma_cell);

    sl_plasma_cell->setTimeAlive(sl_plasma_cell->getTimeAlive() + 1);
    if((sl_plasma_cell->getTimeAlive() > 240 + (current_patch.getIl6() + current_patch.getIl21()) * 10) || die_by_tnf) {
        kill(sl_plasma_cell);
    }
}
