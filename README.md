# B_CELL_ABM

This is a C++ implementation of the netlogo b_cell_abm created by Bryan Shin and described in: Shin B, An G, Cockrell RC (2024) Examining B-cell dynamics and responsiveness in different inflammatory milieus using an agent-based model. PLoS Comput Biol 20(1): e1011776. https://doi.org/10.1371/journal.pcbi.1011776

This implementation was created both so this model can be run on HPC hardware, as netlogo cannot easily, and as input code for use with a large language model to build a code translation pipeline for other computational models written in Netlogo.


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


Here is a video of the simulation output.


![bcell (1)](https://github.com/daleblarie/B_cell_translation/assets/33942693/9af0fb4f-86c3-4369-82f0-3e999f20cb3e =250x250)

