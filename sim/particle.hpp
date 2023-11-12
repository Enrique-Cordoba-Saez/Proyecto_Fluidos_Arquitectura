#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include <iostream>
#include <vector>
#include <cmath>
#include <array>

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
    void setAcceleration (double x, double y, double z);
    void setDensity(double x);
    void setPosition(double px, double py, double pz);
    void setBlockIndexes(int cx, int cy, int cz);
    void setVelocityVector(double vx, double vy, double vz);
    void setHeadVector(double hvx, double hvy, double hvz);

  private:
    double density;
    std::vector<double> acceleration;
    std::vector<double> position;
    std::vector<double> headVector;
    std::vector<double> velocityVector;
    std::vector<int> blockIndexes;
};

void reposicionarParticulas(std::vector<Particle> &particles, std::vector<int> numBloques,
                                        std::vector<double> tamanoBloques, std::vector<std::vector<std::vector<std::vector<int>>>> &Bloques);

void vaciarBloques(std::vector<std::vector<std::vector<std::vector<int>>>> & Bloques);

void movimientoParticulas(std::vector<Particle> &particles);

void chocarParticulasRecinto(std::vector<Particle> &particles, std::vector<int>const & maximo_indice_bloque);

void En_Eje_x(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double new_position_x);

void En_Eje_y(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double new_position_y);

void En_Eje_z(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
              std::vector<int> const & posiciones_particula, double new_position_z);

void chocarParticulasRecintoParte5(std::vector<Particle> &particles, std::vector<int>const & maximo_indice_bloque);

void En_Eje_x_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula);

void En_Eje_y_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula);

void En_Eje_z_Parte5(std::vector<int> const & maximo_indice_bloque, Particle & current_particle,
                     std::vector<int> const & posiciones_particula);



// CALCULO DE ACELERACIONES
void calculoAceleraciones(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                          double const Masa_Particula_m,
                          const std::vector<std::vector<std::vector<std::vector<int>>>>& Bloques);

void incrementoDensidad(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                        std::vector<std::vector<std::vector<std::vector<int>>>> Bloques);

void transformacionDensidad(Particle& particula, double const Longitud_Suavizado_h,
                            double const Masa_Particula_m);

void caso_x(std::vector<std::vector<std::vector<std::vector<int>>>> const & Bloques,
            std::vector<int> const & particles_block, int & x_min, int & x_max);

void caso_y(std::vector<std::vector<std::vector<std::vector<int>>>> const & Bloques,
            std::vector<int> const & particles_block, int & y_min, int & y_max);

void caso_z(std::vector<std::vector<std::vector<std::vector<int>>>> const & Bloques,
            std::vector<int> const & particles_block, int & z_min, int & z_max);

void calculosIncrementoDensidad(Particle & particula, Particle & particula2,
                                double const Longitud_Suavizado_h);

void transferenciaAceleracion(std::vector<Particle> & particulas, double const Longitud_Suavizado_h,
                              double const Masa_Particula_m,
                              std::vector<std::vector<std::vector<std::vector<int>>>> Bloques);

void calculoTransferenciaAceleracion(Particle& particula, Particle& particula2, double const Longitud_Suavizado_h,
                                     double const Masa_Particula_m);

void printParticle(Particle& particula);


#endif // PARTICLE_HPP

