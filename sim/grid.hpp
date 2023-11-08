#ifndef PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
#define PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
#include <array>
#include <cmath>
#include <vector>
#include <iostream>

//Declaración de constantes escalares de la simulación
  const double Multiplicador_De_Radio = 1.695;
  const int Densidad_De_Fluido = 1000;
  const double Presion_De_Rigidez = 3.0;
  const int Colisiones_De_Rigidez = 30000;
  const double Amortiguamiento = 128.0;
  const double Viscosidad = 0.4;
  const double Tamano_de_particula = 0.0002;
  const double Paso_de_tiempo = 0.001;
  const double Gravedad = 9.8; //Librería?
  const double Numero_Pi = 2* acos(0.0); //Librería?

  const int cinco = 5;
  const int seis = 6;
  const int siete = 7;
  const int ocho = 8;
  const int nueve = 9;
  const int diez = 10;

  //Declaración de constantes vectoriales de la simulación
  const std::array<double, 3> Aceleracion_Externa{0.0, -9.8, 0.0};
  const std::array<double, 3> Limite_Superior{0.065, 0.1, 0.065};
  const std::array<double,3> Limite_Inferior{-0.065, -0.08, -0.065};

  std::vector<int> calcularNumBloques(double Longitud_Suavizado_h);

  std::vector<std::vector<std::vector<std::vector<int>>>> crearBloques(std::vector<int> Numero_Bloques);

  void printBloques(std::vector<std::vector<std::vector<std::vector<int>>>> Bloques);

#endif //PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
