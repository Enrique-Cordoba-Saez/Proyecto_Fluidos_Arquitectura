#include "particle.hpp"

// #include <math.h>
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

void Particle::setVelocityVector(double vx, double vy, double vz) {
  velocityVector[0] = vx;
  velocityVector[1] = vy;
  velocityVector[2] = vz;
}

void Particle::setHeadVector(double hvx, double hvy, double hvz) {
  headVector[0] = hvx;
  headVector[1] = hvy;
  headVector[2] = hvz;
}

void Particle::setAcceleration(double ax, double ay, double az){
  acceleration[0] = ax;
  acceleration[1] = ay;
  acceleration[2] = az;
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

// Función para actualizar las aceleraciones de particulas en base a
//chocar con las paredes del recinte (Parte 3 del procesamiento de la simulación)
void chocarParticulasRecinto(std::vector<Particle> &particles, std::vector<int>const & maximo_indice_bloque){
  for (auto &current_particle : particles){
    const std::vector<int> posiciones_particula = current_particle.getBlockIndexes();

    double const new_position_x = current_particle.getPosition()[0] + current_particle.getHeadVector()[0] * Paso_de_tiempo;
    current_particle.setPosition(new_position_x, current_particle.getPosition()[1], current_particle.getPosition()[2]);

    double const new_position_y = current_particle.getPosition()[1] + current_particle.getHeadVector()[1] * Paso_de_tiempo;
    current_particle.setPosition(current_particle.getPosition()[0], new_position_y, current_particle.getPosition()[2]);

    double const new_position_z = current_particle.getPosition()[2] + current_particle.getHeadVector()[2] * Paso_de_tiempo;
    current_particle.setPosition(current_particle.getPosition()[0], current_particle.getPosition()[1], new_position_z);

    En_Eje_x(maximo_indice_bloque, current_particle, posiciones_particula, new_position_x);

    En_Eje_y(maximo_indice_bloque, current_particle, posiciones_particula, new_position_y);

    En_Eje_z(maximo_indice_bloque, current_particle, posiciones_particula, new_position_z);
  }
}

// Función parte de chocarParticulasRecinto(clang.tidy)
void En_Eje_z(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double const new_position_z) {
  if (posiciones_particula[2] == 0){
    double const diferencia_con_limite = Tamano_de_particula - new_position_z + Limite_Inferior[2];
    if (diferencia_con_limite > pow(diez, -diez)){
          double const new_acceleration = current_particle.getAcceleration()[2] + Colisiones_De_Rigidez * diferencia_con_limite
                                          - Amortiguamiento * current_particle.getVelocityVector()[2];
          current_particle.setAcceleration(current_particle.getAcceleration()[0], current_particle.getAcceleration()[1],
                                           new_acceleration);
  }
  }
  if (posiciones_particula[2] == maximo_indice_bloque[2] - 1){
  double const diferencia_con_limite = Tamano_de_particula - Limite_Superior[2] + new_position_z;
  if (diferencia_con_limite > pow(diez, -diez)){
          double const new_acceleration = current_particle.getAcceleration()[2] - Colisiones_De_Rigidez * diferencia_con_limite
                                          - Amortiguamiento * current_particle.getVelocityVector()[2];
          current_particle.setAcceleration(current_particle.getAcceleration()[0], current_particle.getAcceleration()[1],
                                           new_acceleration);
  }}
}

// Función parte de chocarParticulasRecinto(clang.tidy)
void En_Eje_y(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double const new_position_y) {
  if (posiciones_particula[1] == 0){
    double const diferencia_con_limite = Tamano_de_particula - new_position_y + Limite_Inferior[1];
    if (diferencia_con_limite > pow(diez, -diez)){
      double const new_acceleration = current_particle.getAcceleration()[1] + Colisiones_De_Rigidez * diferencia_con_limite
                                      - Amortiguamiento * current_particle.getVelocityVector()[1];
      current_particle.setAcceleration(current_particle.getAcceleration()[0], new_acceleration,
                                     current_particle.getAcceleration()[2]);
  }
  }
  if (posiciones_particula[1] == maximo_indice_bloque[1] - 1){
    double const diferencia_con_limite = Tamano_de_particula - Limite_Superior[1] + new_position_y;
    if (diferencia_con_limite > pow(diez, -diez)){
        double const new_acceleration = current_particle.getAcceleration()[1] - Colisiones_De_Rigidez * diferencia_con_limite
                                        - Amortiguamiento * current_particle.getVelocityVector()[1];
        current_particle.setAcceleration(current_particle.getAcceleration()[0], new_acceleration,
                                         current_particle.getAcceleration()[2]);
  }}
}

// Función parte de chocarParticulasRecinto(clang.tidy)
void En_Eje_x(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double const new_position_x) {
  if (posiciones_particula[0] == 0){
    double const diferencia_con_limite = Tamano_de_particula - new_position_x + Limite_Inferior[0];
    if (diferencia_con_limite > pow(diez, -diez)){
      double const new_acceleration = current_particle.getAcceleration()[0] + Colisiones_De_Rigidez * diferencia_con_limite
                                - Amortiguamiento * current_particle.getVelocityVector()[0];
      current_particle.setAcceleration(new_acceleration, current_particle.getAcceleration()[1],
                                       current_particle.getAcceleration()[2]);
  }
  }
  if (posiciones_particula[0] == maximo_indice_bloque[0] - 1){
    double const diferencia_con_limite = Tamano_de_particula - Limite_Superior[0] + new_position_x;
    if (diferencia_con_limite > pow(diez, -diez)){
      double const new_acceleration = current_particle.getAcceleration()[0] - Colisiones_De_Rigidez * diferencia_con_limite
                                - Amortiguamiento * current_particle.getVelocityVector()[0];
      current_particle.setAcceleration(new_acceleration, current_particle.getAcceleration()[1],
                                       current_particle.getAcceleration()[2]);
  }}
}

// Función para actualizar todas las partículas
void movimientoParticulas(std::vector<Particle> &particles) {
  for (auto &current_particle : particles) {
    std::vector<double> position = current_particle.getPosition();
    std::vector<double> velocity = current_particle.getVelocityVector();
    std::vector<double> head_vector = current_particle.getHeadVector();
    std::vector<double> acceleration = current_particle.getAcceleration();
    // Para cada dimensión x, y, z (un solo bucle para actualizar los tres vectores, fusión de bucles)
    for (int i=0; i < 3; ++i) {
      position[i] += head_vector[i] * Paso_de_tiempo + acceleration[i] * std::pow(Paso_de_tiempo, 2);
      velocity[i] = head_vector[i] + (acceleration[i] * Paso_de_tiempo) / 2;
      head_vector[i] += acceleration[i] * Paso_de_tiempo;
    }
    current_particle.setPosition(position[0], position[1], position[2]);
    current_particle.setVelocityVector(velocity[0], velocity[1], velocity[2]);
    current_particle.setHeadVector(head_vector[0], head_vector[1], head_vector[2]);
  }
}

