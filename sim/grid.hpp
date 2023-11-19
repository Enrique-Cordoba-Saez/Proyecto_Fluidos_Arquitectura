#ifndef PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
#define PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
#include "sim/progargs.hpp"

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

// Declaración de constantes escalares de la simulación
double const Multiplicador_De_Radio = 1.695;
int const Densidad_De_Fluido        = 1000;
double const Presion_De_Rigidez     = 3.0;
int const Colisiones_De_Rigidez     = 30000;
double const Amortiguamiento        = 128.0;
double const Viscosidad             = 0.4;
double const Tamano_de_particula    = 0.0002;
double const Paso_de_tiempo         = 0.001;
double const Gravedad               = 9.8;
double const Numero_Pi              = 2 * acos(0.0);

int const cinco = 5;
int const seis  = 6;
int const siete = 7;
int const ocho  = 8;
int const nueve = 9;
int const diez  = 10;

// Declaración de constantes vectoriales de la simulación
std::array<double, 3> const Aceleracion_Externa{0.0, -9.8, 0.0};
std::array<double, 3> const Limite_Superior{0.065, 0.1, 0.065};
std::array<double, 3> const Limite_Inferior{-0.065, -0.08, -0.065};

std::vector<int> calcularNumBloques(double Longitud_Suavizado_h);

std::vector<std::vector<std::vector<std::vector<int>>>>
    crearBloques(std::vector<int> Numero_Bloques);

void printBloques(std::vector<std::vector<std::vector<std::vector<int>>>> Bloques);

ProgArgs crearProcesador(int argc, char const * const * argv);

void Parametros1(ProgArgs const & procesador, double & Masa_Particula_m,
                 double & Longitud_Suavizado_h, std::vector<int> & Numero_Bloques);

std::vector<double> ParametroTamanoBloque(std::vector<int> const & Numero_Bloques);

void creacionParticulas(std::vector<double> const & valoresDobles,
                        std::vector<Particle> & Particulas);

void partesTresCuatroCinco(std::vector<int> const & Numero_Bloques,
                           std::vector<Particle> & Particulas);

#endif  // PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
