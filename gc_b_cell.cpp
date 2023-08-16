#include "gc_b_cell.h"
#include "world.h"
#include "helper_functions.h"

GCBCell::GCBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}

void World::gc_b_cell_function(std::shared_ptr<GCBCell> gc_b_cell) {
    if(!gc_b_cell->getInBlood()) {
        Patch& current_patch = get_patch(gc_b_cell->getX(), gc_b_cell->getY());

        if(current_patch.getPatchType() == 2) {
            gc_b_cell->setInBlood(true);
            // gc_b_cell->hide_turtle();
        }

        // Downregulates ebi2r and ccr7 so it can localize to follicle center again
        gc_b_cell->setEbi2rLevel(0);
        gc_b_cell->setCcr7Level(0);

        // The GC B-cell only moves if it has not yet reached the follicle center
        if(calculateDistance(WORLD_WIDTH/2, WORLD_HEIGHT/2, gc_b_cell->getX(), gc_b_cell->getY()) > 15) {
            gc_b_cell->chemotaxis();
            move_turtle(gc_b_cell, 0.5);
            // gc_b_cell->gc_move();
        } else {
            int proPC2 = current_patch.getIl21() + current_patch.getIl10() * 2 + current_patch.getIfA() + current_patch.getIfG();
            int proMem2 = current_patch.getIl21() + current_patch.getIl4();
            int proPC = RNG_Engine() % proPC2;
            int proMem = RNG_Engine() % proMem2;

            if(gc_b_cell->getTimeAlive() % 80 == 0) {
                if(proPC > proMem) {
                    auto plasma_cell = std::make_shared<LLPlasmaCell>(gc_b_cell->getX(), gc_b_cell->getY(), gc_b_cell->getHeading());
                    plasma_cell->setExposureNumber(step < 2800 ? 1 : 2);
                    plasma_cell->setAge(0);
                    plasma_cell->setColor("lime");
                    plasma_cell->setShape("circle");
                    plasma_cell->setSize(1);
                    plasma_cell->setS1pr1Level(40);
                    plasma_cell->setProBreg(0);
                    plasma_cell->setTnfaThreshold(plasma_cell->getTnfaThreshold() - 130);
                    std::weak_ptr<Turtle> plasma_cell_weak_ptr = plasma_cell;
                    all_turtles.push_back(plasma_cell_weak_ptr);
                    all_ll_plasma_cells.push_back(plasma_cell);

                } else {
                    auto mem_b_cell = std::make_shared<MemBCell>(gc_b_cell->getX(), gc_b_cell->getY(), gc_b_cell->getHeading());
                    mem_b_cell->setExposureNumber(step < 2800 ? 1 : 2);
                    mem_b_cell->setAge(0);
                    mem_b_cell->setColor("white");
                    mem_b_cell->setShape("target");
                    mem_b_cell->setS1pr1Level(10);
                    mem_b_cell->setProBreg(0);
                    mem_b_cell->setCd21Level(0);
                    mem_b_cell->setCxcr5Level(10);

                    std::weak_ptr<Turtle> mem_b_cell_weak_ptr = mem_b_cell;
                    all_turtles.push_back(mem_b_cell_weak_ptr);
                    all_mem_b_cells.push_back(mem_b_cell);
                }
            }
        }

        //check_breg_status(gc_b_cell);
    }

    checkTNFStatus(gc_b_cell);

    gc_b_cell->setAge(gc_b_cell->getAge() + 1);
    if(gc_b_cell->getAge() > 700) {
        for(auto neighbor : gc_b_cell->getLinkedTurtles()) {
          if (std::shared_ptr<TfhCell> neighbor = std::dynamic_pointer_cast<TfhCell>(neighbor)){
            neighbor->setBcellBindingStatus(false);
          } else if (std::shared_ptr<Th2Cell> neighbor = std::dynamic_pointer_cast<Th2Cell>(neighbor)){
            neighbor->setBcellBindingStatus(false);
          }
        }
        kill(gc_b_cell);
    }
}