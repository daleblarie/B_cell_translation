# B_CELL_ABM

This is a C++ implementation of the netlogo b_cell_abm created by Bryan Shin and described in: Shin B, An G, Cockrell RC (2024) Examining B-cell dynamics and responsiveness in different inflammatory milieus using an agent-based model. PLoS Comput Biol 20(1): e1011776. https://doi.org/10.1371/journal.pcbi.1011776

This implementation was created both so this model can be run on HPC hardware, as netlogo cannot easily, and as input code for use with a large language model to build a code translation pipeline for other computational models written in Netlogo.


Here is a short video of the simulation output.


<img src="https://github.com/daleblarie/B_cell_translation/assets/33942693/9af0fb4f-86c3-4369-82f0-3e999f20cb3e" alt="" style="width:600px;height:600px;margin-left: auto;margin-right: auto;">


## Description of the model


- The system takes place in a two-dimensional "square" of blood. Therefore, each patch represents an xy location in the blood
- In the blood, there are 3 types of objects: B-cells, bacteria, and antibodies
	- B-cells are circles
	- Bacteria are oval things with antennae
	- Antibodies are light brown circles
- Each B-cell expresses a single BCR
- Each bacteria is simplified to express only a single epitope
- Each antibody can bind to a single bacterial epitope

 

## How the model works

- All B-cells, bacteria, and antibodies move in random directions in the system to simulate how they might "float" in the blood
- Bacteria behavior:
	- Each bacteria moves in a random location
- B-cell behavior:
	- B-cells move in random directions
	- If it touches a bacteria, it allows its BCR to interact with the bacteria
	- If the bacteria's epitope happens to fit into the BCR, then the bacteria is phagocytosed and killed
	- Then, the B-cell produces antibodies with the same specificity as the bound BCR
- Antibody behavior:
	- Antibodies move in a random direction
	- Similar to a B-cell, if an antibody touches a bacteria, it neutralizes and kills that bacteria


## Things to notice

When a bacteria with a given epitope-type is first introduced to the system, it takes a long time for the immune system to clear it. However, as antibodies are synthesized, the second inoculation of that same epitope-type bacteria will result in a shorter time to clear the infection. On the same thread, introducing a different epitope-type bacteria will result in a longer clear time, as no antibodies have been synthesized yet.



## Installation notes
This will render the simulation graphically so that you can see what is happening.
It can be paused by focusing on the simulation graphics window and pressing spacebar.
To change the render size, change the `GRID_SIZE` parameter in `rendering.h` to be smaller for a smaller rendering, or larger for a bigger one.


This implementation requires SDL2 to be installed. This is a graphics rendering library for C++.
It can be installed with the following commands. Instructions were taken from https://wiki.libsdl.org/SDL2/Installation
```
git clone https://github.com/libsdl-org/SDL.git -b SDL2
cd SDL
mkdir build
cd build
../configure
make
sudo make install
```


compile this using `source ./compile.sh` and run using `./test.o`
