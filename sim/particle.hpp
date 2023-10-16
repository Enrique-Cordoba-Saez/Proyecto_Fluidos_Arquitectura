#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include <iostream>
#include <vector>

class Particle {
  public:
    Particle(
        std::vector<double> acceleration = std::vector<double>(3, 0.0),
        std::vector<double> initialPosition = std::vector<double>(3, 0.0),
        double density = 0.0,
        std::vector<double> initialHeadVector = std::vector<double>(3, 0.0),
        std::vector<double> initialVelocityVector = std::vector<double>(3, 0.0)
        //std::vector<int> initialBlockIndexes = std::vector<int>(3, 0)
    );

    // Getter methods
    double getDensity() const;
    std::vector<double> getAcceleration() const;
    std::vector<double> getPosition() const;
    std::vector<double> getHeadVector() const;
    std::vector<double> getVelocityVector() const;
    std::vector<int> getBlockIndexes() const;

    void reposition();
    void incremento_densidad(); // estoy mezclando inglés y español
    void transformacion_densidad();
    void transferencia_aceleracion();

  private:
    double density;
    std::vector<double> acceleration;
    std::vector<double> position;
    std::vector<double> headVector;
    std::vector<double> velocityVector;
    std::vector<int> blockIndexes;
};

/*void reposicionarParticulas(std::vector<Particle> particles, std::vector<int> numBloques,
                            std::vector<double> tamanoBloque);*/
#endif // PARTICLE_HPP

