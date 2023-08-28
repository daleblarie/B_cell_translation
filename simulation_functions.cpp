#include "world.h"

// functions here are ones that are used in the simulation, but are not go or setup. Functions defined in world.cpp are more general or dont have to do much with the specific simulation, besudes setup and go

void World::spawnBCell() {
  if(step % 10 == 0) {
    std::cout<<"SPAWNING B CELL"<<std::endl;
      // Position of the new Naive B-cell
      int cell_x = WORLD_WIDTH -2; //I added std::floor()
      int cell_y = std::floor(WORLD_HEIGHT / 2);

      // Create a new Naive B-cell
      auto b_cell = std::make_shared<NaiveBCell>(cell_x, cell_y, global_ID_counter++, 180);
      std::weak_ptr<Turtle> b_cell_weak_ptr = b_cell;
      // Set the properties of the new Naive B-cell
      b_cell->setShape("circle");
      b_cell->setColor("white");
      b_cell->setSize(1);
      b_cell->setTimeAlive(0);
      b_cell->setBcr(RNG_Engine() % 30);
      b_cell->setIsotype("md");
      b_cell->setS1pr1Level(0);
      b_cell->setS1pr2Level(0);
      b_cell->setCxcr5Level(16);
      b_cell->setCcr7Level(0);
      b_cell->setEbi2rLevel(0);
      b_cell->setCd21Level(0);
      b_cell->setInBlood(false);

      // Set thresholds
      b_cell->setTnfaThreshold(BCELL_TNFA_APOP_THRESHOLD + (RNG_Engine() % 50));
      b_cell->setBregThreshold(BCELL_BREG_DIFF_THRESHOLD + (RNG_Engine() % 50));

      // Add the new Naive B-cell to the list
      all_naive_b_cells.push_back(b_cell);
      all_turtles.push_back(b_cell_weak_ptr);
      get_patch(b_cell->getX(), b_cell->getY()).add_turtle(b_cell);

  }
}


void World::spawnTh0Cell() {
    if(step % 20 == 0) {
      std::cout<<"SPAWNING TH0 CELL"<<std::endl;

        // Position of the new Th0-cell
        int cell_x, cell_y;

        // Keep generating random coordinates until we find a patch of type 1
        do {
            cell_x = RNG_Engine() % WORLD_WIDTH;
            cell_y = RNG_Engine() % WORLD_HEIGHT;
        } while(get_patch(cell_x, cell_y).getPatchType() != 1);

        // Increase global ID counter
        global_ID_counter++;
        // Create a new Th0-cell
        auto th0_cell = std::make_shared<Th0Cell>(cell_x, cell_y, global_ID_counter);
        std::weak_ptr<Turtle> th0_cell_weak_ptr = th0_cell;

        // Set the properties of the new Th0-cell
        th0_cell->setShape("square");
        th0_cell->setColor("yellow");
        th0_cell->setTimeAlive(0);
        th0_cell->setS1pr1Level(0);
        th0_cell->setS1pr2Level(0);
        th0_cell->setCxcr5Level(0);
        th0_cell->setCcr7Level(6);
        th0_cell->setEbi2rLevel(6);
        th0_cell->setInBlood(false);

        // Add the new Th0-cell to the list of all Th0-cells
        all_th0_cells.push_back(th0_cell);

        // Also add it to the list of all Turtles
        all_turtles.push_back(th0_cell_weak_ptr);

        get_patch(th0_cell->getX(), th0_cell->getY()).add_turtle(th0_cell);

    }
}

// Function to randomly return an antibody on a specific patch

std::shared_ptr<Antibodies> World::getOneAntibodyHere(int patchX, int patchY) {
    // Create a vector to store antibodies on the specified patch
    std::vector<std::shared_ptr<Antibodies>> antibodiesOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    // Find antibodies on the patch
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<Antibodies> antibody = std::dynamic_pointer_cast<Antibodies>(turtle)) {
        // The turtle is a NaiveBCell, add it to the corresponding vector
        antibodiesOnPatch.push_back(antibody);
      }
    }
    // Check if any antibodies are found on the patch
    if (antibodiesOnPatch.empty()) {
        return nullptr; // Return nullptr if no antibodies found on the patch
    }
    // Generate a random index to pick a random antibody from the antibodiesOnPatch vector
    int randomIndex = RNG_Engine() % antibodiesOnPatch.size();

    // Return the randomly selected antibody
    return antibodiesOnPatch[randomIndex];
}

// Function to randomly return an antibody on a specific patch
std::shared_ptr<FDCs> World::getOneFDCHere(int patchX, int patchY){
    // Create a vector to store antibodies on the specified patch
    std::vector<std::shared_ptr<FDCs>> FDCsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    // Find antibodies on the patch
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<FDCs> fdc = std::dynamic_pointer_cast<FDCs>(turtle)) {
        // The turtle is a NaiveBCell, add it to the corresponding vector
        FDCsOnPatch.push_back(fdc);
      }
    }
    // Check if any antibodies are found on the patch
    if (FDCsOnPatch.empty()) {
        return nullptr; // Return nullptr if no antibodies found on the patch
    }
    // Generate a random index to pick a random antibody from the antibodiesOnPatch vector
    int randomIndex = RNG_Engine() % FDCsOnPatch.size();

    // Return the randomly selected antibody
    return FDCsOnPatch[randomIndex];
}

std::shared_ptr<Bacteria> World::getOneBacteriaHere(int patchX, int patchY){
    // Create a vector to store antibodies on the specified patch
    std::vector<std::shared_ptr<Bacteria>> bacteriaOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    // Find antibodies on the patch
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<Bacteria> bacteria = std::dynamic_pointer_cast<Bacteria>(turtle)) {
        // The turtle is a NaiveBCell, add it to the corresponding vector
        bacteriaOnPatch.push_back(bacteria);
      }
    }
    // Check if any antibodies are found on the patch
    if (bacteriaOnPatch.empty()) {
        return nullptr; // Return nullptr if no antibodies found on the patch
    }
    // Generate a random index to pick a random Bacteria from the antibodiesOnPatch vector
    int randomIndex = RNG_Engine() % bacteriaOnPatch.size();

    // Return the randomly selected Bacteria
    return bacteriaOnPatch[randomIndex];
}
// Function to randomly return a Th2 cell on a specific patch
std::shared_ptr<Th2Cell> World::getOneTh2Here(int patchX, int patchY){
    std::vector<std::shared_ptr<Th2Cell>> Th2CellsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<Th2Cell> th2 = std::dynamic_pointer_cast<Th2Cell>(turtle)) {
        Th2CellsOnPatch.push_back(th2);
      }
    }
    if (Th2CellsOnPatch.empty()) {
        return nullptr;
    }
    int randomIndex = RNG_Engine() % Th2CellsOnPatch.size();
    return Th2CellsOnPatch[randomIndex];
}

// Function to randomly return a Tfh cell on a specific patch
std::shared_ptr<TfhCell> World::getOneTfhHere(int patchX, int patchY){
    std::vector<std::shared_ptr<TfhCell>> TfhCellsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<TfhCell> tfh = std::dynamic_pointer_cast<TfhCell>(turtle)) {
        TfhCellsOnPatch.push_back(tfh);
      }
    }
    if (TfhCellsOnPatch.empty()) {
        return nullptr;
    }
    int randomIndex = RNG_Engine() % TfhCellsOnPatch.size();
    return TfhCellsOnPatch[randomIndex];
}

// Function to randomly return an Activated B cell on a specific patch
std::shared_ptr<ActivatedBCell> World::getOneActivatedBCellHere(int patchX, int patchY){
    std::vector<std::shared_ptr<ActivatedBCell>> ActivatedBCellsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<ActivatedBCell> activated_b_cell = std::dynamic_pointer_cast<ActivatedBCell>(turtle)) {
        ActivatedBCellsOnPatch.push_back(activated_b_cell);
      }
    }
    if (ActivatedBCellsOnPatch.empty()) {
        return nullptr;
    }
    int randomIndex = RNG_Engine() % ActivatedBCellsOnPatch.size();
    return ActivatedBCellsOnPatch[randomIndex];
}


std::vector<std::shared_ptr<FDCs>> World::get_fdcs_with_no_presented_antigen(int patchX, int patchY){
  std::vector<std::shared_ptr<FDCs>> FDCs_with_no_anitigen;
  Patch& current_patch = get_patch(patchX, patchY);
  std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
  for (const auto& turtle : turtles_on_patch) {
    if (std::shared_ptr<FDCs> fdc = std::dynamic_pointer_cast<FDCs>(turtle)) {
      if (fdc->getPresentedAntigen() ==0) {
        FDCs_with_no_anitigen.push_back(fdc);
      }
    }
  }
  return FDCs_with_no_anitigen;
}
