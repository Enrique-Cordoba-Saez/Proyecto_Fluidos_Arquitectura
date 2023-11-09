#include "particle.hpp"

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

void Particle::setDensity(double x) {
  density = x;
}

// Función para calcular los índices de bloque para cada partícula y guardar los índices de esas
// partículas en sus bloques. También guarda en otro vector los índices de las partículas que están
// en un bloque colindante con los límites del recinto
std::vector<int> reposicionarParticulas(std::vector<Particle> &particles, std::vector<int> numBloques,
                                        std::vector<double> tamanoBloques, std::vector<std::vector<std::vector<std::vector<int>>>> &Bloques) {
  vaciarBloques(Bloques);
  std::vector<int> colindantes;
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
    // Guardar id de partículas colindantes con límite
    if (block_x == 0 || block_x == numBloques[0]-1 || block_y == 0 || block_y == numBloques[1]-1 ||
        block_z == 0 || block_z == numBloques[2]-1) {
      colindantes.push_back(p_id);
    }
  }
  return colindantes;
}

// Función para vaciar todos los bloques en cada paso de tiempo
void vaciarBloques(std::vector<std::vector<std::vector<std::vector<int>>>> & Bloques) {
  for (auto & bloque_x : Bloques) {
    for (auto & bloque_y : bloque_x) {
      for (auto & bloque_z : bloque_y) {
        bloque_z.clear();
      }
    }
  }
}

// Función para actualizar las aceleraciones de particulas basandose en
//choques con las paredes del recinto (PARTE 3 del procesamiento de la simulación)
void chocarParticulasRecinto(std::vector<Particle> &particles, std::vector<int>const & maximo_indice_bloque){
  for (auto &current_particle : particles){
    //Aquí el vector "posiciones_particula" hace referencia a los indices del
    // bloque en los que se encuentra la particula con la que estamos tratando
    const std::vector<int> posiciones_particula = current_particle.getBlockIndexes();

    double const new_position_x = current_particle.getPosition()[0] + current_particle.getHeadVector()[0] * Paso_de_tiempo;

    double const new_position_y = current_particle.getPosition()[1] + current_particle.getHeadVector()[1] * Paso_de_tiempo;

    double const new_position_z = current_particle.getPosition()[2] + current_particle.getHeadVector()[2] * Paso_de_tiempo;

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
// Función para actualizar las velocidades y head vectors de particulas basandose en
//choques con las paredes del recinte (PARTE CINCO del procesamiento de la simulación)
void chocarParticulasRecintoParte5(std::vector<Particle> &particles, std::vector<int>const & maximo_indice_bloque){
  for (auto &current_particle : particles){
    const std::vector<int> posiciones_particula = current_particle.getBlockIndexes();

    En_Eje_x_Parte5(maximo_indice_bloque, current_particle, posiciones_particula);

    En_Eje_y_Parte5(maximo_indice_bloque, current_particle, posiciones_particula);

    En_Eje_z_Parte5(maximo_indice_bloque, current_particle, posiciones_particula);
  }
}
// Función parte de chocarParticulasRecintoParte5(clang.tidy)
void En_Eje_z_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula) {
  if (posiciones_particula[2] == 0){
    double const diferencia_con_limite = current_particle.getPosition()[2] - Limite_Inferior[2];
    if (diferencia_con_limite < 0){
      double const new_position_z = Limite_Inferior[2] - diferencia_con_limite;
      current_particle.setPosition(current_particle.getPosition()[0], current_particle.getPosition()[1],
                                   new_position_z);
      current_particle.setHeadVector(current_particle.getHeadVector()[0], current_particle.getHeadVector()[1],
                                     current_particle.getHeadVector()[2] * -1);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0], current_particle.getVelocityVector()[1],
                                         current_particle.getVelocityVector()[2] * -1);
    }
  }
  if (posiciones_particula[2] == maximo_indice_bloque[2] - 1){
    double const diferencia_con_limite = Limite_Superior[2] - current_particle.getPosition()[2];
    if (diferencia_con_limite < 0){
      double const new_position_z = Limite_Superior[2] + diferencia_con_limite;
      current_particle.setPosition(current_particle.getPosition()[0], current_particle.getPosition()[1],
                                   new_position_z);
      current_particle.setHeadVector(current_particle.getHeadVector()[0], current_particle.getHeadVector()[1],
                                     current_particle.getHeadVector()[2] * -1);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0], current_particle.getVelocityVector()[1],
                                         current_particle.getVelocityVector()[2] * -1);
    }}
}
// Función parte de chocarParticulasRecintoParte5(clang.tidy)
void En_Eje_y_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula) {
  if (posiciones_particula[1] == 0){
    double const diferencia_con_limite = current_particle.getPosition()[1] - Limite_Inferior[1];
    if (diferencia_con_limite < 0){
      double const new_position_y = Limite_Inferior[1] - diferencia_con_limite;
      current_particle.setPosition(current_particle.getPosition()[0],new_position_y,
                                   current_particle.getPosition()[2]);
      current_particle.setHeadVector(current_particle.getHeadVector()[0], current_particle.getHeadVector()[1] * -1,
                                     current_particle.getHeadVector()[2]);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0], current_particle.getVelocityVector()[1]  * -1,
                                         current_particle.getVelocityVector()[2]);
    }
  }
  if (posiciones_particula[1] == maximo_indice_bloque[1] - 1){
    double const diferencia_con_limite = Limite_Superior[1] - current_particle.getPosition()[1];
    if (diferencia_con_limite < 0){
      double const new_position_y = Limite_Superior[1] + diferencia_con_limite;
      current_particle.setPosition(current_particle.getPosition()[0], new_position_y,
                                   current_particle.getPosition()[2]);
      current_particle.setHeadVector(current_particle.getHeadVector()[0], current_particle.getHeadVector()[1] * -1,
                                     current_particle.getHeadVector()[2]);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0], current_particle.getVelocityVector()[1] * -1,
                                         current_particle.getVelocityVector()[2]);
    }}
}
// Función parte de chocarParticulasRecintoParte5(clang.tidy)
void En_Eje_x_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula) {
  if (posiciones_particula[0] == 0){
    double const diferencia_con_limite = current_particle.getPosition()[0] - Limite_Inferior[0];
    if (diferencia_con_limite < 0){
      double const new_position_x = Limite_Inferior[0] - diferencia_con_limite;
      current_particle.setPosition(new_position_x, current_particle.getPosition()[1],
                                   current_particle.getPosition()[2]);
      current_particle.setHeadVector(current_particle.getHeadVector()[0] * -1, current_particle.getHeadVector()[1],
                                     current_particle.getHeadVector()[2]);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0] * -1, current_particle.getVelocityVector()[1],
                                         current_particle.getVelocityVector()[2]);
    }
  }
  if (posiciones_particula[0] == maximo_indice_bloque[0] - 1){
    double const diferencia_con_limite = Limite_Superior[0] - current_particle.getPosition()[0];
    if (diferencia_con_limite < 0){
      double const new_position_x = Limite_Superior[0] + diferencia_con_limite;
      current_particle.setPosition(new_position_x, current_particle.getPosition()[1],
                                   current_particle.getPosition()[2]);
      current_particle.setHeadVector(current_particle.getHeadVector()[0] * -1, current_particle.getHeadVector()[1],
                                     current_particle.getHeadVector()[2]);
      current_particle.setVelocityVector(current_particle.getVelocityVector()[0] * -1, current_particle.getVelocityVector()[1],
                                         current_particle.getVelocityVector()[2]);
    }}
}



// CALCULO DE ACELERACIONES
void calculoAceleraciones(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                          double const Masa_Particula_m,
                          const std::vector<std::vector<std::vector<std::vector<int>>>>& Bloques) {
  for (auto & particula : particulas) {
    particula.setAcceleration(0.0, -Gravedad, 0.0);
    particula.setDensity(0.0);
  }

  std::vector<std::array<int, 2>> const pares = incrementoDensidad(particulas, Longitud_Suavizado_h, Bloques);

  for (auto & particula : particulas) {
    transformacionDensidad(particula, Longitud_Suavizado_h, Masa_Particula_m);
  }

  for (size_t i = 0; i < particulas.size(); ++i) {
    transferenciaAceleracion(static_cast<int>(i), particulas[i], particulas, Longitud_Suavizado_h, Masa_Particula_m);
  }
}

std::vector<std::array<int, 2>> incrementoDensidad(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                                                   std::vector<std::vector<std::vector<std::vector<int>>>> Bloques) {
  std::vector<std::array<int, 2>> pares;
  for (int i = 0; i < static_cast<int>(particulas.size()); ++i) {
    Particle & particula = particulas[i];
    std::vector<int> particles_block = particula.getBlockIndexes();
    int x_min = particles_block[0] - 1;
    int y_min = particles_block[1] - 1;
    int z_min = particles_block[2] - 1;
    int x_max = particles_block[0] + 1;
    int y_max = particles_block[1] + 1;
    int z_max = particles_block[2] + 1;
    caso_x(Bloques, particles_block, x_min, x_max);
    caso_y(Bloques, particles_block, y_min, y_max);
    caso_z(Bloques, particles_block, z_min, z_max);
    for (int block_x = x_min; block_x <= x_max; ++block_x) {
      for (int block_y = y_min; block_y <= y_max; ++block_y) {
        for (int block_z = z_min; block_z <= z_max; ++block_z) {
          for (auto other_i : Bloques[block_x][block_y][block_z]) {
            if (other_i > i) {
              pares.push_back(std::array<int, 2>{i, other_i});
              Particle & particula2 = particulas[other_i];
              calculosIncrementoDensidad(Longitud_Suavizado_h, particula, particula2);
            }}}}}
  }
  return pares;
}

void calculosIncrementoDensidad(double const Longitud_Suavizado_h, Particle & particula,
                                Particle & particula2) {
  double h2           = Longitud_Suavizado_h * Longitud_Suavizado_h;
  double r = std::pow(particula.getPosition()[0] - particula2.getPosition()[0], 2) +
             std::pow(particula.getPosition()[1] - particula2.getPosition()[1], 2) +
             std::pow(particula.getPosition()[2] - particula2.getPosition()[2], 2);
  if (r < h2) {
    double var_den = std::pow(h2 - r, 3);
    particula.setDensity(particula.getDensity() + var_den);
    particula2.setDensity(particula2.getDensity() + var_den);
  }
}

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

void transformacionDensidad(Particle& particula, double const Longitud_Suavizado_h,
                            double const Masa_Particula_m){
  double factor = (particula.getDensity() + std::pow(Longitud_Suavizado_h, seis))
                  * (315 / (64 * Numero_Pi * std::pow(Longitud_Suavizado_h, nueve)))
                  * Masa_Particula_m;
  particula.setDensity(factor);
}

void transferenciaAceleracion(int index, Particle& particula, std::vector<Particle>& particulas, double const Longitud_Suavizado_h,
                              double const Masa_Particula_m) {
  for (size_t j = index + 1; j < particulas.size(); ++j) {
    Particle & particula2 = particulas[j];
    auto currentBlock = particula.getBlockIndexes();
    auto otherBlock = particula2.getBlockIndexes();
    // Comprobar que particula2 está en el mismo bloque o uno contiguo a particula
    if (std::abs(currentBlock[0] - otherBlock[0]) <= 1 &&
        std::abs(currentBlock[1] - otherBlock[1]) <= 1 &&
        std::abs(currentBlock[2] - otherBlock[2]) <= 1) {
      double h2 = Longitud_Suavizado_h * Longitud_Suavizado_h;
      double r = std::pow(particula.getPosition()[0] - particula2.getPosition()[0], 2) +
                 std::pow(particula.getPosition()[1] - particula2.getPosition()[1], 2) +
                 std::pow(particula.getPosition()[2] - particula2.getPosition()[2], 2);
      if (r < h2) {
        calculoTransferenciaAceleracion(particula, particula2, Longitud_Suavizado_h, Masa_Particula_m);
      }
    }
  }
}

//Se puede reducir la longitud, está así para que se vea claro
void calculoTransferenciaAceleracion(Particle& particula, Particle& particula2, double const Longitud_Suavizado_h,
                                     double const Masa_Particula_m){

  double r2 = std::pow(particula.getPosition()[0] - particula2.getPosition()[0], 2) +
              std::pow(particula.getPosition()[1] - particula2.getPosition()[1], 2) +
              std::pow(particula.getPosition()[2] - particula2.getPosition()[2], 2);

  double dist = std::sqrt(std::max(r2, 1e-12));

  double factor1x = particula.getPosition()[0] - particula2.getPosition()[0];
  double factor1y = particula.getPosition()[1] - particula2.getPosition()[1];
  double factor1z = particula.getPosition()[2] - particula2.getPosition()[2];

  double factor2 = 15 / (Numero_Pi * std::pow(Longitud_Suavizado_h, 6));

  double factor3 = (3 * Masa_Particula_m * Presion_De_Rigidez) / 2;

  double factor4 = std::pow((Longitud_Suavizado_h - dist), 2) / dist;

  double factor5 = particula.getDensity() + particula2.getDensity() - (2*Densidad_De_Fluido);

  double factor6x = particula2.getVelocityVector()[0] - particula.getVelocityVector()[0];
  double factor6y = particula2.getVelocityVector()[1] - particula.getVelocityVector()[1];
  double factor6z = particula2.getVelocityVector()[2] - particula.getVelocityVector()[2];

  double factor7 = (45 / (Numero_Pi * std::pow(Longitud_Suavizado_h, 6))) * Viscosidad * Masa_Particula_m;

  double factor8 = particula.getDensity() * particula2.getDensity();

  double resultx = (factor1x*factor2*factor3*factor4*factor5+factor6x*factor7)/factor8;
  double resulty = (factor1y*factor2*factor3*factor4*factor5+factor6y*factor7)/factor8;
  double resultz = (factor1z*factor2*factor3*factor4*factor5+factor6z*factor7)/factor8;

  particula.setAcceleration(particula.getAcceleration()[0] + resultx,
                            particula.getAcceleration()[1] + resulty,
                            particula.getAcceleration()[2] + resultz);

  particula2.setAcceleration(particula2.getAcceleration()[0] - resultx,
                             particula2.getAcceleration()[1] - resulty,
                             particula2.getAcceleration()[2] - resultz);

}

void printParticle(std::vector<Particle>& particulas, int index){
  std::cout << "Particula con index " << index << std::endl;
  std::vector<double> aceleracion0 = particulas[index].getAcceleration();
  std::vector<double> posicion0 = particulas[index].getPosition();
  std::vector<double> vectorHead0 = particulas[index].getHeadVector();
  std::vector<double> velocidad0 = particulas[index].getVelocityVector();
  double densidad = particulas[index].getDensity();

  std::cout << "----> Posicion " << posicion0[0] << " " << posicion0[1] << " " << posicion0[2] << std::endl;
  std::cout << "----> Head Vector " << vectorHead0[0] << " " << vectorHead0[1] << " " << vectorHead0[2] << std::endl;
  std::cout << "----> Velocidad " << velocidad0[0] << " " << velocidad0[1] << " " << velocidad0[2] << std::endl;
  std::cout << "----> Densidad " << densidad << std::endl;
  std::cout << "----> Aceleracion " << aceleracion0[0] << " " << aceleracion0[1] << " " << aceleracion0[2] << std::endl;
}