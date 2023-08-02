#include "world.h"

// functions here are ones that are used in the simulation, but are not go or setup. Functions defined in world.cpp are more general or dont have to do much with the specific simulation, besudes setup and go

void World::spawn_b_cell() {
  if(step % 10 == 0) {
      // Position of the new Naive B-cell
      int cell_x = std::floor(WORLD_WIDTH / 2); //I added std::floor()
      int cell_y = 0;
      // Increase global ID counter
      global_ID_counter++;

      // Create a new Naive B-cell
      auto b_cell = std::make_shared<NaiveBCell>(cell_x, cell_y, global_ID_counter);
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
  }
}


void World::spawn_th0_cell() {
    if(step % 20 == 0) {
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
