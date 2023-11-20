/*
#include "block.hpp"

#include <iostream>

// Clase no utilizada
[[maybe_unused]] Block::Block(int n_particulas, std::vector<int> posicion,
                              std::vector<double> tamano, std::vector<int> adyacentes)
  : n_particulas(n_particulas), posicion(std::move(posicion)), tamano(std::move(tamano)),
    bloques_adyacentes(std::move(adyacentes)) { }

int Block::how_many_particles() const {
  return n_particulas;
}

std::vector<int> Block::GetPosition() const {
  return posicion;
}

std::vector<double> Block::GetDimensions() const {
  return tamano;
}

std::vector<int> Block::GetBloquesVecinos() const {
  return bloques_adyacentes;
}
*/