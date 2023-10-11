#include <iostream>
#include <vector>
#include <span>
#include "sim/progargs.hpp"

int main(int argc, char* argv[]) {
  std::span const args_view{argv, static_cast<size_t>(argc)};
  // Crear objeto de la clase ProgArgs
  auto procesador = ProgArgs(argc - 1, args_view);
  // Leer archivo de entrada: devuelve vector con todos los parámetros (sin el header) en double
  std::vector<double> const valoresDobles = procesador.leerArchivo();
  // procesador.escribirArchivo(valoresDobles);
  procesador.imprimirAtributos();
}
