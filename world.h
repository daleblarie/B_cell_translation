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
#include <random>


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
  void move_turtle(std::shared_ptr<Turtle> turtle, float distance=1);
  void move_turtle_random_jump(std::shared_ptr<Turtle> turtle);
  void turtle_wiggle(std::shared_ptr<Turtle> turtle);

  void kill_turtle(std::shared_ptr<Turtle> turtle);

  // templated function to kill any turtle or agent
  template <typename T>
  void kill(std::shared_ptr<T> &ptr){
    ptr->removeLinkedTurtle();
    kill_turtle(ptr);
    ptr.reset();
  }

  void updateTurtleVectors();

  void setup();
  void go();

  void update_chemokine_gradient();

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
  void gc_b_cell_function(std::shared_ptr<GCBCell> gc_b_cell);
  void ll_plasma_cell_function(std::shared_ptr<LLPlasmaCell> ll_plasma_cell);
  void sl_plasma_cell_function(std::shared_ptr<SLPlasmaCell> sl_plasma_cell);
  void memBCellFunction(std::shared_ptr<MemBCell> naive_b_cell);
  void th0CellFunction(std::shared_ptr<Th0Cell> th0_cell);
  void tfhCellFunction(std::shared_ptr<TfhCell> tfh_cell);
  void th1CellFunction(std::shared_ptr<Th1Cell> th1_cell);
  void th2CellFunction(std::shared_ptr<Th2Cell> th2_cell);
  void bacteriaFunction(std::shared_ptr<Bacteria> bacteria);

  std::shared_ptr<Antibodies> getOneAntibodyHere(int xPos, int yPos);
  std::shared_ptr<FDCs> getOneFDCHere(int patchX, int patchY);
  std::shared_ptr<Bacteria> getOneBacteriaHere(int patchX, int patchY);
  std::shared_ptr<Th2Cell> getOneTh2Here(int patchX, int patchY);
  std::shared_ptr<TfhCell> getOneTfhHere(int patchX, int patchY);
  std::shared_ptr<ActivatedBCell> getOneActivatedBCellHere(int patchX, int patchY);
  std::vector<std::shared_ptr<FDCs>> get_fdcs_with_no_presented_antigen(int patchX, int patchY);

  // void checkTNFStatus(std::shared_ptr<T> cell) lives at bottom because of template declaration restrictions
  void isotypeSwitch(std::shared_ptr<ActivatedBCell> activated_b_cell);
  // Getters and setters for global variables
  int getDaysPassed() const { return days_passed; }
  void setDaysPassed(int days) { days_passed = days; }

  double getAverageCD21Expression() const { return average_cd21_expression; }
  void setAverageCD21Expression(double expr) { average_cd21_expression = expr; }

  int getTotalNumOfApoptosedCells() const { return total_num_of_apoptosed_cells; }
  void setTotalNumOfApoptosedCells(int num) { total_num_of_apoptosed_cells = num; }
















  template <typename CellType>
  bool checkTNFStatus(CellType& cell){
// if returns true, that means this cell needs to die
    Patch& current_patch = get_patch(cell->getX(), cell->getY());
    current_patch.setTnfA(current_patch.getTnfA() - .01);
    cell->setTnfaStimulation( current_patch.getTnfA() * 100);
    
    return (cell->getTnfaStimulation() > cell->getTnfaThreshold());
  }

  template <typename CellType>
  bool checkBregStatus(CellType& cell){
    Patch& current_patch = get_patch(cell->getX(), cell->getY());
    if (cell->getProBreg() > cell->getBregThreshold()){
      return true;
    } else{
      cell->setProBreg(((current_patch.getIl6() + current_patch.getIl21()) * 45));
      return false;
    }
  }

  template <typename CellType>
  std::shared_ptr<BregCell> turnIntoBreg(CellType& cell){
    std::shared_ptr<BregCell> breg = std::make_shared<BregCell>(cell->getX(), cell->getY(), cell->getHeading(), global_ID_counter++);
    
    breg->setSize(1);
    breg->setShape("circle");
    breg->setColor("violet");
    breg->setS1pr1Level(0);
    breg->setCxcr5Level(10);
    breg->setTimeAlive(0);
    
    std::weak_ptr<Turtle> breg_weak_ptr = breg;
    all_turtles.push_back(breg_weak_ptr);
    all_breg_cells.push_back(breg);
    kill(cell);
    return breg;
  }
  
  template <typename CellType>
  void chemotaxis(CellType& cell){
    std::cout<<"chemotaxing"<<std::endl;
    std::cout<<"my location" <<cell->getX()<<", "<< cell->getY()<<std::endl;
    Patch& current_patch = get_patch(cell->getX(), cell->getY());
    double rt_turn = 0.0;
    double lt_turn = 0.0;
    double s1pr1_weight = current_patch.getS1pLevel() / 100.0;
    std::vector<Patch> neighbors;
    for (int x = -1; x < 2; x++) {
      for (int y = -1; y < 2; y++) {
        if (x==0 && y==0){continue;}
        std::cout<<"neighbor location"<<cell->getX() + x<<", "<< cell->getY()+ y <<std::endl;

        neighbors.push_back(get_patch(current_patch.getX() + x, current_patch.getY() + y));
      }
    }
    std::cout<<"chemotaxing2"<<std::endl;
    
    double max_s1p = -1;
    Patch max_patch;
    for (auto neighbor_patch: neighbors){
      std::cout<<neighbor_patch.getX()<<std::endl;
      if (neighbor_patch.getS1pLevel() > max_s1p) {
        std::cout<<"Changing max patch"<<std::endl;
        max_s1p = neighbor_patch.getS1pLevel();
        max_patch = get_patch(neighbor_patch.getX(), neighbor_patch.getY());
        /* code */
      }
    }
   std::cout<<"chemotaxing3"<<std::endl;
// Patch max_s1p_patch = std::max_element(neighbors.begin(), neighbors.end(), 
    //                                          [](const Patch* a, const Patch* b) {
    //                                              return a.getS1pLevel() < b.getS1pLevel();
    //                                          });
    std::cout<<max_patch.getX()<<","<< max_patch.getY()<<std::endl;
    double angle_to_s1p = cell->angle_to(get_patch(max_patch.getX(), max_patch.getY()));
    std::cout<<"chemotaxing4"<<std::endl;

    double cur_angle = cell->getHeading();
    double x = angle_to_s1p - cur_angle;
    if (x < 0.0) {
        x += 360.0;
    }
    double y = 360.0 - x;
    if (x < y) {
        rt_turn += x * s1pr1_weight;
    } else {
        lt_turn += y * s1pr1_weight;
    }
  }
  
  
  
};


#endif
