#include "particle.hpp"
#include <cmath>
#include <iostream>
#include "grid.hpp"

Particle::Particle(
    std::vector<double> initialAcceleration,
    std::vector<double> initialPosition,
    double initialDensity,
    std::vector<double> initialHeadVector,
    std::vector<double> initialVelocityVector
    //std::vector<int> initialBlockIndexes
    ) : density(initialDensity),
    acceleration(std::move(initialAcceleration)),
    position(std::move(initialPosition)),
    headVector(std::move(initialHeadVector)),
    velocityVector(std::move(initialVelocityVector)),
    blockIndexes(std::vector<int>(3, 0)) {
  // Constructor implementation
}

double Particle::getDensity() const {
  return density;
}

std::vector<double> Particle::getAcceleration() const {
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

void Particle::setPosition(double px, double py, double pz) {
  position[0] = px;
  position[1] = py;
  position[2] = pz;
}

void Particle::setBlockIndexes(int cx, int cy, int cz) {
  blockIndexes[0] = cx;
  blockIndexes[1] = cy;
  blockIndexes[2] = cz;
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

// Función para calcular los índices de bloque para cada partícula
void reposicionarParticulas(std::vector<Particle> &particles, std::vector<int> numBloques,
                            std::vector<double> tamanoBloques) {
  for (auto &current_particle : particles) {
    std::vector<double> current_position = current_particle.getPosition();
    int block_x = std::floor((current_position[0] - Limite_Inferior[0]) / tamanoBloques[0]);
    int block_y = std::floor((current_position[1] - Limite_Inferior[1]) / tamanoBloques[1]);
    int block_z = std::floor((current_position[2] - Limite_Inferior[2]) / tamanoBloques[2]);
    // Ajustar los índices al rango permitido
    block_x = std::max(0, std::min(block_x, numBloques[0] - 1));
    block_y = std::max(0, std::min(block_y, numBloques[1] - 1));
    block_z = std::max(0, std::min(block_z, numBloques[2] - 1));
    current_particle.setBlockIndexes(block_x, block_y, block_z);
  }
}
