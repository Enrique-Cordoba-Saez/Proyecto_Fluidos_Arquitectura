//
// Created by Kry0m on 03/10/2023.
//

#include <iostream>
#include "block.hpp"

Block::Block(
    int n_particulas, std::vector<int> posicion,
    std::vector<double> tamano,
    std::vector<int> adyacentes
    ) : n_particulas(n_particulas), posicion(std::move(posicion)),
    tamano(std::move(tamano)),
    bloques_adyacentes(std::move(adyacentes)) {

}

int Block::how_many_particles() const{
  return n_particulas;
}

std::vector<int> Block::PosicionActual() const{
  return posicion;
}

std::vector<double> Block::Dimensiones() const{
  return tamano;
}

std::vector<int> Block::BloquesVecinos() const{
  return bloques_adyacentes;
}

