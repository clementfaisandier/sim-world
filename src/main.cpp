#include "main.h"

int main(int argc, char* argv[])
{
    int sizeX = 10, sizeY = 10, sizeZ = 10;

    // Check if the user provided 3 dimensions
    if (argc == 4) {
        sizeX = std::stoi(argv[1]);
        sizeY = std::stoi(argv[2]);
        sizeZ = std::stoi(argv[3]);
    }

    std::cout << "Launching sim-world with grid: " 
              << sizeX << "x" << sizeY << "x" << sizeZ << std::endl;
    
    return grid_simulation(glm::vec3(sizeX, sizeY, sizeZ));
    // return spherical_simulation();
}
