#include <iostream>
#include <vector>
#include <span>
#include <cmath>
#include "sim/progargs.hpp"
#include "sim/particle.hpp"


//Declaración de constantes escalares de la simulación
  const float Multiplicador_De_Radio = 1.695;
  const int Densidad_De_Fluido = 1000;
  const float Presion_De_Rigidez = 3.0;
  const int Colisiones_De_Rigidez = 30000;
  const float Amortiguamiento = 128.0;
  const float Viscosidad = 0.4;
  const float Tamano_de_particula = 0.0002;
  const float Paso_de_tiempo = 0.001;



int main(int argc, const char* argv[]) {

  //Declaración de constantes vectoriales de la simulación
  const std::vector<float> Aceleracion_Externa{0.0, -9.8, 0.0};
  const std::vector<float> Limite_Superior{0.065, 0.1, 0.065};
  const std::vector<float> Limite_Inferior{-0.065, -0.08, -0.065};

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
