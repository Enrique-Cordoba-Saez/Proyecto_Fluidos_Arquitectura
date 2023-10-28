#ifndef PROYECTO_FLUIDOS_ARQUITECTURA_PROGARGS_HPP
#define PROYECTO_FLUIDOS_ARQUITECTURA_PROGARGS_HPP

#include <span>

class ProgArgs {
  private:
    int time_steps;
    std::string archivoEntrada;
    std::string archivoSalida;
    double ppm{};
    int num_particles{};

  public:
    ProgArgs(int argc, std::vector<const char*> args);
    std::vector<double> leerArchivo();
    void escribirArchivo(std::vector<double> valoresDoble);
    void comprobarArchivoEntrada(const std::vector<double>& valoresDoble) const;
    void imprimirDatos(double masa_particula, double longitud_suavizado,
                       std::vector<int> num_bloques, std::vector<double> tam_bloque) const;
    void leerHeader(std::ifstream & archivo, std::vector<char> & buffer);
    static void leerParametros(std::ifstream & archivo, std::vector<double> & valoresDoble,
                               std::vector<char> & buffer);

    [[nodiscard]] int getTimesteps() const;
    [[nodiscard]] double getPpm() const;
    [[nodiscard]] int getNumParticles() const;

};

#endif //PROYECTO_FLUIDOS_ARQUITECTURA_PROGARGS_HPP
