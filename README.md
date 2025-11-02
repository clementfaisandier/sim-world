# sim-world
Simulation projects!

Greensim is a climate change simulation meant to give people a global perspective of climate change.

## Development

Install necessary dependencies:
``` bash
sudo apt install cmake c++ gdb libglfw3-dev libglew-dev pkg-config
```

Installing [glm](https://github.com/g-truc/glm#):
``` bash
# Navigate to the directory you want to clone repo to
git clone git@github.com:g-truc/glm.git
cd glm/
cmake \
    -DGLM_BUILD_TESTS=OFF \
    -DBUILD_SHARED_LIBS=OFF \
    -B build .
cmake --build build -- all
sudo cmake --build build -- install
```