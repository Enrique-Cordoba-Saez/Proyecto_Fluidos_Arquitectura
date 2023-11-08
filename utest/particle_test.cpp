#include "gtest/gtest.h"
#include "sim/particle.hpp"
#include "sim/grid.hpp"

// Para una partícula dentro de los límites del recinto, comprobar que se reposiciona en el bloque correcto
/*TEST(ReposicionarParticulasTest, CorrectoTest) {
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
*/
// Para una partícula por debajo de los límites del recinto, comprobar que se le asigna el primer bloque
/*
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
*/
// Para una partícula por encima de los límites del recinto, comprobar que se le asigna el último bloque
/*
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
*/
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

// ------------------------------------------------------------

TEST(ColisionesParticulasTest_Parte3_Test, TestEjeX_Minimo){
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                           std::vector<double>{-0.064903137, 0.015217851, 0.029195517},
                                           0.0, std::vector<double> {-0.296863, 0.11840522, 0.14053094},
                                           std::vector<double> {0.21360235, 0.12304694, 0.13833959});

  const int Ppm= 19523;
  double const Longitud_Suavizado_h = Multiplicador_De_Radio / Ppm;
  const std::vector<int> Numero_Bloques = {int(std::floor((Limite_Superior[0]-Limite_Inferior[0])/Longitud_Suavizado_h)),
                         int(std::floor((Limite_Superior[1]-Limite_Inferior[1])/Longitud_Suavizado_h)),
                         int(std::floor((Limite_Superior[2]-Limite_Inferior[2])/Longitud_Suavizado_h))
};
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada x ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(0, 0, 0);
  Particulas.push_back(nuevaParticula);

  double const new_position_x = -0.064903137 + -0.296863 * Paso_de_tiempo;
  double const diferencia_con_limite = Tamano_de_particula - new_position_x + Limite_Inferior[0];
  const double newExpectedAceleration = 0.0 + Colisiones_De_Rigidez * diferencia_con_limite
                                          - Amortiguamiento * 0.21360235;
  chocarParticulasRecinto(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getAcceleration(), (std::vector<double>{newExpectedAceleration, -9.8, 0}));
}


TEST(ColisionesParticulasTest_Parte3_Test, TestEjeZ_Maximo){
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                     std::vector<double>{-0.055703137, 0.015217851, 0.064903137},
                                     0.0, std::vector<double> {0.21121116, 0.11840522, 0.296863},
                                     std::vector<double> {0.13833959, 0.12304694, 0.21360235});

  const int Ppm= 19523;
  double const Longitud_Suavizado_h = Multiplicador_De_Radio / Ppm;
  const std::vector<int> Numero_Bloques = {int(std::floor((Limite_Superior[0]-Limite_Inferior[0])/Longitud_Suavizado_h)),
                                           int(std::floor((Limite_Superior[1]-Limite_Inferior[1])/Longitud_Suavizado_h)),
                                           int(std::floor((Limite_Superior[2]-Limite_Inferior[2])/Longitud_Suavizado_h))
  };
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada z ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(Numero_Bloques[0] - 1, Numero_Bloques[1] - 1, Numero_Bloques[2] - 1);
  Particulas.push_back(nuevaParticula);

  double const new_position_z = 0.064903137 + 0.296863 * Paso_de_tiempo;
  double const diferencia_con_limite = Tamano_de_particula + new_position_z - Limite_Superior[2];
  const double newExpectedAceleration = 0.0 - Colisiones_De_Rigidez * diferencia_con_limite
                                        - Amortiguamiento * 0.21360235;
  chocarParticulasRecinto(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getAcceleration(), (std::vector<double>{0.0, -9.8, newExpectedAceleration}));
}

// ------------------------------------------------------------

TEST(InteraccionesLimitesRecinto_Parte5_Test, TestEjeZ_Minimo){
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                     std::vector<double>{-0.055703137, 0.015217851, -0.067149506},
                                     0.0, std::vector<double> {0.21121116, 0.11840522, 0.14053094},
                                     std::vector<double> {0.21360235, 0.12304694, 0.13833959});

  const int Ppm= 19523;
  double const Longitud_Suavizado_h = Multiplicador_De_Radio / Ppm;
  const std::vector<int> Numero_Bloques = {int(std::floor((Limite_Superior[0]-Limite_Inferior[0])/Longitud_Suavizado_h)),
                                            int(std::floor((Limite_Superior[1]-Limite_Inferior[1])/Longitud_Suavizado_h)),
                                            int(std::floor((Limite_Superior[2]-Limite_Inferior[2])/Longitud_Suavizado_h))
  };
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada z ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(0, 0, 0);
  Particulas.push_back(nuevaParticula);

  double const diferencia_con_limite = -0.067149506 - Limite_Inferior[2];
  const double new_position_z = Limite_Inferior[2] - diferencia_con_limite;
  chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getPosition(), (std::vector<double>{-0.055703137, 0.015217851, new_position_z}));

  //En los dos casos inferiores sencillamente se invierte el signo del valor en la coordenada z
  ASSERT_EQ(Particulas[0].getHeadVector(), (std::vector<double>{0.21121116, 0.11840522, -0.14053094}));
  ASSERT_EQ(Particulas[0].getVelocityVector(), (std::vector<double>{0.21360235, 0.12304694, -0.13833959}));
}

TEST(InteraccionesLimitesRecinto_Parte5_Test, TestEjeY_Maximo){
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula = Particle(std::vector<double>{0.0, -9.8, 0.0},
                                     std::vector<double>{-0.055703137, 0.105217851, 0.029195517},
                                     0.0, std::vector<double> {0.21121116, 0.11840522, 0.14053094},
                                     std::vector<double> {0.21360235, 0.12304694, 0.13833959});

  const int Ppm= 19523;
  double const Longitud_Suavizado_h = Multiplicador_De_Radio / Ppm;
  const std::vector<int> Numero_Bloques = {int(std::floor((Limite_Superior[0]-Limite_Inferior[0])/Longitud_Suavizado_h)),
                                           int(std::floor((Limite_Superior[1]-Limite_Inferior[1])/Longitud_Suavizado_h)),
                                           int(std::floor((Limite_Superior[2]-Limite_Inferior[2])/Longitud_Suavizado_h))
  };
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada z ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(Numero_Bloques[0] - 1, Numero_Bloques[1] - 1, Numero_Bloques[2] - 1);
  Particulas.push_back(nuevaParticula);

  double const diferencia_con_limite = Limite_Superior[1] - 0.105217851;
  const double new_position_y = Limite_Superior[1] + diferencia_con_limite;
  chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getPosition(), (std::vector<double>{-0.055703137, new_position_y, 0.029195517}));

  //En los dos casos inferiores sencillamente se invierte el signo del valor en la coordenada y
  ASSERT_EQ(Particulas[0].getHeadVector(), (std::vector<double>{0.21121116, -0.11840522, 0.14053094}));
  ASSERT_EQ(Particulas[0].getVelocityVector(), (std::vector<double>{0.21360235, -0.12304694, 0.13833959}));

}