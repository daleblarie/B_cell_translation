#include "naive_b_cell.h"
#include "world.h"
#include "parameters.h"

NaiveBCell::NaiveBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}


void World::naiveBCellFunction(std::shared_ptr<NaiveBCell> naive_b_cell) {
  // Get the current patch of the naive B cell
  Patch& current_patch = get_patch(naive_b_cell->getX(), naive_b_cell->getY());

  // Calculate CD21 expression based on il6 and il10 levels in the current patch
  int cd21_level = 300 - (current_patch.getIl6() * 50 + current_patch.getIl10() * 0);
  naive_b_cell->setCd21Level(cd21_level);

  // If naive B cell is in the follicle, it dies
  if (current_patch.getPatchType() == 2) {
    kill(naive_b_cell);
  }
  auto apc = getOneFDCHere(naive_b_cell->getX(), naive_b_cell->getY());
  int random_encounter = RNG_Engine() % 100;
  auto antigen = getOneBacteriaHere(naive_b_cell->getX(), naive_b_cell->getY());

  if ((apc != nullptr && apc->getPresentedAntigen() != 0 && apc->getResponsiveness() > random_encounter) || antigen != nullptr){
    if(naive_b_cell->getCd21Level() > bcell_cd21_activation_threshold){
      
      // creating a new activated_b_cell
      auto new_activated_b_cell = std::make_shared<ActivatedBCell>(naive_b_cell->getX(), naive_b_cell->getX(), global_ID_counter, naive_b_cell->getID());
      global_ID_counter++;
      current_patch.setIl6(current_patch.getIl6() + phag_il6_burst);
      new_activated_b_cell->setProBreg(0);
      new_activated_b_cell->setShape("circle");
      new_activated_b_cell->setSize(1);
      new_activated_b_cell->setCsrBool(false);
      new_activated_b_cell->setTimeAlive(0);
      
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
      
      kill(naive_b_cell);
      std::shared_ptr<ActivatedBCell> naive_b_cell = new_activated_b_cell;
    }
  }
  
  
  
  naive_b_cell->chemotaxis();
  move_turtle(naive_b_cell);

  // Checks level of stimulation of b-reg differentiation
  naive_b_cell->check_breg_status();

  // Checks level of TNF-a stimulation for apoptosis
  naive_b_cell->check_tnf_status();

  // this slowly increases the # of s1p receptors (s1pr) in the naive b cell when the b-cell is old enough
  if(naive_b_cell->getAge() > 300)
  {
      naive_b_cell->setS1pr1Level(naive_b_cell->getS1pr1Level() + 0.5);
  }

  naive_b_cell->setAge(naive_b_cell->getAge() + 1);

  // Checks if the cell has lived beyond its maximum lifespan, if so, it will be removed
  if(naive_b_cell->getAge() > 1000)
  {
      kill(naive_b_cell);
  }
  

}
