#include "gtest/gtest.h"
#include "sim/particle.hpp"
#include "sim/grid.hpp"

// Para una partícula dentro de los límites del recinto, comprobar que se reposiciona en el bloque correcto
TEST(ReposicionarParticulasTest, CorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                           std::vector<double>{-0.055703137, 0.015217851, 0.029195517},
                                           0.0, std::vector<double> {},
                                           std::vector<double> {});
  Particulas.push_back(nuevaParticula);
  const std::vector<int> Numero_Bloques = {15, 21, 15};
  const std::vector<double> Tamano_Bloques = {0.00866667, 0.00857143, 0.00866667};
  reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques);
  ASSERT_EQ(Particulas[0].getBlockIndexes(), (std::vector<int>{1, 11, 10}));
}

// Para una partícula por debajo de los límites del recinto, comprobar que se le asigna el primer bloque
TEST(ReposicionarParticulasTest, BajoRecintoCorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                           std::vector<double>{-0.075703137, -0.095217851, -0.069195517},
                                           0.0, std::vector<double> {},
                                           std::vector<double> {});
  Particulas.push_back(nuevaParticula);
  const std::vector<int> Numero_Bloques = {15, 21, 15};
  const std::vector<double> Tamano_Bloques = {0.00866667, 0.00857143, 0.00866667};
  reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques);
  ASSERT_EQ(Particulas[0].getBlockIndexes(), (std::vector<int>{0, 0, 0}));
}

// Para una partícula por encima de los límites del recinto, comprobar que se le asigna el último bloque
TEST(ReposicionarParticulasTest, SobreRecintoCorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                           std::vector<double>{0.075703137, 0.15217851, 0.069195517},
                                           0.0, std::vector<double> {},
                                           std::vector<double> {});
  Particulas.push_back(nuevaParticula);
  const std::vector<int> Numero_Bloques = {15, 21, 15};
  const std::vector<double> Tamano_Bloques = {0.00866667, 0.00857143, 0.00866667};
  reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques);
  ASSERT_EQ(Particulas[0].getBlockIndexes(),
            (std::vector<int>{Numero_Bloques[0] - 1 , Numero_Bloques[1] - 1, Numero_Bloques[2] - 1}));
}

// ------------------------------------------------------------

// Para una partícula cualquiera, comprobar que se actualizan correctamente sus parámetros
TEST(MovimientoParticulasTest, CorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                           std::vector<double>{-0.055703137, 0.015217851, 0.029195517},
                                           0.0, std::vector<double>{0.21121116, 0.11840522, 0.14053094},
                                           std::vector<double>{0.21360235, 0.12304694, 0.13833959});
  Particulas.push_back(nuevaParticula);
  movimientoParticulas(Particulas);
  std::vector<float> position;
  for (int i = 0; i <= 2; ++i) {
    position.push_back(static_cast<float>(Particulas[0].getPosition()[i]));
  }

  ASSERT_EQ(position, (std::vector<float>{-0.055491924, 0.015326456, 0.029336048}));
  ASSERT_EQ(Particulas[0].getVelocityVector(), (std::vector<double>{0.21121116, 0.11350522, 0.14053094}));
  ASSERT_EQ(Particulas[0].getHeadVector(), (std::vector<double>{0.21121116, 0.10860522, 0.14053094}));
}