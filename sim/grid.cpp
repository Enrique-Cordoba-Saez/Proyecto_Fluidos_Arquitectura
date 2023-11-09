#include "grid.hpp"

#include <iostream>
#include <vector>

std::vector<int> calcularNumBloques(double Longitud_Suavizado_h) {
  const std::vector<int> Numero_Bloques = {int(std::floor((Limite_Superior[0]-Limite_Inferior[0])/Longitud_Suavizado_h)),
                                           int(std::floor((Limite_Superior[1]-Limite_Inferior[1])/Longitud_Suavizado_h)),
                                           int(std::floor((Limite_Superior[2]-Limite_Inferior[2])/Longitud_Suavizado_h))};
  return Numero_Bloques;
}

std::vector<std::vector<std::vector<std::vector<int>>>> crearBloques(std::vector<int> Numero_Bloques) {
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
        for (int const p_id : Bloques[i][j][k]) {
          std::cout << p_id << " ";
        }
        total_particulas += Bloques[i][j][k].size();
        std::cout << "\n";
      }
    }
  }
  std::cout << "Partículas leídas en bloques: " << total_particulas << "\n";
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