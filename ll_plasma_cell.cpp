#include "ll_plasma_cell.h"
#include "world.h"

LLPlasmaCell::LLPlasmaCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}


void World::ll_plasma_cell_function(std::shared_ptr<LLPlasmaCell> ll_plasma_cell) {
  Patch& current_patch = get_patch(ll_plasma_cell->getX(), ll_plasma_cell->getY());
  bool turn_into_breg;
    if(!ll_plasma_cell->getInBlood()) {

        if(current_patch.getPatchType() == 2) {
            ll_plasma_cell->setInBlood(true);
            // ll_plasma_cell->hide_turtle();
        }

        // Assuming you have a check_breg_status function
        turn_into_breg = checkBregStatus(ll_plasma_cell);
        if (turn_into_breg){std::shared_ptr<BregCell> ll_plasma_cell = turnIntoBreg(ll_plasma_cell);};

        // ll_plasma_cell->check_breg_status();

        chemotaxis(ll_plasma_cell);
        ll_plasma_cell->move();
      
    }
    else {
        if(ll_plasma_cell->getTimeAlive() % 200 == 0) {
            auto antibody = std::make_shared<Antibodies>(ll_plasma_cell->getX(), ll_plasma_cell->getX(), global_ID_counter++, ll_plasma_cell->getID());
            antibody->setTimeAlive(0);
            antibody->setAntibodyType(ll_plasma_cell->getIsotype());
            // antibody->setHidden(true);
            
            std::weak_ptr<Turtle> antibody_weak_ptr = antibody;
            all_turtles.push_back(antibody_weak_ptr);
            all_antibodies.push_back(antibody);        }
    }


      // Assuming you have a check_tnf_status function
    bool die_by_tnf = checkTNFStatus(ll_plasma_cell);
    
    ll_plasma_cell->setTimeAlive(ll_plasma_cell->getTimeAlive() + 1);
    if ((ll_plasma_cell->getTimeAlive() > 8000 + (current_patch.getIl6() + current_patch.getIl21()) * 10) || die_by_tnf) {
      kill(ll_plasma_cell);
      
    }
}
