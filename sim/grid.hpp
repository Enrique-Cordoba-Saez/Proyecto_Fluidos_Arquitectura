#ifndef PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
#define PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
#include <array>
#include <cmath>

//Declaración de constantes escalares de la simulación
  const float Multiplicador_De_Radio = 1.695;
  const int Densidad_De_Fluido = 1000;
  const float Presion_De_Rigidez = 3.0;
  const int Colisiones_De_Rigidez = 30000;
  const float Amortiguamiento = 128.0;
  const float Viscosidad = 0.4;
  const float Tamano_de_particula = 0.0002;
  const float Paso_de_tiempo = 0.001;
  const float Gravedad = 9.8; //Librería?
  const float Pi = M_PI; //Librería?

  const int cinco = 5;
  const int seis = 6;
  const int siete = 7;
  const int ocho = 8;
  const int nueve = 9;
  const int diez = 10;

  //Declaración de constantes vectoriales de la simulación
  const std::array<float, 3> Aceleracion_Externa{0.0, -9.8, 0.0};
  const std::array<float, 3> Limite_Superior{0.065, 0.1, 0.065};
  const std::array<float,3> Limite_Inferior{-0.065, -0.08, -0.065};

#endif //PROYECTO_FLUIDOS_ARQUITECTURA_GRID_HPP
