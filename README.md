# sim-world
Simulation projects!

Greensim is a climate change simulation meant to give people a global perspective of climate change.

## Development

Running `conan build` should create the `sim-word` executable under `./build/Release/`.

### Build System

Using CMake (Make) + Conan 2.X in our build system. In the future, I would like to add a nix configuration file to ensure reproducability. This system ensures our dependencies are tracked by Conan and system details and underlying dependencies are tracked by Nix.

Run `uv sync` to get conan installed, this will ensure your intelisense has access to the right python packages while working on the `conanfile.py` recipe.

#### System Dependencies

While waiting for Nix integration, the following will have to be done manually by each dev.

This section is extremely incomplete. Just struggle and figure it out lol. Conan actually tends to tell you what is missing so there are few wild goose chases.

##### OpenGL Dispatch Library

There are a number of implementations by MESA, NVIDIA, AMD...

I installed the MESA one since that's what conan when running `conan install`.

``` bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y libgl1-mesa-dev
```

#### Conan

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
