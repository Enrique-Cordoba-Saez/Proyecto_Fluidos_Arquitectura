/*#include <iostream>*/
#include <vector>
#include <cmath>
#include "sim/progargs.hpp"
/*#include "sim/particle.hpp"*/
#include "sim/grid.hpp"
/*#include <chrono>*/

ProgArgs crearProcesador(int argc, char const * const * argv);

void Parametros1(ProgArgs const & procesador, double const & Masa_Particula_m,
                 double const & Longitud_Suavizado_h, std::vector<int> & Numero_Bloques);

std::vector<double> ParametroTamanoBloque(std::vector<int> const & Numero_Bloques);

void creacionParticulas(std::vector<double> const & valoresDobles,
                        std::vector<Particle> & Particulas);

std::vector<Particle> & partesTresCuatroCinco(std::vector<int> const & Numero_Bloques,
                                              std::vector<Particle> & Particulas);



int main(int argc, const char* argv[]) {
  ProgArgs procesador = crearProcesador(argc, argv);
  // Leer archivo de entrada: devuelve vector con todos los parámetros (sin el header) en double
  std::vector<double> const valoresDobles = procesador.leerArchivo();
  // Declaración de parámetros de la simulación
  double const Masa_Particula_m = 0.0;
  double const Longitud_Suavizado_h = 0.0;
  std::vector<int> Numero_Bloques;
  Parametros1(procesador, Masa_Particula_m, Longitud_Suavizado_h, Numero_Bloques);
  std::vector<double> const Tamano_Bloques = ParametroTamanoBloque(Numero_Bloques);
  auto Bloques = crearBloques(Numero_Bloques);
  // Creación de las partículas
  std::vector<Particle> Particulas;
  creacionParticulas(valoresDobles, Particulas);
  // Iterar pasos de tiempo
  int const time_steps = procesador.getTimesteps();
  for (int i = 1; i <= time_steps; i++) {
    // 1. Reposicionamiento de cada partícula en la malla.
    reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);
    //  2. Cálculo de fuerzas y aceleraciones para cada partícula.
    calculoAceleraciones(Particulas, Longitud_Suavizado_h, Masa_Particula_m, Bloques);
    // 3. Procesamiento de colisiones, 4. Movimiento de partículas y 5. Procesamiento de límites.
    Particulas = partesTresCuatroCinco(Numero_Bloques, Particulas);
  }
  procesador.escribirArchivo(Particulas);
}




std::vector<Particle> & partesTresCuatroCinco(std::vector<int> const & Numero_Bloques,
                                              std::vector<Particle> & Particulas) {
  chocarParticulasRecinto(Particulas, Numero_Bloques);
  movimientoParticulas(Particulas);
  chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
  return Particulas;
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

[[maybe_unused]] void Parametros1(ProgArgs const & procesador, double  & Masa_Particula_m,
                 double  & Longitud_Suavizado_h, std::vector<int> & Numero_Bloques) {
  // Declaración de parámetros de la simulación
  Masa_Particula_m= Densidad_De_Fluido / pow(procesador.getPpm(), 3);
  Longitud_Suavizado_h= Multiplicador_De_Radio / procesador.getPpm();
  Numero_Bloques= calcularNumBloques(Longitud_Suavizado_h);
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
// Registra el tiempo de inicio
auto start = std::chrono::high_resolution_clock::now();
*/


/*procesador.imprimirDatos(Masa_Particula_m, Longitud_Suavizado_h, Numero_Bloques, Tamano_Bloques);*/

// printBloques(Bloques);

/*
// Registra el tiempo de finalización
auto end = std::chrono::high_resolution_clock::now();

// Calcula la duración del tiempo de ejecución en segundos
auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

// Imprime el tiempo de ejecución en segundos
std::cout << "Tiempo de ejecucion: " << duration << " segundos" << "\n";
*/