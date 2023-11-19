#include "grid.hpp"

#include <iostream>
#include <vector>

std::vector<int> calcularNumBloques(double Longitud_Suavizado_h) {
  std::vector<int> const Numero_Bloques = {
    int(std::floor((Limite_Superior[0] - Limite_Inferior[0]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[1] - Limite_Inferior[1]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[2] - Limite_Inferior[2]) / Longitud_Suavizado_h))};
  return Numero_Bloques;
}

std::vector<std::vector<std::vector<std::vector<int>>>>
    crearBloques(std::vector<int> Numero_Bloques) {
  std::vector<std::vector<std::vector<std::vector<int>>>> Bloques(
      Numero_Bloques[0], std::vector<std::vector<std::vector<int>>>(
                             Numero_Bloques[1], std::vector<std::vector<int>>(Numero_Bloques[2])));
  return Bloques;
}

void printBloques(std::vector<std::vector<std::vector<std::vector<int>>>> Bloques) {
  // Imprimir todos los valores
  size_t total_particulas = 0;
  for (size_t i = 0; i < Bloques.size(); ++i) {
    for (size_t j = 0; j < Bloques[i].size(); ++j) {
      for (size_t k = 0; k < Bloques[i][j].size(); ++k) {
        // Imprimir los valores del vector en la posición [i][j][k]
        std::cout << "Bloques[" << i << "][" << j << "][" << k << "]: ";
        for (int const p_id : Bloques[i][j][k]) { std::cout << p_id << " "; }
        total_particulas += Bloques[i][j][k].size();
        std::cout << "\n";
      }
    }
  }
  std::cout << "Partículas leídas en bloques: " << total_particulas << "\n";
}

void partesTresCuatroCinco(std::vector<int> const & Numero_Bloques,
                           std::vector<Particle> & Particulas) {
  chocarParticulasRecinto(Particulas, Numero_Bloques);
  movimientoParticulas(Particulas);
  chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
}

void creacionParticulas(std::vector<double> const & valoresDobles,
                        std::vector<Particle> & Particulas) {
  for (size_t i = 0; i < valoresDobles.size(); i += nueve) {
    Particle const nuevaParticula = Particle(
        std::vector<double>{0.0, 0.0, 0.0},
        std::vector<double>{valoresDobles[i], valoresDobles[i + 1], valoresDobles[i + 2]}, 0.0,
        std::vector<double>{valoresDobles[i + 3], valoresDobles[i + 4], valoresDobles[i + cinco]},
        std::vector<double>{valoresDobles[i + seis], valoresDobles[i + siete],
                            valoresDobles[i + ocho]});
    Particulas.push_back(nuevaParticula);
  }
}

std::vector<double> ParametroTamanoBloque(std::vector<int> const & Numero_Bloques) {
  std::vector<double> Tamano_Bloques = {
    (Limite_Superior[0] - Limite_Inferior[0]) / double(Numero_Bloques[0]),
    (Limite_Superior[1] - Limite_Inferior[1]) / double(Numero_Bloques[1]),
    (Limite_Superior[2] - Limite_Inferior[2]) / double(Numero_Bloques[2])};
  return Tamano_Bloques;
}

[[maybe_unused]] void Parametros1(ProgArgs const & procesador, double & Masa_Particula_m,
                                  double & Longitud_Suavizado_h,
                                  std::vector<int> & Numero_Bloques) {
  // Declaración de parámetros de la simulación
  Masa_Particula_m     = Densidad_De_Fluido / pow(procesador.getPpm(), 3);
  Longitud_Suavizado_h = Multiplicador_De_Radio / procesador.getPpm();
  Numero_Bloques       = calcularNumBloques(Longitud_Suavizado_h);
}

ProgArgs crearProcesador(int argc, char const * const * argv) {
  // Extraer argumentos
  std::span const args_view{argv, static_cast<size_t>(argc)};
  std::vector const args(args_view.begin() + 1, args_view.end());
  // Crear objeto de la clase ProgArgs
  auto procesador = ProgArgs(argc - 1, args);
  return procesador;
}

/*
void printBloques(std::vector<std::vector<std::vector<std::vector<int>>>> Bloques) {
  // Imprimir todos los valores
  size_t total_particulas = 0;
  int x_min = i-1;
  int y_min = i-1;
  int z_min = i-1;
  if (particle.blockx == 0) {
    x_min = 0;
  } else if (particle.blockx == nx-1) {
    x_max = i;
  }
  for (int x = i-1; x <= x+1; ++x) {
    for (int y = i-1; y <= y+1; ++y) {
      for (int z = i-1; z <= z+1; ++z) {
        for (auto: j, Bloques[x][y][z]) {
          Partiicle particula2 = particulas[j];





        }
      }
    }
  }
  std::cout << "Partículas leídas en bloques: " << total_particulas << "\n";
}
*/