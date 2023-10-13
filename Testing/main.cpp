#include <iostream>
#include <fstream>
#include <vector>
#include "particle.hpp"

std::vector<Particle> readParticlesFromFile(const std::string& filename, double& commonDensity, double& commonAcceleration);

int main() {
    const std::string filename = "particle_data.txt";

    double commonDensity, commonAcceleration;
    std::vector<Particle> particles = readParticlesFromFile(filename, commonDensity, commonAcceleration);

    std::cout << "Common Density: " << commonDensity << std::endl;
    std::cout << "Common Acceleration: " << commonAcceleration << std::endl;

    for (size_t i = 0; i < particles.size(); ++i) {
        Vector pos = particles[i].getPosition();
        Vector hv = particles[i].getHeadVector();
        Vector vel = particles[i].getVelocityVector();

        std::cout << "Particle " << i + 1 << " position: (" 
                  << pos.x << ", " << pos.y << ", " << pos.z << "), "
                  << "hv: (" << hv.x << ", " << hv.y << ", " << hv.z << "), "
                  << "v: (" << vel.x << ", " << vel.y << ", " << vel.z << ")"
                  << std::endl;

        // Call the new function
        particles[i].reposition();
    }

    return 0;
}

std::vector<Particle> readParticlesFromFile(const std::string& filename, double& commonDensity, double& commonAcceleration) {
    std::vector<Particle> particles;
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return particles;
    }

    inputFile >> commonDensity >> commonAcceleration;

    double px, py, pz, hvx, hvy, hvz, vx, vy, vz;
    while (inputFile >> px >> py >> pz >> hvx >> hvy >> hvz >> vx >> vy >> vz) {
        Particle particle(
            commonDensity, commonAcceleration,
            Vector{px, py, pz},
            Vector{hvx, hvy, hvz},
            Vector{vx, vy, vz}
        );
        particles.push_back(particle);
    }

    inputFile.close();
    return particles;
}
