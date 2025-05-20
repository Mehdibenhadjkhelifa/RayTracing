# Raytracer

A software rasterizated real-time RayTracer/PathTracer developed to further hone my
optimization and rendering knowledge.

![image](https://github.com/user-attachments/assets/61302002-e554-46d6-a325-bbb0711280ff)

**PROJECT_STATUS** : *WIP*. Still lacks some features for future development.
## Dependencies
#### 1- [Walnut](https://github.com/StudioCherno/Walnut) by StudioCherno
#### 2- [Imgui](https://github.com/ocornut/imgui) by ocornut
#### 3- [GLFW](https://github.com/glfw/glfw) 
#### 4- [glm](https://github.com/g-truc/glm) by g-truc
## Features
#### - Lighting and Shading
#### - Interactive 3D camera system
#### - Multiple Object Rendering
#### - Materials and Physically based Rendering
#### - Emission and Emissive materials
#### - Multithreaded real-time Rendering

## Setup

#### 1. Download the repository

-navigate to where you want to clone the repository using the command line

-clone the repository by enter the following command in your terminal : `git clone --recursive https://github.com/Mehdibenhadjkhelifa/RayTracing.git`

#### 2. Building the project 

**FOR LINUX**:

-simply use the shell scripts which can : 
 
 1- setup and build the project by typing in the command line : `scipts/setup.sh "config"`
 
 where "config" is either Debug or Release depending on your desired configuration

 this will generate build files and generate binaries for your choosen configuration

 2- Build the binaries by using this command : `scripts/build.sh` 
 
 which uses ninja build files and clang compiler under the hood

 3- run the binary (Need to build Release) with the command :`scripts/run.sh`

 **FOR WINDOWS**:

 1-make a build directory and navigate to it : `mkdir build && cd build`

 ##### NOTE : if you want to compile using visual studio you can then do `cmake ..` and ignore next steps
  
 2-execute cmake in the build directory:`cmake -DCMAKE_BUILD_TYPE=config ..`

 replace config with either Debug or Release depending on your desired configuration

 3-build the binaries : `cmake --build .`
 
 you should now have a bin folder inside build which contains your binaries
                                      
