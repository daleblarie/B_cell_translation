#include "activated_b_cell.h"
#include "world.h"

ActivatedBCell::ActivatedBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}


void World::activatedBCellFunction(std::shared_ptr<ActivatedBCell> abcell) {
    // Only performs the following commands if currently inside the follicle, and NOT in the blood/lymph
    if (!abcell->getInBlood()) {
        // Get the patch where the B cell is
        Patch& bcell_patch = get_patch(abcell->getX(), abcell->getY());

        // If the cell reaches the follicle exit
        if(bcell_patch.getPatchType() == 2) {
            abcell->setInBlood(true);
            abcell->setVisible(false);
        }

        isotypeSwitch(abcell);   // Determines which isotype to switch to

        if (abcell->getResponseType() == 2) {
            tdResponse(abcell);
        } else if (abcell->getResponseType() == 1) {
            tiResponse(abcell);
        }

        //abcell->checkBregStatus();

        abcell->chemotaxis();
        move_turtle(abcell);
    }

    checkTNFStatus(abcell);

    abcell->setAge(abcell->getAge() + 1);
    if (abcell->getAge() > 300) {
        kill(abcell);
    }
}

void World::isotypeSwitch(std::shared_ptr<ActivatedBCell> activated_b_cell) {
    // Get the patch where the activated b cell is
    Patch& activated_b_cell_patch = get_patch(activated_b_cell->getX(), activated_b_cell->getY());

    if (!activated_b_cell->getCsrBool()) {
        double igM_bucket = activated_b_cell_patch.getIl12() + activated_b_cell_patch.getIl15() + activated_b_cell_patch.getIl6();
        double igA_bucket = activated_b_cell_patch.getIl10() + activated_b_cell_patch.getIl15() + activated_b_cell_patch.getIl21() + activated_b_cell_patch.getTgfB();
        double igG_bucket = activated_b_cell_patch.getIl4() + activated_b_cell_patch.getIl10() + activated_b_cell_patch.getIl15() + activated_b_cell_patch.getIl21();
        double igE_bucket = activated_b_cell_patch.getIl4() - activated_b_cell_patch.getIl12() - activated_b_cell_patch.getIfA() - activated_b_cell_patch.getIfG() - activated_b_cell_patch.getTgfB() + activated_b_cell_patch.getIl21();

        std::vector<double> mylist = {igM_bucket, igA_bucket, igG_bucket, igE_bucket};
        int max_index = std::max_element(mylist.begin(), mylist.end()) - mylist.begin();

        if (max_index == 0) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("m");
        } else if (max_index == 1) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("a");
        } else if (max_index == 2) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("g");
        } else if (max_index == 3) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("e");
        }
    }
}




void World::tdResponse(std::shared_ptr<ActivatedBCell> activated_b_cell) {
    // Try to get Tfh cell in the current location of the activated B cell
    auto tfh = getOneTfhHere(activated_b_cell->getX(), activated_b_cell->getY());

    // If a Tfh cell is found, create a new GcB cell and link it with the Tfh cell
    if (tfh) {
        auto new_gc_b_cell = std::make_shared<GCBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++, activated_b_cell->getHeading());
        new_gc_b_cell->setProBreg(0);
        new_gc_b_cell->setColor("orange");
        new_gc_b_cell->setShape("circle");
        new_gc_b_cell->setSize(1);
        new_gc_b_cell->setTimeAlive(0);

        std::weak_ptr<Turtle> new_gc_b_cell_weak_ptr = new_gc_b_cell;
        all_turtles.push_back(new_gc_b_cell_weak_ptr);
        all_gcb_cells.push_back(new_gc_b_cell);

        // createLinkWith(new_gc_b_cell, tfh);
        tfh->setEbi2rLevel(0);
        tfh->setCcr7Level(0);
        tfh->setBcellBindingStatus(true);

        kill(activated_b_cell);
    } else {
        // If no Tfh cell is found, try to get a Th2 cell in the current location of the activated B cell
        auto th2 = getOneTh2Here(activated_b_cell->getX(), activated_b_cell->getY());

        // If a Th2 cell is found, create a new GcB cell and link it with the Th2 cell
        if (th2) {
            auto new_gc_b_cell = std::make_shared<GCBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++, activated_b_cell->getHeading());
            new_gc_b_cell->setProBreg(0);
            new_gc_b_cell->setColor("orange");
            new_gc_b_cell->setShape("circle");
            new_gc_b_cell->setSize(1);
            new_gc_b_cell->setTimeAlive(0);

            std::weak_ptr<Turtle> new_gc_b_cell_weak_ptr = new_gc_b_cell;
            all_turtles.push_back(new_gc_b_cell_weak_ptr);
            all_gcb_cells.push_back(new_gc_b_cell);

            // createLinkWith(new_gc_b_cell, th2);
            th2->setEbi2rLevel(0);
            th2->setCcr7Level(0);
            th2->setBcellBindingStatus(true);

            kill(activated_b_cell);
        }
    }
}


void World::tiResponse(std::shared_ptr<ActivatedBCell> activated_b_cell) {
    // Activated B-cells undergoing TI response secrete TNF-a
    auto current_patch = get_patch(activated_b_cell->getX(), activated_b_cell->getY());
    current_patch.setTnfA(current_patch.getTnfA() + 1);

    if (activated_b_cell->getTimeAlive() % 70 == 0) {
        double proPC = (current_patch.getIl21() + current_patch.getIl10() + current_patch.getIfA() + current_patch.getIfG()) * 10;
        double proMem = 0;
        if (proPC > proMem) {
            // Create a new SL Plasma Cell and add it to the corresponding containers
            auto new_sl_plasma_cell = std::make_shared<SLPlasmaCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++);
            new_sl_plasma_cell->setAge(0);
            // Set color, shape, size, etc. as required
            // new_sl_plasma_cell->setColor("lime + 3");
            new_sl_plasma_cell->setShape("circle");
            new_sl_plasma_cell->setSize(1);


            std::weak_ptr<Turtle> new_sl_plasma_cell_weak_ptr = new_sl_plasma_cell;
            all_turtles.push_back(new_sl_plasma_cell_weak_ptr);
            all_sl_plasma_cells.push_back(new_sl_plasma_cell);
        } else {
            if (step < 2800) {
                // Create a new Memory B Cell and add it to the corresponding containers
                auto new_mem_b_cell = std::make_shared<MemBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++);

                new_mem_b_cell->setExposureNumber(1);
                new_mem_b_cell->setAge(0);
                // Set color, shape, size, etc. as required
                new_mem_b_cell->setColor("white");
                new_mem_b_cell->setShape("target");
                new_mem_b_cell->setSize(1);
                new_mem_b_cell->setS1pr1Level(10);
                new_mem_b_cell->setProBreg(0);
                new_mem_b_cell->setCd21Level(0);
                new_mem_b_cell->setCxcr5Level(10);

                std::weak_ptr<Turtle> new_mem_b_cell_weak_ptr = new_mem_b_cell;
                all_turtles.push_back(new_mem_b_cell_weak_ptr);
                all_mem_b_cells.push_back(new_mem_b_cell);
            } else {
              // Create a new Memory B Cell and add it to the corresponding containers
              auto new_mem_b_cell = std::make_shared<MemBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++);

              new_mem_b_cell->setExposureNumber(2);
              new_mem_b_cell->setAge(0);
              // Set color, shape, size, etc. as required
              new_mem_b_cell->setColor("white");
              new_mem_b_cell->setShape("target");
              new_mem_b_cell->setSize(1);
              new_mem_b_cell->setS1pr1Level(10);
              new_mem_b_cell->setProBreg(0);
              new_mem_b_cell->setCd21Level(0);
              new_mem_b_cell->setCxcr5Level(10);
            }
        }
    }
}
