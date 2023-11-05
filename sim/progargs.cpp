#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <array>
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
void ProgArgs::imprimirDatos(double masa_particula, double longitud_suavizado,
                             std::vector<int> num_bloques, std::vector<double> tam_bloque) const {
  std::cout << "Number of particles: " << num_particles << "\nParticles per meter: " << ppm
            << "\nSmoothing length: " << longitud_suavizado << "\nParticle mass: " << masa_particula
            << "\nGrid size: " << num_bloques[0] << " x " << num_bloques[1] << " x " << num_bloques[2]
            << "\nNumber of blocks: " << num_bloques[0]*num_bloques[1]*num_bloques[2]
            << "\nBlock size: " << tam_bloque[0] << " x " << tam_bloque[1] << " x " << tam_bloque[2] << '\n';
}

// Método para escribir todos los datos finales en el archivo de salida
void ProgArgs::escribirArchivo(std::vector<Particle> const& particles) {
  // Abrir archivo en modo binario
  std::ofstream archivo_out(archivoSalida, std::ios::binary);
  if (!archivo_out) {
    std::cerr << "Error: Cannot open " << archivoSalida << " for writing\n";
    exit(-4);
  }
  // Escribir header del archivo
  escribirHeader(archivo_out);
  // Escribir los parámetros double de las partículas en formato float
  escribirParametros(particles, archivo_out);
  archivo_out.close();
}

void ProgArgs::escribirParametros(std::vector<Particle> const & particles, std::ofstream & archivo_out) {
  for (const auto& particle : particles) {
    std::vector<double> const position = particle.getPosition();
    std::vector<double> const head_vector = particle.getHeadVector();
    std::vector<double> const velocity = particle.getVelocityVector();
    std::array<char, sizeof(float)> buffer{};
    // Escribir posición
    for (int i = 0; i < 3; ++i) {
      auto current_position = static_cast<float>(position[i]);
      std::memcpy(buffer.data(), &current_position, sizeof(float));
      archivo_out.write(buffer.data(), buffer.size());
    }
    // Escribir head vector
    for (int i = 0; i < 3; ++i) {
      auto current_head_vector = static_cast<float>(head_vector[i]);
      std::memcpy(buffer.data(), &current_head_vector, sizeof(float));
      archivo_out.write(buffer.data(), buffer.size());
    }
    // Escribir velocidad
    for (int i = 0; i < 3; ++i) {
      auto current_velocity = static_cast<float>(velocity[i]);
      std::memcpy(buffer.data(), &current_velocity, sizeof(float));
      archivo_out.write(buffer.data(), buffer.size());
    }
  }
}

void ProgArgs::escribirHeader(std::ofstream & archivo_out) const {
  // Escribir ppm
  std::array<char, sizeof(float)> bufferSimple{};
  auto const simple_ppm = static_cast<float>(ppm);
  memcpy(bufferSimple.data(), &simple_ppm, sizeof(float));
  archivo_out.write(bufferSimple.data(), bufferSimple.size());

  // Escribir num_particles
  std::array<char, sizeof(int)> bufferInt{};
  memcpy(bufferInt.data(), &num_particles, sizeof(int));
  archivo_out.write(bufferInt.data(), bufferInt.size());
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

std::string ProgArgs:: getArchivoEntrada() const {
  return archivoEntrada;
}

std::string ProgArgs:: getArchivoSalida() const {
  return archivoSalida;
}
