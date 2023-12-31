#include "particle.hpp"

#include "grid.hpp"

Particle::Particle(std::vector<double> initialAcceleration, std::vector<double> initialPosition,
                   double initialDensity, std::vector<double> initialHeadVector,
                   std::vector<double> initialVelocityVector
                   // std::vector<int> initialBlockIndexes
                   )
  : density(initialDensity), acceleration(std::move(initialAcceleration)),
    position(std::move(initialPosition)), headVector(std::move(initialHeadVector)),
    velocityVector(std::move(initialVelocityVector)), blockIndexes(std::vector<int>(3, 0)) {
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

void Particle::setAcceleration(double ax, double ay, double az) {
  acceleration[0] = ax;
  acceleration[1] = ay;
  acceleration[2] = az;
}

void Particle::setDensity(double x) {
  density = x;
}

// Función para calcular los índices de bloque para cada partícula y guardar los índices de esas
// partículas en sus bloques. También guarda en otro vector los índices de las partículas que están
// en un bloque colindante con los límites del recinto
void reposicionarParticulas(std::vector<Particle> & particles, std::vector<int> numBloques,
                            std::vector<double> tamanoBloques,
                            std::vector<std::vector<std::vector<std::vector<int>>>> & Bloques) {
  vaciarBloques(Bloques);
  for (size_t i = 0; i < particles.size(); i++) {
    std::vector<double> current_position = particles[i].getPosition();
    int block_x = std::floor((current_position[0] - Limite_Inferior[0]) / tamanoBloques[0]);
    int block_y = std::floor((current_position[1] - Limite_Inferior[1]) / tamanoBloques[1]);
    int block_z = std::floor((current_position[2] - Limite_Inferior[2]) / tamanoBloques[2]);
    // Ajustar los índices al rango permitido
    block_x = std::max(0, std::min(block_x, numBloques[0] - 1));
    block_y = std::max(0, std::min(block_y, numBloques[1] - 1));
    block_z = std::max(0, std::min(block_z, numBloques[2] - 1));
    particles[i].setBlockIndexes(block_x, block_y, block_z);
    // Guardar id de partículas en bloques
    int const p_id = static_cast<int>(i);
    Bloques[block_x][block_y][block_z].push_back(p_id);
  }
}

// Función para vaciar todos los bloques en cada paso de tiempo
void vaciarBloques(std::vector<std::vector<std::vector<std::vector<int>>>> & Bloques) {
  for (auto & bloque_x : Bloques) {
    for (auto & bloque_y : bloque_x) {
      for (auto & bloque_z : bloque_y) { bloque_z.clear(); }
    }
  }
}

// Función para actualizar las aceleraciones de particulas basandose en
// choques con las paredes del recinto (PARTE 3 del procesamiento de la simulación)
void chocarParticulasRecinto(std::vector<Particle> & particles,
                             std::vector<int> const & maximo_indice_bloque) {
  for (auto & current_particle : particles) {
    // Aquí el vector "posiciones_particula" hace referencia a los indices del
    //  bloque en los que se encuentra la particula con la que estamos tratando
    std::vector<int> const posiciones_particula = current_particle.getBlockIndexes();

    double const new_position_x =
        current_particle.getPosition()[0] + current_particle.getHeadVector()[0] * Paso_de_tiempo;

    double const new_position_y =
        current_particle.getPosition()[1] + current_particle.getHeadVector()[1] * Paso_de_tiempo;

    double const new_position_z =
        current_particle.getPosition()[2] + current_particle.getHeadVector()[2] * Paso_de_tiempo;

    En_Eje_x(maximo_indice_bloque, current_particle, posiciones_particula, new_position_x);

    En_Eje_y(maximo_indice_bloque, current_particle, posiciones_particula, new_position_y);

    En_Eje_z(maximo_indice_bloque, current_particle, posiciones_particula, new_position_z);
  }
}

// Función parte de chocarParticulasRecinto(clang.tidy)
void En_Eje_z(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double const new_position_z) {
  if (posiciones_particula[2] == 0) {
    double const diferencia_con_limite = Tamano_de_particula - new_position_z + Limite_Inferior[2];
    if (diferencia_con_limite > pow(diez, -diez)) {
      double const new_acceleration = current_particle.getAcceleration()[2] +
                                      Colisiones_De_Rigidez * diferencia_con_limite -
                                      Amortiguamiento * current_particle.getVelocityVector()[2];
      current_particle.setAcceleration(current_particle.getAcceleration()[0],
                                       current_particle.getAcceleration()[1], new_acceleration);
    }
  }
  if (posiciones_particula[2] == maximo_indice_bloque[2] - 1) {
    double const diferencia_con_limite = Tamano_de_particula - Limite_Superior[2] + new_position_z;
    if (diferencia_con_limite > pow(diez, -diez)) {
      double const new_acceleration = current_particle.getAcceleration()[2] -
                                      Colisiones_De_Rigidez * diferencia_con_limite -
                                      Amortiguamiento * current_particle.getVelocityVector()[2];
      current_particle.setAcceleration(current_particle.getAcceleration()[0],
                                       current_particle.getAcceleration()[1], new_acceleration);
    }
  }
}

// Función parte de chocarParticulasRecinto(clang.tidy)
void En_Eje_y(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double const new_position_y) {
  if (posiciones_particula[1] == 0) {
    double const diferencia_con_limite = Tamano_de_particula - new_position_y + Limite_Inferior[1];
    if (diferencia_con_limite > pow(diez, -diez)) {
      double const new_acceleration = current_particle.getAcceleration()[1] +
                                      Colisiones_De_Rigidez * diferencia_con_limite -
                                      Amortiguamiento * current_particle.getVelocityVector()[1];
      current_particle.setAcceleration(current_particle.getAcceleration()[0], new_acceleration,
                                       current_particle.getAcceleration()[2]);
    }
  }
  if (posiciones_particula[1] == maximo_indice_bloque[1] - 1) {
    double const diferencia_con_limite = Tamano_de_particula - Limite_Superior[1] + new_position_y;
    if (diferencia_con_limite > pow(diez, -diez)) {
      double const new_acceleration = current_particle.getAcceleration()[1] -
                                      Colisiones_De_Rigidez * diferencia_con_limite -
                                      Amortiguamiento * current_particle.getVelocityVector()[1];
      current_particle.setAcceleration(current_particle.getAcceleration()[0], new_acceleration,
                                       current_particle.getAcceleration()[2]);
    }
  }
}

// Función parte de chocarParticulasRecinto(clang.tidy)
void En_Eje_x(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double const new_position_x) {
  if (posiciones_particula[0] == 0) {
    double const diferencia_con_limite = Tamano_de_particula - new_position_x + Limite_Inferior[0];
    if (diferencia_con_limite > pow(diez, -diez)) {
      double const new_acceleration = current_particle.getAcceleration()[0] +
                                      Colisiones_De_Rigidez * diferencia_con_limite -
                                      Amortiguamiento * current_particle.getVelocityVector()[0];
      current_particle.setAcceleration(new_acceleration, current_particle.getAcceleration()[1],
                                       current_particle.getAcceleration()[2]);
    }
  }
  if (posiciones_particula[0] == maximo_indice_bloque[0] - 1) {
    double const diferencia_con_limite = Tamano_de_particula - Limite_Superior[0] + new_position_x;
    if (diferencia_con_limite > pow(diez, -diez)) {
      double const new_acceleration = current_particle.getAcceleration()[0] -
                                      Colisiones_De_Rigidez * diferencia_con_limite -
                                      Amortiguamiento * current_particle.getVelocityVector()[0];
      current_particle.setAcceleration(new_acceleration, current_particle.getAcceleration()[1],
                                       current_particle.getAcceleration()[2]);
    }
  }
}

// Función para actualizar todas las partículas
void movimientoParticulas(std::vector<Particle> & particles) {
  for (auto & current_particle : particles) {
    std::vector<double> position     = current_particle.getPosition();
    std::vector<double> velocity     = current_particle.getVelocityVector();
    std::vector<double> head_vector  = current_particle.getHeadVector();
    std::vector<double> acceleration = current_particle.getAcceleration();
    // Para cada dimensión x, y, z (un solo bucle para actualizar los tres vectores, fusión de
    // bucles)
    for (int i = 0; i < 3; ++i) {
      position[i] +=
          head_vector[i] * Paso_de_tiempo + acceleration[i] * std::pow(Paso_de_tiempo, 2);
      velocity[i]     = head_vector[i] + (acceleration[i] * Paso_de_tiempo) / 2;
      head_vector[i] += acceleration[i] * Paso_de_tiempo;
    }
    current_particle.setPosition(position[0], position[1], position[2]);
    current_particle.setVelocityVector(velocity[0], velocity[1], velocity[2]);
    current_particle.setHeadVector(head_vector[0], head_vector[1], head_vector[2]);
  }
}

// Función para actualizar las velocidades y head vectors de particulas basandose en
// choques con las paredes del recinte (PARTE CINCO del procesamiento de la simulación)
void chocarParticulasRecintoParte5(std::vector<Particle> & particles,
                                   std::vector<int> const & maximo_indice_bloque) {
  for (auto & current_particle : particles) {
    std::vector<int> const posiciones_particula = current_particle.getBlockIndexes();

    En_Eje_x_Parte5(maximo_indice_bloque, current_particle, posiciones_particula);

    En_Eje_y_Parte5(maximo_indice_bloque, current_particle, posiciones_particula);

    En_Eje_z_Parte5(maximo_indice_bloque, current_particle, posiciones_particula);
  }
}

// Función parte de chocarParticulasRecintoParte5(clang.tidy)
void En_Eje_z_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula) {
  if (posiciones_particula[2] == 0 || posiciones_particula[2] == maximo_indice_bloque[2] - 1) {
    // Caso límite superior
    double diferencia_con_limite = Limite_Superior[2] - current_particle.getPosition()[2];
    double new_position_z        = Limite_Superior[2] + diferencia_con_limite;
    // Caso límite inferior
    if (posiciones_particula[2] == 0) {
      diferencia_con_limite = current_particle.getPosition()[2] - Limite_Inferior[2];
      new_position_z        = Limite_Inferior[2] - diferencia_con_limite;
    }
    if (diferencia_con_limite < 0) {
      current_particle.setPosition(current_particle.getPosition()[0],
                                   current_particle.getPosition()[1], new_position_z);
      current_particle.setHeadVector(current_particle.getHeadVector()[0],
                                     current_particle.getHeadVector()[1],
                                     current_particle.getHeadVector()[2] * -1);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0],
                                         current_particle.getVelocityVector()[1],
                                         current_particle.getVelocityVector()[2] * -1);
    }
  }
}

// Función parte de chocarParticulasRecintoParte5(clang.tidy)
void En_Eje_y_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula) {
  if (posiciones_particula[1] == 0 || posiciones_particula[1] == maximo_indice_bloque[1] - 1) {
    // Caso límite superior
    double diferencia_con_limite = Limite_Superior[1] - current_particle.getPosition()[1];
    double new_position_y        = Limite_Superior[1] + diferencia_con_limite;
    // Caso límite inferior
    if (posiciones_particula[1] == 0) {
      diferencia_con_limite = current_particle.getPosition()[1] - Limite_Inferior[1];
      new_position_y        = Limite_Inferior[1] - diferencia_con_limite;
    }
    if (diferencia_con_limite < 0) {
      current_particle.setPosition(current_particle.getPosition()[0], new_position_y,
                                   current_particle.getPosition()[2]);
      current_particle.setHeadVector(current_particle.getHeadVector()[0],
                                     current_particle.getHeadVector()[1] * -1,
                                     current_particle.getHeadVector()[2]);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0],
                                         current_particle.getVelocityVector()[1] * -1,
                                         current_particle.getVelocityVector()[2]);
    }
  }
}

// Función parte de chocarParticulasRecintoParte5(clang.tidy)
void En_Eje_x_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula) {
  if (posiciones_particula[0] == 0 || posiciones_particula[0] == maximo_indice_bloque[0] - 1) {
    // Caso límite superior
    double diferencia_con_limite = Limite_Superior[0] - current_particle.getPosition()[0];
    double new_position_x        = Limite_Superior[0] + diferencia_con_limite;
    // Caso límite inferior
    if (posiciones_particula[0] == 0) {
      diferencia_con_limite = current_particle.getPosition()[0] - Limite_Inferior[0];
      new_position_x        = Limite_Inferior[0] - diferencia_con_limite;
    }
    if (diferencia_con_limite < 0) {
      current_particle.setPosition(new_position_x, current_particle.getPosition()[1],
                                   current_particle.getPosition()[2]);
      current_particle.setHeadVector(current_particle.getHeadVector()[0] * -1,
                                     current_particle.getHeadVector()[1],
                                     current_particle.getHeadVector()[2]);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0] * -1,
                                         current_particle.getVelocityVector()[1],
                                         current_particle.getVelocityVector()[2]);
    }
  }
}

// Función para hacer el cálculo de las aceleraciones
void calculoAceleraciones(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                          double const Masa_Particula_m,
                          std::vector<std::vector<std::vector<std::vector<int>>>> const & Bloques) {
  for (auto & particula : particulas) {
    particula.setAcceleration(0.0, -Gravedad, 0.0);
    particula.setDensity(0.0);
  }
  incrementoDensidad(particulas, Longitud_Suavizado_h, Bloques);
  for (auto & particula : particulas) {
    transformacionDensidad(particula, Longitud_Suavizado_h, Masa_Particula_m);
  }
  transferenciaAceleracion(particulas, Longitud_Suavizado_h, Masa_Particula_m, Bloques);
}

// Función para calcular el incremento de las densidades
void incrementoDensidad(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                        std::vector<std::vector<std::vector<std::vector<int>>>> Bloques) {
  for (int i = 0; i < static_cast<int>(particulas.size()); ++i) {
    Particle & particula             = particulas[i];
    std::vector<int> particles_block = particula.getBlockIndexes();
    int x_min                        = particles_block[0] - 1;
    int y_min                        = particles_block[1] - 1;
    int z_min                        = particles_block[2] - 1;
    int x_max                        = particles_block[0] + 1;
    int y_max                        = particles_block[1] + 1;
    int z_max                        = particles_block[2] + 1;
    caso_x(Bloques, particles_block, x_min, x_max);
    caso_y(Bloques, particles_block, y_min, y_max);
    caso_z(Bloques, particles_block, z_min, z_max);
    for (int block_x = x_min; block_x <= x_max; ++block_x) {
      for (int block_y = y_min; block_y <= y_max; ++block_y) {
        for (int block_z = z_min; block_z <= z_max; ++block_z) {
          for (auto other_i : Bloques[block_x][block_y][block_z]) {
            if (other_i > i) {
              calculosIncrementoDensidad(particula, particulas[other_i], Longitud_Suavizado_h);
            }
          }
        }
      }
    }
  }
}

// Función para hacer los cálculos de los incrementos de las densidades
void calculosIncrementoDensidad(Particle & particula, Particle & particula2,
                                double const Longitud_Suavizado_h) {
  double const Longitud_Suavizado_h_2 = Longitud_Suavizado_h * Longitud_Suavizado_h;
  double const distancia = std::pow(particula.getPosition()[0] - particula2.getPosition()[0], 2) +
                           std::pow(particula.getPosition()[1] - particula2.getPosition()[1], 2) +
                           std::pow(particula.getPosition()[2] - particula2.getPosition()[2], 2);
  if (distancia < Longitud_Suavizado_h_2) {
    double const var_den = std::pow(Longitud_Suavizado_h_2 - distancia, 3);
    particula.setDensity(particula.getDensity() + var_den);
    particula2.setDensity(particula2.getDensity() + var_den);
  }
}

// Funciones para calcular el índice de bloque mínimo y máximo en casos especiales
void caso_z(std::vector<std::vector<std::vector<std::vector<int>>>> const & Bloques,
            std::vector<int> const & particles_block, int & z_min, int & z_max) {
  if (particles_block[2] == 0) {
    z_min += 1;
  } else if (particles_block[2] == static_cast<int>(Bloques[0][0].size()) - 1) {
    z_max -= 1;
  }
}

void caso_y(std::vector<std::vector<std::vector<std::vector<int>>>> const & Bloques,
            std::vector<int> const & particles_block, int & y_min, int & y_max) {
  if (particles_block[1] == 0) {
    y_min += 1;
  } else if (particles_block[1] == static_cast<int>(Bloques[0].size()) - 1) {
    y_max -= 1;
  }
}

void caso_x(std::vector<std::vector<std::vector<std::vector<int>>>> const & Bloques,
            std::vector<int> const & particles_block, int & x_min, int & x_max) {
  if (particles_block[0] == 0) {
    x_min += 1;
  } else if (particles_block[0] == static_cast<int>(Bloques.size()) - 1) {
    x_max -= 1;
  }
}

// Función para transformar las densidades una vez calculadas
void transformacionDensidad(Particle & particula, double const Longitud_Suavizado_h,
                            double const Masa_Particula_m) {
  double const factor = (particula.getDensity() + std::pow(Longitud_Suavizado_h, seis)) *
                        (315 / (64 * Numero_Pi * std::pow(Longitud_Suavizado_h, nueve))) *
                        Masa_Particula_m;
  particula.setDensity(factor);
}

// Función para hacer la transferencia de la aceleración
void transferenciaAceleracion(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                              double const Masa_Particula_m,
                              std::vector<std::vector<std::vector<std::vector<int>>>> Bloques) {
  for (int i = 0; i < static_cast<int>(particulas.size()); ++i) {
    std::vector<int> particles_block = particulas[i].getBlockIndexes();
    int x_min                        = particles_block[0] - 1;
    int y_min                        = particles_block[1] - 1;
    int z_min                        = particles_block[2] - 1;
    int x_max                        = particles_block[0] + 1;
    int y_max                        = particles_block[1] + 1;
    int z_max                        = particles_block[2] + 1;
    caso_x(Bloques, particles_block, x_min, x_max);
    caso_y(Bloques, particles_block, y_min, y_max);
    caso_z(Bloques, particles_block, z_min, z_max);
    for (int block_x = x_min; block_x <= x_max; ++block_x) {
      for (int block_y = y_min; block_y <= y_max; ++block_y) {
        for (int block_z = z_min; block_z <= z_max; ++block_z) {
          for (auto other_i : Bloques[block_x][block_y][block_z]) {
            if (other_i > i) {
              calculoTransferenciaAceleracion(particulas[i], particulas[other_i],
                                              Longitud_Suavizado_h, Masa_Particula_m);
            }
          }
        }
      }
    }
  }
}

// Función para hacer los cálculos de la transferencia de aceleración
void calculoTransferenciaAceleracion(Particle & particula, Particle & particula2,
                                     double const Longitud_Suavizado_h,
                                     double const Masa_Particula_m) {
  double const distancia = std::pow(particula.getPosition()[0] - particula2.getPosition()[0], 2) +
                           std::pow(particula.getPosition()[1] - particula2.getPosition()[1], 2) +
                           std::pow(particula.getPosition()[2] - particula2.getPosition()[2], 2);
  double const Longitud_Suavizado_h_2 = Longitud_Suavizado_h * Longitud_Suavizado_h;
  if (distancia < Longitud_Suavizado_h_2) {
    double const dist = std::sqrt(std::max(distancia, 1e-12));
    std::vector<double> factor1{0, 0, 0};
    std::vector<double> factor6{0, 0, 0};
    factor1_6(particula, particula2, factor1, factor6);
    double const factor2 = 15 / (Numero_Pi * std::pow(Longitud_Suavizado_h, 6)) *
                           (3 * Masa_Particula_m * Presion_De_Rigidez) / 2 *
                           std::pow((Longitud_Suavizado_h - dist), 2) / dist;
    double const factor5 =
        particula.getDensity() + particula2.getDensity() - (2 * Densidad_De_Fluido);
    double const factor7 =
        (45 / (Numero_Pi * std::pow(Longitud_Suavizado_h, 6))) * Viscosidad * Masa_Particula_m;
    double const factor8 = particula.getDensity() * particula2.getDensity();
    std::vector<double> result{0, 0, 0};
    for (int i = 0; i < 3; i++) {
      result[i] = (factor1[i] * factor2 * factor5 + factor6[i] * factor7) / factor8;
    }
    resultadosCalculoTransferencia(particula, particula2, result);
  }
}

void factor1_6(Particle const & particula, Particle const & particula2,
               std::vector<double> & factor1, std::vector<double> & factor6) {
  for (int i = 0; i < 3; i++) {
    factor1[i] = particula.getPosition()[i] - particula2.getPosition()[i];
    factor6[i] = particula2.getVelocityVector()[i] - particula.getVelocityVector()[i];
  }
}

void resultadosCalculoTransferencia(Particle & particula, Particle & particula2,
                                    std::vector<double> const & result) {
  particula.setAcceleration(particula.getAcceleration()[0] + result[0],
                            particula.getAcceleration()[1] + result[1],
                            particula.getAcceleration()[2] + result[2]);
  particula2.setAcceleration(particula2.getAcceleration()[0] - result[0],
                             particula2.getAcceleration()[1] - result[1],
                             particula2.getAcceleration()[2] - result[2]);
}
