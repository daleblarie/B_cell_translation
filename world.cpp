// world.cpp
#include "world.h"
#include "helper_functions.h"


void World::add_patch(int x, int y){
  all_patches[x][y] = Patch(x,y);
}

Patch& World::get_patch(int x, int y){
  x = (x+WORLD_WIDTH)%WORLD_WIDTH;
  y = (y+WORLD_HEIGHT)%WORLD_HEIGHT;
  return all_patches[x][y];
}

Patch& World::get_patch_ahead(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front of the turtle, based on its current heading
  int heading = turtle->getHeading();
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

Patch& World::get_patch_ahead_right(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front and to the right of the turtle, based on its current heading
  int heading = turtle->getHeading() - 45; // to look right 45 degrees
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

Patch& World::get_patch_ahead_left(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front and to the right of the turtle, based on its current heading
  int heading = turtle->getHeading() + 45; // to look left 45 degrees
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

void World::setup_patches(){
  for(int x=0; x<WORLD_WIDTH; x++){
    for(int y=0; y<WORLD_HEIGHT; y++){
      add_patch(x,y);
      // set initial values on patches here; eg patch.setOxy(100) to set patch oxy to initial value
    }
  }
}

void World::display_patches(){
  for(auto &patchArr: all_patches){
    for(auto &patch: patchArr){
      patch.display();
    }
  }
}

void World::add_turtle(int x, int y, int id, int heading){
  std::shared_ptr<Turtle> new_turtle = std::make_shared<Turtle>(x, y, id, heading);
  get_patch(x,y).add_turtle(new_turtle);
  all_turtles.emplace_back(new_turtle);
}

void World::move_turtle(std::shared_ptr<Turtle> turtle){
  // moves the turtle but asking the turtle to calculate its movement, and if there is space on the target_patch, the world executes the move
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  std::pair<int,int> new_coords = turtle->move(); //moving along turtle.heading for the default value of 1 unit
  Patch& target_patch = get_patch(new_coords.first, new_coords.second);
  if (turtle_current_patch == target_patch){
    return;
  } else if (target_patch.is_at_capacity()){
    turtle->execute_move(false);
  }
  else{
    turtle->execute_move(true);
    turtle_current_patch.remove_turtle(turtle);
    target_patch.add_turtle(turtle);
  }
}

void World::move_turtle_random_jump(std::shared_ptr<Turtle> turtle){
  // same as move turtle but instead of turtle-> move() we are using turtle->jumpRandom(RNG_Engine)
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  std::pair<int,int> new_coords = turtle->jumpRandom(RNG_Engine);
  Patch& target_patch = get_patch(new_coords.first, new_coords.second);
    if (turtle_current_patch == target_patch){
    return;
  } else if (target_patch.is_at_capacity()){
    turtle->execute_move(false);
  }
  else{
    turtle->execute_move(true);
    turtle_current_patch.remove_turtle(turtle);
    target_patch.add_turtle(turtle);
  }
}

void World::turtle_wiggle(std::shared_ptr<Turtle> turtle){
  // wiggles the turtles heading, then moves forward along the new heading for 1 unit
  turtle->wiggle(RNG_Engine);
  move_turtle(turtle);
}

void World::kill_turtle(std::shared_ptr<Turtle> turtle){
  // removed turtle from its patch
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  turtle_current_patch.remove_turtle(turtle);
}

//void kill(std::shared_ptr<T> &ptr) <-- templated function that will kill any turtle or subclass of turtle. implemented in header file

void World::updateTurtleVectors(){
    // erasing the turtle weak pointers that are now a null pointer because they have been reset
    all_turtles.erase(std::remove_if(all_turtles.begin(), all_turtles.end(),
    [](const std::weak_ptr<Turtle>& wp) {     //lambda function to check if weak pointer is expired (aka killed and reset() has been called)
        return wp.expired();
      }),
    all_turtles.end());

    // now need to erase the shared pointers that were reset with kill()
    // for example:  all_pmns.erase(std::remove(all_pmns.begin(), all_pmns.end(), nullptr), all_pmns.end());
    // implement erasure for all turtle shared_ptr vectors here
}

void World::setup(){
  // This function initializes the global variables and setus up the world
  global_ID_counter = 0;
  step = 0;
  time = 1;
  setup_patches(); // Initializing the patches

  // Clear all existing entities
  all_turtles.clear();
  all_bacterias.clear();
  all_antibodies.clear();
  all_fdcs.clear();
  all_naive_b_cells.clear();
  all_activated_b_cells.clear();
  all_gcb_cells.clear();
  all_sl_plasma_cells.clear();
  all_ll_plasma_cells.clear();
  all_mem_b_cells.clear();
  all_breg_cells.clear();
  all_tfh_cells.clear();
  all_th0_cells.clear();
  all_th1_cells.clear();
  all_th2_cells.clear();

  // Check if random runs is false, then set seed
  if (!RandomRuns) {
      set_rng_seed(RNG_SEED);
  }

  // Sets up the world structure
  // In NetLogo, the center of the world is 0,0
  // In C++, the bottom left of the world is 0,0
  // Therefore, we need to shift the patches
  int center_x = std::ceil(WORLD_WIDTH / 2);   //added the std::floor() because only in odd world sizes
  int center_y = std::ceil(WORLD_HEIGHT / 2);  //will it matter, but 101 is odd

  // Paracortex zone
  for (int x = center_x - 200; x <= center_x + 200; x++) {
      for (int y = center_y - 200; y <= center_y + 200; y++) {
          if (x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT) {
              Patch& p = get_patch(x, y);
              p.setPatchType(1);
              p.setColor("gray");
          }
      }
  }

  // Follicle zone
  for (int x = center_x - 49; x <= center_x + 49; x++) {
      for (int y = center_y - 49; y <= center_y + 49; y++) {
          if (x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT) {
            // Calculate the distance from the center
            int dist_x = center_x - x;
            int dist_y = center_y - y;
            if (dist_x * dist_x + dist_y * dist_y <= 49 * 49) {  // Radius of 49 units
                Patch& p = get_patch(x, y);
                p.setPatchType(0);
                p.setColor("black");
            }
          }
      }
  }

  // Exit from follicle
  for (int y = center_y - 5; y <= center_y + 5; y++) {
      if (y >= 0 && y < WORLD_HEIGHT) {
          Patch& p1 = get_patch(center_x - 50, y);
          Patch& p2 = get_patch(center_x - 49, y);
          p1.setPatchType(2);
          p1.setColor("red");
          p2.setPatchType(2);
          p2.setColor("red");
      }
  }

  //I added this part. In order to initialize 100 (or however many) FDCs that are apropriate distances from the center and also from eachother, we need to generate their positions intentionally, and not randomly. To do this we should generate all of them ahead of time, then create the cells.
  std::vector<std::pair<int, int>> FDCcoordinates = generateCoordinates(100, center_x, center_y, 30, 3);
  // creates 100 coordinates, centered on our origin (center_x/y) within 30 units of the center, and each at least 3 units apart

  // using the above command, I told gpt to recreate the pmn example from the general world.h example, but doing a different initialization command
  for (int i = 0; i < FDCcoordinates.size(); i++) {
      global_ID_counter++; // increment global ID counter to give to the new cell
      int x = FDCcoordinates[i].first;  // x-coordinate of the FDC
      int y = FDCcoordinates[i].second; // y-coordinate of the FDC
      auto fdc = std::make_shared<FDCs>(x, y, global_ID_counter); // creating the actual FDC turtle
      Patch& patch = get_patch(x, y); // get patch to add the turtle to
      patch.add_turtle(fdc); // add turtle to patch

      // Set shape and color for the FDC
      fdc->setShape("square");
      fdc->setColor("brown");

      std::weak_ptr<Turtle> fdc_weak_ptr = fdc;   // creating a weak pointer to give to all turtles vector so that deleting fdc object kills it in the turtle list

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(fdc_weak_ptr);
      all_fdcs.push_back(fdc);
  }

  int NUM_TFH_CELLS_TO_ADD = 50;
  for (int i = 0; i < NUM_TFH_CELLS_TO_ADD; i++) {
      global_ID_counter++; // increment global ID counter to give to the new cell
      // generate a new coordinate and verify if the patch at the coordinate is of type 1
      std::pair<int, int> coord;
      // do while loop. executes the codeblock once, then checks. while condition, it executes the block again then checks. Same thing as a normal while loop, but it will always do it at least once
      do {
          int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
          int random_y = RNG_Engine() % WORLD_HEIGHT;
          coord = std::make_pair(random_x, random_y);
      } while (get_patch(coord.first, coord.second).getPatchType() != 1);
      std::cout<<coord.first<<", "<<coord.second<< ", "<< get_patch(coord.first, coord.second).getPatchType()<<std::endl;

      auto tfhCell = std::make_shared<TfhCell>(coord.first, coord.second, global_ID_counter); // create the TfhCell
      Patch& patch = get_patch(coord.first, coord.second); // get patch to add the turtle to
      patch.add_turtle(tfhCell); // add turtle to patch

      // Set cell variables
      tfhCell->setTimeAlive(-1000);
      tfhCell->setShape("square");
      tfhCell->setColor("cyan");
      tfhCell->setCxcr5Level(11);
      tfhCell->setCcr7Level(6);
      tfhCell->setEbi2rLevel(5);
      tfhCell->setBcellBindingStatus(false);

      std::weak_ptr<Turtle> tfhCell_weak_ptr = tfhCell; // creating a weak pointer to give to all turtles vector

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(tfhCell_weak_ptr);
      all_tfh_cells.push_back(tfhCell);
  }

  int NUM_TH1_CELLS_TO_ADD = 10;
  for (int i = 0; i < NUM_TH1_CELLS_TO_ADD; i++) {
      global_ID_counter++; // increment global ID counter to give to the new cell
      std::pair<int, int> coord;
      do {
          int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
          int random_y = RNG_Engine() % WORLD_HEIGHT;
          coord = std::make_pair(random_x, random_y);
      } while (get_patch(coord.first, coord.second).getPatchType() != 1);

      auto th1Cell = std::make_shared<Th1Cell>(coord.first, coord.second, global_ID_counter); // create the Th1Cell
      Patch& patch = get_patch(coord.first, coord.second); // get patch to add the turtle to
      patch.add_turtle(th1Cell); // add turtle to patch

      // Set cell variables
      th1Cell->setTimeAlive(-1000);
      th1Cell->setShape("square");
      th1Cell->setColor("blue");
      th1Cell->setCxcr5Level(0);
      th1Cell->setCcr7Level(2);
      th1Cell->setEbi2rLevel(2);
      th1Cell->setBcellBindingStatus(false);

      std::weak_ptr<Turtle> th1Cell_weak_ptr = th1Cell; // creating a weak pointer to give to all turtles vector

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(th1Cell_weak_ptr);
      all_th1_cells.push_back(th1Cell);
  }

  int NUM_TH2_CELLS_TO_ADD = 50;
  for (int i = 0; i < NUM_TH2_CELLS_TO_ADD; i++) {
      global_ID_counter++; // increment global ID counter to give to the new cell
      std::pair<int, int> coord;
      do {
          int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
          int random_y = RNG_Engine() % WORLD_HEIGHT;
          coord = std::make_pair(random_x, random_y);
      } while (get_patch(coord.first, coord.second).getPatchType() != 1);

      auto th2Cell = std::make_shared<Th2Cell>(coord.first, coord.second, global_ID_counter); // create the Th2Cell
      Patch& patch = get_patch(coord.first, coord.second); // get patch to add the turtle to
      patch.add_turtle(th2Cell); // add turtle to patch

      // Set cell variables
      th2Cell->setTimeAlive(-1000);
      th2Cell->setShape("square");
      th2Cell->setColor("blue");
      th2Cell->setCxcr5Level(0);
      th2Cell->setCcr7Level(2);
      th2Cell->setEbi2rLevel(2);
      th2Cell->setBcellBindingStatus(false);

      std::weak_ptr<Turtle> th2Cell_weak_ptr = th2Cell; // creating a weak pointer to give to all turtles vector

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(th2Cell_weak_ptr);
      all_th2_cells.push_back(th2Cell);
  }



  // Initialize global variables and counters
  days_passed = 0;

  // Reset ticks or equivalent time step counter
  step = 0;
}


void World::go() {
  // calculate_incoming_tnfa_il6_level();
  // simulate_background_inflammation();

  // Update days_passed based on the step value.
  setDaysPassed(step / 48);

  spawnBCell();
  spawnTh0Cell();

  // Cytokine release from paracortex
  for (int x = 0; x < WORLD_WIDTH; x++) {
      for (int y = 0; y < WORLD_HEIGHT; y++) {
          Patch& p = get_patch(x, y);
          if (p.getPatchType() == 1) {
              p.setCcl19Level(p.getCcl19Level() + 2);
              p.setEbi2Level(p.getEbi2Level() + 2);
          }
      }
  }

  // Cytokine release from follicle exit
  for (int x = 0; x < WORLD_WIDTH; x++) {
      for (int y = 0; y < WORLD_HEIGHT; y++) {
          Patch& p = get_patch(x, y);
          if (p.getPatchType() == 2) {
              p.setS1pLevel(p.getS1pLevel() + 2);
          }
      }
  }

  // Ask all agents to perform their functions
  // for (auto& fdc : all_fdcs){fdcFunction(fdc);}
  // //
  // for (auto& naiveBCell : all_naive_b_cells){naiveBCell->naiveBCellFunction();}
  //
  // for (auto& activatedBCell : all_activated_b_cells){activatedBCell->activatedBCellFunction(activatedBCell);}
  //
  // for (auto& gcbCell : all_gcb_cells){gcbCell->gcbCellFunction(gcbCell);}
  //
  // for (auto& llPlasmaCell : all_ll_plasma_cells){llPlasmaCell->llPlasmaCellFunction(llPlasmaCell);}
  //
  // for (auto& slPlasmaCell : all_sl_plasma_cells){slPlasmaCell->slPlasmaCellFunction(slPlasmaCell);}
  //
  // for (auto& memBCell : all_mem_b_cells){memBCell->memBCellFunction(memBCell);}
  //
  // for (auto& antibody : all_antibodies){antibodiesFunction(antibody);}
  //
  // for (auto& bregCell : all_breg_cells){bregCell->bregFunction(bregCell);}
  //
  // for (auto& tfhCell : all_tfh_cells){tfhCell->tfhCellFunction(tfhCell);}
  //
  // for (auto& th0Cell : all_th0_cells){th0Cell->th0CellFunction(th0Cell);}
  //
  // for (auto& th1Cell : all_th1_cells){th1Cell->th1CellFunction(th1Cell);}
  //
  // for (auto& th2Cell : all_th2_cells){th2Cell->th2CellFunction(th2Cell);}
  //
  // for (auto& bacteria : all_bacterias){bacteria->bacteriaFunction(bacteria);}
  //
  // Check if autoinoculate is active
  if(AUTOINOCULATE) {
      if(step == AUTIINOCULATE_FIRST_TIME) {
          // auto_inoculate(first_exposure_amt);  // Call your inoculation function
      }
      else if(step == AUTIINOCULATE_SECOND_TIME) {
          // auto_inoculate(second_exposure_amt);  // Call your inoculation function
      }
      else if(step == STOP_EXPOSURE_TIME) {
          // stop();  // You might want to stop the simulation
      }
  }


  // update_chemokine_gradient();
  // check_overall_cd21_expression();

  // Check if autoinoculate is active
  // if(autoinoculate) {
      // Specific actions based on current step value
  // }

  // Increase the step (equivalent to NetLogo's tick)
  step++;



  // end of go
  updateTurtleVectors(); // need to update turtle positions/delete dead turtles
}


void World::diffuse(){
  // need to diffuse values from patches out to neighboring patches
  float factor_for_neighbors = 1/8; //eight neighbors in a 2D grid

  // // follow this example format for each diffusing variable
  // for (auto& patch : all_patches){patch->setTempVar(0);}  // reset temp_var
  // // start with Endotoxin
  // // get totals for neighbors
  // for (auto& center_patch : all_patches){
  //   int center_patch_x_pos = center_patch->getX();
  //   int center_patch_y_pos = center_patch->getY();
  //   for (int i=-1; i<2; i++){
  //     for (int j=-1; j<2; j++){
  //       if (i==0 && j==0) { // skipping center cell
  //         continue;
  //       }
  //       int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH); // getting modulo of position + x/y to wrap around toroidal world
  //       int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);
  //       auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);
  //       neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getEndotoxin()*factor_for_neighbors*ENDOTOXIN_DIFFUSION_FACTOR); // evenly adding the amount diffused out from the center patch to neighbor patches
  //     }
  //   }
  // }
  // // set Endotoxin on each patch from the temp var, then reset tempvar
  // for (auto& patch : all_patches){
  //   patch->setEndotoxin((patch->getEndotoxin() * (1-ENDOTOXIN_DIFFUSION_FACTOR)) + patch->getTempVar());  // new total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  //   patch->setTempVar(0); // resetting
  // }

  //implement diffusion for other variables below


}

void World::evaporate(){
  // evaporate variables off of all patches
  for (auto& patch : all_patches) {
    // Endotoxin as an example, amount to set is current amount times the evaporation factor
    // patch->setEndotoxin(patch->getEndotoxin() * ENDOTOXIN_EVAPORATION_FACTOR);

  }
}
