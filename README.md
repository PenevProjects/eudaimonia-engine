# TO RUN:

Download a release version and run the .exe.  
*Needs to be running on a Dedicated GPU for the PBR shaders to work!  
If you are on a laptop and are only getting hard colors without reflections, please use a GPU to run the application.*

# Overview #

This repo contains 2 frameworks:

**zero** – a data-oriented Entity-Component-System header-only framework in C++17
**eudaimonia** – 3D game engine prototype built with **zero**

 

# zero #

"zero" is a header-only Entity-Component-System framework created in C++17.

# eudaimonia #

"eudaimonia" is a 3D PBR game engine prototype created in C++17 on top of the "zero" ECS framework.

## demo ##

/Release folder contains "eudaimonia.exe"
Keybind "F" toggles mouse lock.

## to rebuild ##
1. Install `cmake > 3.10`
2. Open command terminal in root directory
```
    mkdir build
    cd build
    cmake ..
``` 
3. Set "eudaimonia" as start-up project.
4. Make sure you compile in "Debug" mode and target architecture is "Win32".
