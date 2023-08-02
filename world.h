// world.h
#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <memory>
#include <algorithm>

#include "parameters.h"
#include "agent.h"
#include "patch.h"
#include "turtle.h"
#include "bacteria.h"
#include "antibodies.h"
#include "fdcs.h"
#include "naive_b_cell.h"
#include "activated_b_cell.h"
#include "gc_b_cell.h"
#include "sl_plasma_cell.h"
#include "ll_plasma_cell.h"
#include "mem_b_cell.h"
#include "breg_cell.h"
#include "tfh_cell.h"
#include "th0_cell.h"
#include "th1_cell.h"
#include "th2_cell.h"

class World {
private:
  std::mt19937 RNG_Engine;
  int global_ID_counter;  //ID counter for creating new turtles to make sure each has a unique ID
  int step;
  int time;
  int loop_run;

  // NetLogo global variables
  int days_passed;
  double average_cd21_expression;
  int total_num_of_apoptosed_cells;

  Patch all_patches[WORLD_WIDTH][WORLD_HEIGHT];

  // Vectors of shared pointers to different turtle subclasses
  std::vector<std::shared_ptr<Bacteria>> all_bacterias;
  std::vector<std::shared_ptr<Antibodies>> all_antibodies;
  std::vector<std::shared_ptr<FDCs>> all_fdcs;
  std::vector<std::shared_ptr<NaiveBCell>> all_naive_b_cells;
  std::vector<std::shared_ptr<ActivatedBCell>> all_activated_b_cells;
  std::vector<std::shared_ptr<GCBCell>> all_gcb_cells;
  std::vector<std::shared_ptr<SLPlasmaCell>> all_sl_plasma_cells;
  std::vector<std::shared_ptr<LLPlasmaCell>> all_ll_plasma_cells;
  std::vector<std::shared_ptr<MemBCell>> all_mem_b_cells;
  std::vector<std::shared_ptr<BregCell>> all_breg_cells;
  std::vector<std::shared_ptr<TfhCell>> all_tfh_cells;
  std::vector<std::shared_ptr<Th0Cell>> all_th0_cells;
  std::vector<std::shared_ptr<Th1Cell>> all_th1_cells;
  std::vector<std::shared_ptr<Th2Cell>> all_th2_cells;

  void set_rng_seed(int newSeed){RNG_Engine.seed(newSeed);}
  void add_patch(int x, int y);

public:
  std::vector<std::weak_ptr<Turtle>> all_turtles;

  World(){};

  Patch& get_patch(int x, int y);
  Patch& get_patch_ahead(std::shared_ptr<Turtle> turtle, float distance=1);
  Patch& get_patch_ahead_right(std::shared_ptr<Turtle> turtle, float distance=1);
  Patch& get_patch_ahead_left(std::shared_ptr<Turtle> turtle, float distance=1);

  void setup_patches();
  void display_patches();

  void add_turtle(int x, int y, int id, int heading=0);
  void move_turtle(std::shared_ptr<Turtle> turtle);
  void move_turtle_random_jump(std::shared_ptr<Turtle> turtle);
  void turtle_wiggle(std::shared_ptr<Turtle> turtle);

  void kill_turtle(std::shared_ptr<Turtle> turtle);

  // templated function to kill any turtle or agent
  template <typename T>
  void kill(std::shared_ptr<T> &ptr){
    kill_turtle(ptr);
    ptr.reset();
  }

  void updateTurtleVectors();

  void setup();
  void go();

  void diffuse();
  void evaporate();

  void spawnTh0Cell();
  void spawnBCell();
  void fdcFunction(std::shared_ptr<FDCs> fdc);
  void antibodiesFunction(std::shared_ptr<Antibodies> antibody);
  void naiveBCellFunction(std::shared_ptr<NaiveBCell> naive_b_cell);
  void bregFunction(std::shared_ptr<BregCell> breg_cell);
  void activatedBCellFunction(std::shared_ptr<ActivatedBCell> activated_b_cell);
  void tdResponse(std::shared_ptr<ActivatedBCell> activated_b_cell);
  void tiResponse(std::shared_ptr<ActivatedBCell> activated_b_cell);



  std::shared_ptr<Antibodies> getOneAntibodyHere(int xPos, int yPos);
  std::shared_ptr<FDCs> getOneFDCHere(int patchX, int patchY);
  std::shared_ptr<Bacteria> getOneBacteriaHere(int patchX, int patchY);
  std::shared_ptr<Th2Cell> getOneTh2Here(int patchX, int patchY);
  std::shared_ptr<TfhCell> getOneTfhHere(int patchX, int patchY);
  std::shared_ptr<ActivatedBCell> getOneActivatedBCellHere(int patchX, int patchY);


  // void checkTNFStatus(std::shared_ptr<T> cell) lives at bottom because of template declaration restrictions
  void isotypeSwitch(std::shared_ptr<ActivatedBCell> activated_b_cell);
  // Getters and setters for global variables
  int getDaysPassed() const { return days_passed; }
  void setDaysPassed(int days) { days_passed = days; }

  double getAverageCD21Expression() const { return average_cd21_expression; }
  void setAverageCD21Expression(double expr) { average_cd21_expression = expr; }

  int getTotalNumOfApoptosedCells() const { return total_num_of_apoptosed_cells; }
  void setTotalNumOfApoptosedCells(int num) { total_num_of_apoptosed_cells = num; }
















  template <typename T>
  void checkTNFStatus(std::shared_ptr<T> cell) {
      // Get the patch where the cell is
      Patch& cell_patch = get_patch(cell->getX(), cell->getY());

      // Represents consumption of TNFa
      cell_patch.setTnfA(cell_patch.getTnfA() - 0.01);

      // Calculate tnf-a stimulation
      double tnf_a_stimulation = 100 * cell_patch.getTnfA();

      // Check if the tnf-a stimulation is above the threshold
      if(tnf_a_stimulation > cell->getTnfaThreshold()) {
          // Increase total number of apoptosed cells
          ++total_num_of_apoptosed_cells;

          // Kill the cell
          kill(cell);
      }
  }

};


#endif
