/*#include <iostream>*/
#include "sim/progargs.hpp"

/*#include <cmath>*/
#include <vector>
/*#include "sim/particle.hpp"*/
#include "sim/grid.hpp"

/*#include <chrono>*/

int main(int argc, char const * argv[]) {
  ProgArgs procesador = crearProcesador(argc, argv);
  // Leer archivo de entrada: devuelve vector con todos los parámetros (sin el header) en double
  std::vector<double> const valoresDobles = procesador.leerArchivo();
  // Declaración de parámetros de la simulación
  double Masa_Particula_m     = 0.0;
  double Longitud_Suavizado_h = 0.0;
  std::vector<int> Numero_Bloques;
  Parametros1(procesador, Masa_Particula_m, Longitud_Suavizado_h, Numero_Bloques);
  std::vector<double> const Tamano_Bloques = ParametroTamanoBloque(Numero_Bloques);
  auto Bloques                             = crearBloques(Numero_Bloques);
  procesador.imprimirDatos(Masa_Particula_m, Longitud_Suavizado_h, Numero_Bloques, Tamano_Bloques);
  // Creación de las partículas
  std::vector<Particle> Particulas;
  creacionParticulas(valoresDobles, Particulas);
  // Iterar pasos de tiempo
  int const time_steps = procesador.getTimesteps();
  for (int i = 1; i <= time_steps; i++) {
    // 1. Reposicionamiento de cada partícula en la malla y 2. Cálculo de fuerzas y aceleraciones.
    reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);
    calculoAceleraciones(Particulas, Longitud_Suavizado_h, Masa_Particula_m, Bloques);
    // 3. Procesamiento de colisiones, 4. Movimiento de partículas y 5. Procesamiento de límites.
    partesTresCuatroCinco(Numero_Bloques, Particulas);
  }
  procesador.escribirArchivo(Particulas);
}

/*
// Registra el tiempo de inicio
auto start = std::chrono::high_resolution_clock::now();
*/

/*procesador.imprimirDatos(Masa_Particula_m, Longitud_Suavizado_h, Numero_Bloques,
 * Tamano_Bloques);*/

// printBloques(Bloques);

/*
// Registra el tiempo de finalización
auto end = std::chrono::high_resolution_clock::now();

// Calcula la duración del tiempo de ejecución en segundos
auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

// Imprime el tiempo de ejecución en segundos
std::cout << "Tiempo de ejecucion: " << duration << " segundos" << "\n";
*/