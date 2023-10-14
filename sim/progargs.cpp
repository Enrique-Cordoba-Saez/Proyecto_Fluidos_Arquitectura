#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <array>
#include <span>
#include "progargs.hpp"

// Constructor de la clase ProgArgs. Recibe el número de argumentos (argc) y
// una vista de los mismos (args_view).
ProgArgs::ProgArgs(int argc, std::vector<const char*> args) {
  // Comprobar número de argumentos
  if (argc != 3) {
    std::cerr << "Error: Invalid number of arguments: " << argc << '\n';
    exit(-1);
  }
  // Comprobar que time_steps es un entero
  try {
    time_steps = std::stoi(static_cast<std::string>(args[0]));
  } catch (std::invalid_argument const & ex) {
    std::cerr << "Error: time steps must be numeric.\n";
    exit(-1);
  }
  // Comprobar que time_steps no es negativo
  if (time_steps < 0) {
    std::cerr << "Error: Invalid number of time steps.\n";
    exit(-2);
  }
  archivoEntrada = args[1];
  archivoSalida = args[2];
}

// Método para leer el archivo de entrada
std::vector<double> ProgArgs::leerArchivo() {
  // Abrir archivo en modo binario
  std::ifstream archivo(archivoEntrada, std::ios::binary);
  std::vector<double> valoresDoble;
  if (!archivo) {
    std::cerr << "Error: Cannot open" << archivoEntrada << " for reading\n";
    exit(-3);
  }
  std::vector<char> buffer(sizeof(float));
  leerHeader(archivo, buffer);
  leerParametros(archivo, valoresDoble, buffer);
  archivo.close();
  comprobarArchivoEntrada(valoresDoble);
  return valoresDoble;
}

// Método para leer y guardar el header del archivo de entrada
void ProgArgs::leerHeader(std::ifstream & archivo, std::vector<char> & buffer) {
  // Leer header del archivo de entrada
  for (int i = 0; i < 2; ++i) {
    if (archivo.read(buffer.data(), static_cast<std::streamsize>(buffer.size()))) {
      if (i == 0) {
        float simple{};
        memcpy(&simple, buffer.data(), buffer.size());
        ppm = simple;
      } else {
        int integer{};
        memcpy(&integer, buffer.data(), buffer.size());
        num_particles = static_cast<int>(integer);
      }
    } else {
      std::cerr << "Error: Cannot read from file\n";
      archivo.close();
      exit(-3);
    }
  }
}

// Método para leer los parámetros de las partículas del archivo de entrada
void ProgArgs::leerParametros(std::ifstream & archivo, std::vector<double> & valoresDoble,
                              std::vector<char> & buffer) {
  // Leer el resto de parámetros del archivo de 4 en 4 bytes
  while (archivo.read(buffer.data(), static_cast<std::streamsize>(buffer.size()))) {
    float data_simple{};
    memcpy(&data_simple, buffer.data(), buffer.size());
    // Transformar cada elemento a doble precisión
    double const data_double = data_simple;
    valoresDoble.push_back(data_double);
  }
}

// Método para comprobar que el número de partículas especificado en el header coincide con
// el número real de partículas encontradas en el archivo
void ProgArgs::comprobarArchivoEntrada(const std::vector<double>& valoresDoble) const {
  int const exit_status = -5;
  if (num_particles <= 0) {
    std::cerr << "Error: Invalid number of particles: " << num_particles << '\n';
    exit(exit_status);
  }
  long const found_particles = static_cast<long>(valoresDoble.size() / 9);
  if (num_particles != found_particles) {
    std::cerr << "Error: Number of particles mismatch. Header: "
              << num_particles << ", Found: " << found_particles << '\n';
    exit(exit_status);
  }
}

// Método de prueba para imprimir todos los atributos de la clase ProgArgs
void ProgArgs::imprimirAtributos() {
  std::cout << "Atributos de la clase ProgArgs:\nPasos de tiempo: " << time_steps
            << "\nArchivo de entrada: " << archivoEntrada << "\nArchivo de salida: "
            << archivoSalida << "\nPartículas por metro: " << ppm << "\nNúmero de partículas: "
            << num_particles <<'\n';
}

// Método para escribir todos los datos finales en el archivo de salida
void ProgArgs::escribirArchivo(std::vector<double> valoresDoble) {
  // Transformar parámetros de vuelta a simple precisión
  std::vector<float> const valoresSimple(valoresDoble.begin(), valoresDoble.end());
  // Abrir archivo en modo binario
  std::ofstream archivo_out(archivoSalida, std::ios::binary);
  if (!archivo_out) {
    std::cerr << "Error: Cannot open" << archivoSalida << " for writing\n";
    exit(-4);
  }
  for (const auto& valor : valoresSimple) {
    std::array<char, sizeof(float)> buffer{};
    std::memcpy(buffer.data(), &valor, sizeof(float));
    archivo_out.write(buffer.data(), buffer.size());
  }
  archivo_out.close();
}

int ProgArgs::getTimesteps() const {
  return time_steps;
}

double ProgArgs::getPpm() const {
  return ppm;
}

int ProgArgs::getNumParticles() const {
  return num_particles;
}