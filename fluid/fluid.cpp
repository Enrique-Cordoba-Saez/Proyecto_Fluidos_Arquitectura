#include <iostream>
#include <vector>
#include <span>
#include "sim/progargs.hpp"
#include "sim/particle.hpp"

int main(int argc, const char* argv[]) {
  std::span const args_view{argv, static_cast<size_t>(argc)};
  std::vector const args(args_view.begin() + 1, args_view.end());
  // Crear objeto de la clase ProgArgs
  auto procesador = ProgArgs(argc - 1, args);
  // Leer archivo de entrada: devuelve vector con todos los parámetros (sin el header) en double
  std::vector<double> const valoresDobles = procesador.leerArchivo();
  procesador.imprimirAtributos();
  // std::vector<Particle> particles = readParticlesFromFile(filename, commonDensity, commonAcceleration);
  // Iterar pasos de tiempo
  int const time_steps = procesador.getTimesteps();
  for (int i = 1; i <= time_steps; i++) {
    // En cada proceso, para cada partícula: for particle in particles:
    // 1. Reposicionamiento de cada partícula en la malla.
    // 2. Cálculo de fuerzas y aceleraciones para cada partícula.
    // 3. Procesamiento de colisiones.
    // 4. Movimiento de partículas.
    // 5. Procesamiento de límites.
  }
  // procesador.escribirArchivo(valoresDobles);
}
