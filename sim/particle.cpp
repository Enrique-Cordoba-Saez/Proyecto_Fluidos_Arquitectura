#include "particle.hpp"
#include <iostream>

Particle::Particle(
    double initialDensity, double initialAcceleration,
    std::vector<double> initialPosition,
    std::vector<double> initialHeadVector,
    std::vector<double> initialVelocityVector,
    std::vector<int> initialBlockIndexes
    ) : density(initialDensity),
    acceleration(initialAcceleration),
    position(initialPosition),
    headVector(initialHeadVector),
    velocityVector(initialVelocityVector),
    blockIndexes(initialBlockIndexes) {
  // Constructor implementation
}

double Particle::getDensity() const {
  return density;
}

double Particle::getAcceleration() const {
  return acceleration;
}

std::vector<double> Particle::getPosition() const {
  return position;
}

std::vector<double> Particle::getHeadVector() const {
  return headVector;
}

std::vector<double> Particle::getVelocityVector() const {
  return velocityVector;
}

std::vector<int> Particle::getBlockIndexes() const {
  return blockIndexes;
}

void Particle::reposition() {
  std::cout << "Repositioning particle..." << std::endl;

  incremento_densidad();
  transformacion_densidad();
  transferencia_aceleracion();
}

void Particle::incremento_densidad() {
  std::cout << "Incremento de densidad..." << std::endl;
}

void Particle::transformacion_densidad() {
  std::cout << "Transformacion de densidad..." << std::endl;
}

void Particle::transferencia_aceleracion() {
  std::cout << "Transferencia de aceleracion..." << std::endl;
}

/*void reposicionarParticulas(std::vector<Particle> particles, std::vector<double> numBloques,
                            std::vector<double> tamanoBloque) {
    for (auto current_particle : particles) {

    }
}*/