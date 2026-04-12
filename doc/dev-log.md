# Dev Log

## 4/12/2026

First dev log entry! Although this project started almost two years ago, I have a few months worth of work into it. So far, the project has focused on OpenGl: understanding the basic rendering pipeline, meshes, basic linear algebra etc. I have also worked on the build system to understand CMake and Conan, which I believe are the best build related tools in C++ development. Nix integration being the last step in that process to encapsulate the system package dependencies not managed by CMake or Conan. 

At this point in the project, I can construct and render basic rectangular and spherical meshes. I have yet to do any kind of simulation in those meshes. I describe potential next steps in [this commit](https://github.com/clementfaisandier/sim-world/issues/3). My current focus is to implement some basic simulations. The big challenge there being my limited knowledge in ODEs and experience with Physical simulations.

For the sake of my sanity, I will start building simulations with my very limited knowledge. These will often not be right. But at least it will be fun to derive from my understanding of physics and math. Then I will be able to compare with the textbooks, and learn the proper math and physics through experimentation and failure.

To start, I will model 2D float using my grid mesh builder. This mesh builder could be improved render-wise using instanced and deffered rendering, but that will be a lesson for another day, for now the un-optimized method will do.


