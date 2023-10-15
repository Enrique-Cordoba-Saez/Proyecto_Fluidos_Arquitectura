//
// Created by Kry0m on 03/10/2023.
//

#ifndef BLOCK_HPP
#define BLOCK_HPP
#include <iostream>
#include <vector>
#include <array>


//typedef std::array<int, 8> adyacentes;

/*
struct Vector_adyacencias {
    std::vector<int> lista_adyacencias

};
*/
const int diecisiete = 17;

class Block {
  private:
    int n_particulas;
    std::vector<int> posicion;
    std::vector<double> tamano;
    std::vector<int> bloques_adyacentes;

  public:
    Block(
        int n_particulas = 0,
        std::vector<int> posicion = std::vector<int>(3, 0),
        std::vector<double> tamano = std::vector<double>(3, 0.0),
        std::vector<int> lista_adyacencias = std::vector<int>(diecisiete, -1)
    );

    [[nodiscard]] int how_many_particles () const;
    [[nodiscard]] std::vector<int> PosicionActual() const;
    [[nodiscard]] std::vector<double> Dimensiones() const;
    [[nodiscard]] std::vector<int> BloquesVecinos() const;

};

#endif //PROYECTO_FLUIDOS_ARQUITECTURA_BLOCK_HPP
