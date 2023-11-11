#include <iostream>
#include <vector>
#include <cmath>
#include "sim/progargs.hpp"
#include "sim/particle.hpp"
#include "sim/grid.hpp"
#include <chrono>


int main(int argc, const char* argv[]) {
  // Registra el tiempo de inicio
  auto start = std::chrono::high_resolution_clock::now();

  std::span const args_view{argv, static_cast<size_t>(argc)};
  std::vector const args(args_view.begin() + 1, args_view.end());
  // Crear objeto de la clase ProgArgs
  auto procesador = ProgArgs(argc - 1, args);
  // Leer archivo de entrada: devuelve vector con todos los parámetros (sin el header) en double
  std::vector<double> const valoresDobles = procesador.leerArchivo();

  //Declaración de parámetros de la simulación
  double const Masa_Particula_m = Densidad_De_Fluido / pow(procesador.getPpm(), 3);
  double const Longitud_Suavizado_h = Multiplicador_De_Radio / procesador.getPpm();
  const std::vector<int> Numero_Bloques = calcularNumBloques(Longitud_Suavizado_h);
  const std::vector<double> Tamano_Bloques = {(Limite_Superior[0]-Limite_Inferior[0])/double(Numero_Bloques[0]),
                                              (Limite_Superior[1]-Limite_Inferior[1])/double(Numero_Bloques[1]),
                                              (Limite_Superior[2]-Limite_Inferior[2])/double(Numero_Bloques[2])
  };
  auto Bloques = crearBloques(Numero_Bloques);
  procesador.imprimirDatos(Masa_Particula_m, Longitud_Suavizado_h,
                           Numero_Bloques, Tamano_Bloques);

  //Creación de las partículas
  std::vector<Particle> Particulas;
  for (size_t i = 0; i < valoresDobles.size(); i+=nueve){
    Particle const nuevaParticula = Particle(std::vector<double>{0.0, 0.0, 0.0}, std::vector<double>{valoresDobles[i], valoresDobles[i+1], valoresDobles[i+2]},
                                             0.0, std::vector<double> {valoresDobles[i+3], valoresDobles[i+4], valoresDobles[i+cinco]},
                                             std::vector<double> {valoresDobles[i+seis], valoresDobles[i+siete], valoresDobles[i+ocho]});
    Particulas.push_back(nuevaParticula);
  }
  // Iterar pasos de tiempo
  int const time_steps = procesador.getTimesteps();
  for (int i = 1; i <= time_steps; i++) {
    // 1. Reposicionamiento de cada partícula en la malla.
    reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);
    //printBloques(Bloques);
    // 2. Cálculo de fuerzas y aceleraciones para cada partícula.
    std::cout << "----------------------------------------------------------------------------" << std::endl;
    std::cout << "Iteracion " << i << std::endl;
    calculoAceleraciones(Particulas, Longitud_Suavizado_h, Masa_Particula_m, Bloques);

    // 3. Procesamiento de colisiones.
    chocarParticulasRecinto(Particulas, Numero_Bloques);
    // 4. Movimiento de partículas.
    movimientoParticulas(Particulas);
    // 5. Procesamiento de límites.
    chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
  }
  procesador.escribirArchivo(Particulas);

  // Registra el tiempo de finalización
  auto end = std::chrono::high_resolution_clock::now();

  // Calcula la duración del tiempo de ejecución en segundos
  auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

  // Imprime el tiempo de ejecución en segundos
  std::cout << "Tiempo de ejecucion: " << duration << " segundos" << std::endl;
}
