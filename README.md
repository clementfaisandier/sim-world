# sim-world
Simulation projects!

Greensim is a climate change simulation meant to give people a global perspective of climate change.

## Development

Using CMake (Make) + Conan 2.X in our build system. In the future, I would like to add a nix configuration file to ensure reproducability. This system ensures our dependencies are tracked by Conan and system details and underlying dependencies are tracked by Nix.

### System Dependencies

While waiting for Nix integration, the following will have to be done manually by each dev.

This section is extremely incomplete. Just struggle and figure it out lol. Conan actually tends to tell you what is missing so there are few wild goose chases.

#### OpenGL Dispatch Library

There are a number of implementations by MESA, NVIDIA, AMD...

I installed the MESA one since that's what conan when running `conan install`.

``` bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y libgl1-mesa-dev
```

### Conan

Why Conan? I think C/C++ needs a package manager. Conan is open source and built in Python. As a bonus Conan also has pre-built packages for all our dependencies so we won't have to build them ourselves.

Conan is a Python tool. I recommend you install [uv](https://docs.astral.sh/uv/), it is my preferred way of managing python environments. The tool is still in pre-release but works well.

``` bash
uv tool install conan
conan --version
#> Conan version 2.27.0

# For conan first time setup:
conan profile detect
```

Notice this project has a `pyproject.toml`. This is for our python interpreter to have access to the conan library and enable syntax highlighting when developing `conanfile.py`.

### OLD

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
