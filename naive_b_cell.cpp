#include "naive_b_cell.h"
#include "world.h"
#include "parameters.h"

NaiveBCell::NaiveBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
}


void World::naiveBCellFunction(std::shared_ptr<NaiveBCell> naive_b_cell) {
  bool print_test = false;

  // std::cout<<"naive_b_cellfunction for ID number "<<naive_b_cell->getID()<<"Ebi2rLevel "<<naive_b_cell->getEbi2rLevel()<<std::endl;
  // Get the current patch of the naive B cell
  Patch& current_patch = get_patch(naive_b_cell->getX(), naive_b_cell->getY());

  // Calculate CD21 expression based on il6 and il10 levels in the current patch
  int cd21_level = 300 - (current_patch.getIl6() * 50 + current_patch.getIl10() * 0);
  naive_b_cell->setCd21Level(cd21_level);

  // If naive B cell is in the follicle, it dies
  if (current_patch.getPatchType() == 2) {
    kill(naive_b_cell);
    // return;
  }
  auto apc = getOneFDCHere(naive_b_cell->getX(), naive_b_cell->getY());
  int random_encounter = RNG_Engine() % 100;
  auto antigen = getOneBacteriaHere(naive_b_cell->getX(), naive_b_cell->getY());

  if ((apc != nullptr && apc->getPresentedAntigen() != 0 && apc->getResponsiveness() > random_encounter) || antigen != nullptr){
    if (print_test){std::cout<<"STEPPING INTO NAIVE_B_CELL FUNCTION"<<std::endl;}
    if(naive_b_cell->getCd21Level() > BCELL_CD21_ACTIVATION_THRESHOLD){
      if(print_test){std::cout<<"becoming activated B_cell"<<std::endl;}
      // creating a new activated_b_cell
      auto new_activated_b_cell = std::make_shared<ActivatedBCell>(naive_b_cell->getX(), naive_b_cell->getX(), global_ID_counter++, naive_b_cell->getHeading());
      new_activated_b_cell->copy_other_turtle_attributes(naive_b_cell);
      current_patch.setIl6(current_patch.getIl6() + PHAG_IL6_BURST);
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
      current_patch.add_turtle(new_activated_b_cell);

      kill(naive_b_cell);
      std::shared_ptr<ActivatedBCell> naive_b_cell = new_activated_b_cell; // renaming naive_b_Cell so the rest of the function still works as expected
    }
  }


  if(print_test){std::cout<<"chomtaxin naive_b_cell"<<std::endl;}
  chemotaxis(naive_b_cell);
  move_turtle(naive_b_cell);

  // Checks level of stimulation of b-reg differentiation
  bool turn_into_breg = checkBregStatus(naive_b_cell);
  if (turn_into_breg){
    std::cout<<"naive_b cell ID "<<naive_b_cell->getID()<<"turning into Breg"<<std::endl;
    // cant do it like below, leads to weird error where orignal cell is overwritten in templated turnIntoBreg function.
    // std::shared_ptr<BregCell> naive_b_cell = turnIntoBreg(naive_b_cell);
    // need to do it like this instead
    std::shared_ptr<BregCell> output_b_cell = turnIntoBreg(naive_b_cell);
    std::shared_ptr<BregCell> naive_b_Cell;
    naive_b_Cell = output_b_cell;
  };

  // Checks level of TNF-a stimulation for apoptosis
  bool die_by_tnf = checkTNFStatus(naive_b_cell);

  // this slowly increases the # of s1p receptors (s1pr) in the naive b cell when the b-cell is old enough
  if(naive_b_cell->getTimeAlive() > 300)
  {
      naive_b_cell->setS1pr1Level(naive_b_cell->getS1pr1Level() + 0.5);
  }

  naive_b_cell->setTimeAlive(naive_b_cell->getTimeAlive() + 1);

  // Checks if the cell has lived beyond its maximum lifespan, if so, it will be removed
  if((naive_b_cell->getTimeAlive() > 1000) || die_by_tnf ) {
      kill(naive_b_cell);
  }


}
