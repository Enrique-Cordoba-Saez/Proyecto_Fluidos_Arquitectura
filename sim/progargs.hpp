#ifndef PROYECTO_FLUIDOS_ARQUITECTURA_PROGARGS_HPP
#define PROYECTO_FLUIDOS_ARQUITECTURA_PROGARGS_HPP

#include "particle.hpp"

#include <span>

class ProgArgs {
  private:
    int time_steps;
    std::string archivoEntrada;
    std::string archivoSalida;
    double ppm{};
    int num_particles{};

  public:
    ProgArgs(int argc, std::vector<char const *> args);
    std::vector<double> leerArchivo();
    void escribirArchivo(std::vector<Particle> const & particles);
    void comprobarArchivoEntrada(std::vector<double> const & valoresDoble) const;
    void imprimirDatos(double masa_particula, double longitud_suavizado,
                       std::vector<int> num_bloques, std::vector<double> tam_bloque) const;
    void leerHeader(std::ifstream & archivo, std::vector<char> & buffer);
    static void leerParametros(std::ifstream & archivo, std::vector<double> & valoresDoble,
                               std::vector<char> & buffer);
    void escribirHeader(std::ofstream & archivo_out) const;
    static void escribirParametros(std::vector<Particle> const & particles,
                                   std::ofstream & archivo_out);

    [[nodiscard]] int getTimesteps() const;
    [[nodiscard]] double getPpm() const;
    [[nodiscard]] int getNumParticles() const;
    [[nodiscard]] std::string getArchivoEntrada() const;
    [[nodiscard]] std::string getArchivoSalida() const;
};

#endif  // PROYECTO_FLUIDOS_ARQUITECTURA_PROGARGS_HPP
