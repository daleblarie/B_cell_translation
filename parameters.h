// parameters.h
#ifndef PARAMETERS_H
#define PARAMETERS_H

const bool RENDERING = true;
const bool SAVING_DATA = false;
const bool TURTLE_CONTINOUS_MOVEMENT = false;

const int RNG_SEED=108;
const int NUM_STEPS = 2000;


const static int WORLD_WIDTH = 101;
const static int WORLD_HEIGHT = 101;
const int PATCH_MAX_CAPACITY = 10;
const int MAX_RANDOM_DISTANCE = 100;

// this netlgoo implementation specific variables
const bool RandomRuns = false;
const bool AUTOINOCULATE = false;
const int AUTIINOCULATE_FIRST_TIME = 480;
const int AUTIINOCULATE_SECOND_TIME = 2880;
const int STOP_EXPOSURE_TIME = 4800;

const int BCELL_TNFA_APOP_THRESHOLD = 0;
const int BCELL_BREG_DIFF_THRESHOLD = 0;
const int BCELL_CD21_ACTIVATION_THRESHOLD =101;;

const int PHAG_IL6_BURST=0;


#endif
