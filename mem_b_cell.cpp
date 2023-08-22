#include "mem_b_cell.h"
#include "world.h"

MemBCell::MemBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}

void World::memBCellFunction(std::shared_ptr<MemBCell> mem_b_cell) {
  // Get the current patch of the Memory B cell
  Patch& current_patch = get_patch(mem_b_cell->getX(), mem_b_cell->getY());

  // Calculate CD21 expression based on il6 and il10 levels in the current patch
  int cd21_level = 300 - (current_patch.getIl6() * 50 + current_patch.getIl10() * 0);
  mem_b_cell->setCd21Level(cd21_level);

  // If the memory B cell is in the follicle
  if(current_patch.getPatchType() == 2) {
    mem_b_cell->setProBreg(0);
    mem_b_cell->setS1pr1Level(0);
    mem_b_cell->setTimeInFollicle(0);
  }

  mem_b_cell->setTimeInFollicle(mem_b_cell->getTimeInFollicle() + 1);
  if(mem_b_cell->getTimeInFollicle() > 300) {
    mem_b_cell->setS1pr1Level(mem_b_cell->getS1pr1Level() + 0.5);
  }

  mem_b_cell->setTimeAlive(mem_b_cell->getTimeAlive() + 1);
  if(mem_b_cell->getTimeAlive() > 15000) {
    kill(mem_b_cell);
  }

  auto apc = getOneFDCHere(mem_b_cell->getX(), mem_b_cell->getY());
  int random_encounter = RNG_Engine() % 100;
  auto antigen = getOneBacteriaHere(mem_b_cell->getX(), mem_b_cell->getY());

  if((apc != nullptr && apc->getPresentedAntigen() != 0 && apc->getResponsiveness() > random_encounter) || antigen != nullptr){
    if(mem_b_cell->getCd21Level() > BCELL_CD21_ACTIVATION_THRESHOLD){

      auto new_activated_b_cell = std::make_shared<ActivatedBCell>(mem_b_cell->getX(), mem_b_cell->getX(), global_ID_counter, mem_b_cell->getHeading());
      global_ID_counter++;
      current_patch.setIl6(current_patch.getIl6() + PHAG_IL6_BURST);
      new_activated_b_cell->setProBreg(0);
      new_activated_b_cell->setShape("circle");
      new_activated_b_cell->setSize(1);
      new_activated_b_cell->setCsrBool(false);
      new_activated_b_cell->setTimeAlive(100);

      int rTI = RNG_Engine() % 100;
      int rTD = RNG_Engine() % 100;

      if(antigen != nullptr){
        if(rTI > rTD){
          new_activated_b_cell->setResponseType(1); // TI response
        } else{
          new_activated_b_cell->setResponseType(2); // TD response
          new_activated_b_cell->setCcr7Level(12);
          new_activated_b_cell->setEbi2rLevel(12);
        }
        kill(antigen);
      }
      else if (apc != nullptr){
        if(rTI > rTD){
            new_activated_b_cell->setResponseType(1); // TI response
        }
        else{
            new_activated_b_cell->setResponseType(2); // TD response
            new_activated_b_cell->setCcr7Level(12);
            new_activated_b_cell->setEbi2rLevel(12);
        }
      }

      std::weak_ptr<Turtle> new_activated_b_cell_weak_ptr = new_activated_b_cell;
      all_turtles.push_back(new_activated_b_cell_weak_ptr);
      all_activated_b_cells.push_back(new_activated_b_cell);

      kill(mem_b_cell);
      std::shared_ptr<ActivatedBCell> mem_b_cell = new_activated_b_cell;
    }
  }
  // Checks level of stimulation of b-reg differentiation
  bool turn_into_breg = checkBregStatus(mem_b_cell);
  if (turn_into_breg){std::shared_ptr<BregCell> mem_b_cell = turnIntoBreg(mem_b_cell);};

  chemotaxis(mem_b_cell);
  move_turtle(mem_b_cell);


  // Checks level of TNF-a stimulation for apoptosis
  if(checkTNFStatus(mem_b_cell)){kill(mem_b_cell);};
}
